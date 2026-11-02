"""
bridge.py — Bidirectional Telemetry & Command Bridge between local p4m.exe
(http://127.0.0.1:8080) and a remote Render-hosted Web Radar instance.

Usage:
    python bridge.py https://your-p4m-radar.onrender.com
"""
import sys
import time
import requests

LOCAL_CPP = "http://127.0.0.1:8080"
REMOTE_RENDER = sys.argv[1].rstrip("/") if len(sys.argv) > 1 else "http://127.0.0.1:5000"


def run_bridge():
    print(f"[p4m Bridge] Forwarding {LOCAL_CPP} <-> {REMOTE_RENDER}/api/sync")
    session = requests.Session()
    while True:
        try:
            r_blips = session.get(f"{LOCAL_CPP}/api/blips", timeout=0.4)
            r_cfg = session.get(f"{LOCAL_CPP}/api/config", timeout=0.4)
            if r_blips.status_code == 200 and r_cfg.status_code == 200:
                payload = {
                    "blips": r_blips.json(),
                    "config": r_cfg.json(),
                }
                sync_resp = session.post(f"{REMOTE_RENDER}/api/sync", json=payload, timeout=1.5)
                if sync_resp.status_code == 200:
                    commands = sync_resp.json().get("commands", [])
                    for cmd in commands:
                        ctype = cmd.get("type")
                        if ctype == "set_config":
                            session.get(f"{LOCAL_CPP}/api/set", params=cmd.get("params", {}), timeout=0.4)
                        elif ctype == "teleport":
                            session.get(
                                f"{LOCAL_CPP}/api/teleport",
                                params={"x": cmd.get("x"), "y": cmd.get("y"), "z": cmd.get("z")},
                                timeout=0.4,
                            )
                        elif ctype == "admin_add":
                            session.get(f"{LOCAL_CPP}/api/admin/add", params={"name": cmd.get("name")}, timeout=0.4)
                        elif ctype == "admin_remove":
                            session.get(f"{LOCAL_CPP}/api/admin/remove", params={"name": cmd.get("name")}, timeout=0.4)
        except Exception:
            pass
        time.sleep(0.20)


if __name__ == "__main__":
    run_bridge()
