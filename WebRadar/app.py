import os
import time
import threading
import requests
from flask import Flask, render_template, jsonify, request, send_file, abort
from flask_cors import CORS

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
STATIC_DIR = os.path.join(BASE_DIR, "static")
MAP_STYLES_DIR = os.path.join(STATIC_DIR, "mapStyles")

app = Flask(
    __name__,
    template_folder=os.path.join(BASE_DIR, "templates"),
    static_folder=STATIC_DIR,
)
CORS(app)

# =============================================================================
# Configuration & Shared Telemetry State (Render Cloud Sync + Local C++ Bridge)
# =============================================================================
CPP_LOCAL_URL = os.environ.get("P4M_LOCAL_CPP_URL", "http://127.0.0.1:8080")
TILE_UPSTREAM_BASE = "https://raw.githubusercontent.com/Trusted-Studios/mapStyles/main"

state_lock = threading.Lock()
tile_http_session = requests.Session()

# Latest telemetry pushed by p4m C++ client (/api/sync) or polled from 127.0.0.1:8080
telemetry_state = {
    "last_sync_ts": 0.0,
    "source": "standby",  # "cpp_push", "cpp_local_poll", or "standby"
    "last_teleport": None,
    "blips": {
        "local": None,
        "players": [],
        "vehicles": [],
        "map_blips": [],
    },
    "config": {
        "esp_enabled": True,
        "esp_box": True,
        "esp_skeleton": True,
        "esp_snaplines": False,
        "esp_health": True,
        "esp_armor": True,
        "esp_names": True,
        "esp_weapon": True,
        "esp_headcircle": True,
        "esp_distance": True,
        "esp_ignore_npcs": True,
        "esp_ignore_dead": True,
        "esp_max_distance": 250,
        "esp_admin": True,
        "esp_admin_rainbow": True,
        "vehesp_enabled": False,
        "vehesp_names": True,
        "vehesp_snaplines": False,
        "vehesp_distance": True,
        "aimbot_enabled": False,
        "aimbot_fov": 180,
        "aimbot_speed": 12,
        "aimbot_distance": 240,
        "aimbot_visible": False,
        "silentaim_enabled": False,
        "silentaim_fov": 40,
        "silentaim_distance": 200,
        "triggerbot_enabled": False,
        "godmode": False,
        "noclip": False,
        "noclip_speed": 2.0,
        "fastrun": False,
        "stamina": False,
        "ammo": False,
        "norecoil": False,
        "nospread": False,
    },
    "admin_watchlist": ["admin", "mod", "staff", "owner", "txadmin", "anticheat"],
}

# Command queue drained by p4m C++ client on every /api/sync heartbeat
pending_commands = []


def poll_local_cpp_loop():
    """Background worker: if p4m.exe is running locally on 127.0.0.1:8080,
    automatically pulls live blips and config without needing manual setup."""
    session = requests.Session()
    while True:
        try:
            now = time.time()
            with state_lock:
                if (
                    now - telemetry_state["last_sync_ts"] < 3.0
                    and telemetry_state["source"] == "cpp_push"
                ):
                    time.sleep(0.5)
                    continue

            r_blips = session.get(f"{CPP_LOCAL_URL}/api/blips", timeout=0.35)
            if r_blips.status_code == 200:
                blips_data = r_blips.json()
                r_cfg = session.get(f"{CPP_LOCAL_URL}/api/config", timeout=0.35)
                cfg_data = r_cfg.json() if r_cfg.status_code == 200 else None

                with state_lock:
                    telemetry_state["blips"] = blips_data
                    if cfg_data and isinstance(cfg_data, dict):
                        telemetry_state["config"].update(cfg_data)
                    telemetry_state["last_sync_ts"] = time.time()
                    telemetry_state["source"] = "cpp_local_poll"
        except Exception:
            pass
        time.sleep(0.25)


poller_thread = threading.Thread(target=poll_local_cpp_loop, daemon=True)
poller_thread.start()


# =============================================================================
# Web UI & Authentic GTA V Map Tile Server Routes
# =============================================================================
@app.route("/")
def index():
    return render_template("index.html")


@app.route("/mapStyles/<style>/<int:z>/<int:x>/<int:y>.<ext>")
def serve_gta_tile(style, z, x, y, ext):
    """Serves authentic GTA V raster tiles (styleAtlas, styleSatelite, styleGrid)
    from local disk cache, fetching and caching higher zoom levels on demand."""
    if style not in ("styleAtlas", "styleSatelite", "styleGrid"):
        abort(404)
    if ext not in ("jpg", "png") or z < 0 or z > 6 or x < 0 or y < 0:
        abort(404)

    rel_path = os.path.join(style, str(z), str(x), f"{y}.{ext}")
    local_path = os.path.join(MAP_STYLES_DIR, rel_path)

    if os.path.isfile(local_path) and os.path.getsize(local_path) > 0:
        resp = send_file(local_path)
        resp.headers["Cache-Control"] = "public, max-age=31536000, immutable"
        return resp

    # On-demand fetch & cache for z=5 tiles not pre-bundled
    upstream_url = f"{TILE_UPSTREAM_BASE}/{style}/{z}/{x}/{y}.{ext}"
    try:
        r = tile_http_session.get(upstream_url, timeout=6)
        if r.status_code == 200 and r.content:
            os.makedirs(os.path.dirname(local_path), exist_ok=True)
            with open(local_path, "wb") as f:
                f.write(r.content)
            resp = send_file(local_path)
            resp.headers["Cache-Control"] = "public, max-age=31536000, immutable"
            return resp
    except Exception:
        pass

    # Fallback to authentic ocean tile when outside island tile bounds
    empty_tile = os.path.join(MAP_STYLES_DIR, style, "empty.jpg")
    if not os.path.isfile(empty_tile):
        empty_tile = os.path.join(MAP_STYLES_DIR, "styleAtlas", "empty.jpg")
    if os.path.isfile(empty_tile):
        resp = send_file(empty_tile)
        resp.headers["Cache-Control"] = "public, max-age=86400"
        return resp

    abort(404)


# =============================================================================
# REST API Endpoints (Compatible with Browser UI and p4m C++ Client)
# =============================================================================
@app.route("/api/status", methods=["GET"])
def api_status():
    with state_lock:
        age = time.time() - telemetry_state["last_sync_ts"]
        connected = telemetry_state["last_sync_ts"] > 0 and age < 4.0
        source = telemetry_state["source"] if connected else "standby"
        return jsonify({
            "status": "online",
            "connected_to_game": connected,
            "source": source,
            "last_sync_age_sec": round(age, 2) if telemetry_state["last_sync_ts"] > 0 else None,
            "pending_commands": len(pending_commands),
            "last_teleport": telemetry_state.get("last_teleport"),
        })


@app.route("/api/blips", methods=["GET"])
def api_blips():
    with state_lock:
        age = time.time() - telemetry_state["last_sync_ts"]
        connected = telemetry_state["last_sync_ts"] > 0 and age < 4.0
        if connected:
            data = dict(telemetry_state["blips"])
            data["source"] = telemetry_state["source"]
            data["connected"] = True
            data["last_teleport"] = telemetry_state.get("last_teleport")
            return jsonify(data)
        else:
            return jsonify({
                "local": None,
                "players": [],
                "vehicles": [],
                "map_blips": [],
                "source": "standby",
                "connected": False,
                "last_teleport": telemetry_state.get("last_teleport"),
            })


@app.route("/api/config", methods=["GET"])
def api_config():
    with state_lock:
        return jsonify(telemetry_state["config"])


@app.route("/api/set", methods=["GET", "POST"])
def api_set():
    params = {}
    if request.args:
        params.update(request.args.to_dict())
    if request.is_json:
        body = request.get_json(silent=True) or {}
        params.update({k: str(v) for k, v in body.items()})

    with state_lock:
        for k, v in params.items():
            if k in telemetry_state["config"]:
                cur = telemetry_state["config"][k]
                if isinstance(cur, bool):
                    telemetry_state["config"][k] = str(v).lower() in ("true", "1", "yes", "on")
                elif isinstance(cur, int):
                    try:
                        telemetry_state["config"][k] = int(float(v))
                    except ValueError:
                        pass
                elif isinstance(cur, float):
                    try:
                        telemetry_state["config"][k] = float(v)
                    except ValueError:
                        pass

        pending_commands.append({"type": "set_config", "params": params})

    try:
        requests.get(f"{CPP_LOCAL_URL}/api/set", params=params, timeout=0.25)
    except Exception:
        pass

    return jsonify({"status": "ok", "updated": params})


@app.route("/api/teleport", methods=["GET", "POST"])
def api_teleport():
    x = request.args.get("x")
    y = request.args.get("y")
    z = request.args.get("z")

    if request.is_json:
        body = request.get_json(silent=True) or {}
        x = body.get("x", x)
        y = body.get("y", y)
        z = body.get("z", z)

    if x is None or y is None:
        return jsonify({"status": "error", "message": "Missing x or y coordinates"}), 400

    try:
        fx = float(x)
        fy = float(y)
        fz = float(z) if z is not None else 35.0
    except ValueError:
        return jsonify({"status": "error", "message": "Invalid coordinate values"}), 400

    with state_lock:
        if telemetry_state["blips"].get("local") and isinstance(
            telemetry_state["blips"]["local"], dict
        ):
            telemetry_state["blips"]["local"]["x"] = fx
            telemetry_state["blips"]["local"]["y"] = fy
            telemetry_state["blips"]["local"]["z"] = fz

        telemetry_state["last_teleport"] = {
            "x": round(fx, 2),
            "y": round(fy, 2),
            "z": round(fz, 2),
            "ts": round(time.time(), 2),
        }

        pending_commands.append({
            "type": "teleport",
            "x": fx,
            "y": fy,
            "z": fz,
        })

    forwarded_local = False
    try:
        r = requests.get(
            f"{CPP_LOCAL_URL}/api/teleport",
            params={"x": fx, "y": fy, "z": fz},
            timeout=0.25,
        )
        forwarded_local = r.status_code == 200
    except Exception:
        pass

    return jsonify({
        "status": "ok",
        "forwarded_local": forwarded_local,
        "queued_for_sync": True,
        "teleported_to": {"x": fx, "y": fy, "z": fz},
    })


@app.route("/api/admin/watchlist", methods=["GET"])
def api_admin_watchlist():
    with state_lock:
        return jsonify({"watchlist": telemetry_state["admin_watchlist"]})


@app.route("/api/admin/add", methods=["GET", "POST"])
def api_admin_add():
    name = request.args.get("name", "").strip()
    if request.is_json:
        body = request.get_json(silent=True) or {}
        name = str(body.get("name", name)).strip()

    if not name:
        return jsonify({"status": "error", "message": "Missing admin identifier"}), 400

    lower = name.lower()
    with state_lock:
        if lower not in telemetry_state["admin_watchlist"]:
            telemetry_state["admin_watchlist"].append(lower)
        pending_commands.append({"type": "admin_add", "name": lower})

    try:
        requests.get(f"{CPP_LOCAL_URL}/api/admin/add", params={"name": lower}, timeout=0.25)
    except Exception:
        pass

    return jsonify({
        "status": "ok",
        "added": lower,
        "watchlist": telemetry_state["admin_watchlist"],
    })


@app.route("/api/admin/remove", methods=["GET", "POST"])
def api_admin_remove():
    name = request.args.get("name", "").strip()
    if request.is_json:
        body = request.get_json(silent=True) or {}
        name = str(body.get("name", name)).strip()

    if not name:
        return jsonify({"status": "error", "message": "Missing admin identifier"}), 400

    lower = name.lower()
    with state_lock:
        telemetry_state["admin_watchlist"] = [
            w for w in telemetry_state["admin_watchlist"] if w != lower
        ]
        pending_commands.append({"type": "admin_remove", "name": lower})

    try:
        requests.get(f"{CPP_LOCAL_URL}/api/admin/remove", params={"name": lower}, timeout=0.25)
    except Exception:
        pass

    return jsonify({
        "status": "ok",
        "removed": lower,
        "watchlist": telemetry_state["admin_watchlist"],
    })


# =============================================================================
# Bidirectional Cloud Sync Endpoint for p4m.exe (Render Deployment Bridge)
# =============================================================================
@app.route("/api/sync", methods=["POST"])
def api_sync():
    """Receives live telemetry & config from p4m.exe (or bridge.py) and returns
    any queued remote commands (teleports, config toggles, admin watchlist)."""
    payload = request.get_json(silent=True) or {}

    with state_lock:
        if "blips" in payload and isinstance(payload["blips"], dict):
            telemetry_state["blips"] = payload["blips"]
        elif "local" in payload:
            telemetry_state["blips"] = {
                "local": payload.get("local"),
                "players": payload.get("players", []),
                "vehicles": payload.get("vehicles", []),
                "map_blips": payload.get("map_blips", []),
            }

        if "config" in payload and isinstance(payload["config"], dict):
            has_pending_cfg = any(
                cmd.get("type") == "set_config" for cmd in pending_commands
            )
            if not has_pending_cfg:
                telemetry_state["config"].update(payload["config"])

        telemetry_state["last_sync_ts"] = time.time()
        telemetry_state["source"] = "cpp_push"

        commands_to_execute = list(pending_commands)
        pending_commands.clear()

    return jsonify({
        "status": "ok",
        "commands": commands_to_execute,
    })


if __name__ == "__main__":
    port = int(os.environ.get("PORT", 5000))
    print(f"[p4m GTA Web Radar] Listening on http://0.0.0.0:{port}")
    app.run(host="0.0.0.0", port=port, debug=False, threaded=True)
