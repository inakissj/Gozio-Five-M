#pragma once
#include <Core/Web/WebServer.hpp>

namespace Core
{
	namespace Web
	{
		inline std::string cWebServer::GetHtmlPage()
		{
			std::string page;
			page.reserve(32768);
			page += R"WEBAPP(<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>p4m — GTA V Live Map Radar & Controller</title>
<style>
:root {
  --bg-primary: #0a0b10;
  --bg-secondary: #12141e;
  --bg-tertiary: #1b1e2e;
  --accent: #3b82f6;
  --accent-glow: rgba(59, 130, 246, 0.4);
  --admin-color: #ef4444;
  --admin-glow: rgba(239, 68, 68, 0.4);
  --local-color: #f59e0b;
  --player-color: #38bdf8;
  --vehicle-color: #a855f7;
  --success: #10b981;
  --text-main: #f1f5f9;
  --text-dim: #94a3b8;
  --border: #2d3348;
}
* { box-sizing: border-box; margin: 0; padding: 0; font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif; }
body { background: var(--bg-primary); color: var(--text-main); height: 100vh; display: flex; flex-direction: column; overflow: hidden; }

/* Header */
header {
  height: 56px; background: var(--bg-secondary); border-bottom: 1px solid var(--border);
  display: flex; align-items: center; justify-content: space-between; padding: 0 20px;
  box-shadow: 0 4px 12px rgba(0,0,0,0.3); z-index: 100;
}
.brand { display: flex; align-items: center; gap: 12px; font-weight: 700; font-size: 18px; letter-spacing: 0.5px; }
.brand-badge { background: linear-gradient(135deg, #3b82f6, #8b5cf6); padding: 4px 10px; border-radius: 6px; font-size: 12px; text-transform: uppercase; color: #fff; }
.status-pill { display: flex; align-items: center; gap: 8px; background: var(--bg-tertiary); padding: 6px 14px; border-radius: 20px; font-size: 13px; border: 1px solid var(--border); }
.status-dot { width: 8px; height: 8px; border-radius: 50%; background: var(--success); box-shadow: 0 0 8px var(--success); animation: pulse 2s infinite; }
@keyframes pulse { 0%, 100% { opacity: 1; } 50% { opacity: 0.4; } }

/* Tabs */
.nav-tabs { display: flex; gap: 6px; }
.tab-btn { background: transparent; border: none; color: var(--text-dim); padding: 8px 16px; border-radius: 8px; cursor: pointer; font-size: 13px; font-weight: 600; display: flex; align-items: center; gap: 8px; transition: all 0.2s; }
.tab-btn:hover { background: var(--bg-tertiary); color: var(--text-main); }
.tab-btn.active { background: var(--accent); color: #fff; box-shadow: 0 2px 10px var(--accent-glow); }

/* Main Content */
.main-container { flex: 1; display: flex; overflow: hidden; position: relative; }
.tab-content { display: none; width: 100%; height: 100%; }
.tab-content.active { display: flex; }

/* Map View */
#tab-map { position: relative; width: 100%; height: 100%; background: #07090e; }
#map-canvas { width: 100%; height: 100%; display: block; cursor: crosshair; }

/* Map HUD Overlays */
.map-hud { position: absolute; top: 16px; left: 16px; z-index: 10; display: flex; flex-direction: column; gap: 10px; pointer-events: none; }
.hud-card { background: rgba(18, 20, 30, 0.85); backdrop-filter: blur(10px); border: 1px solid var(--border); border-radius: 10px; padding: 12px 16px; pointer-events: auto; box-shadow: 0 4px 16px rgba(0,0,0,0.4); }
.hud-title { font-size: 11px; text-transform: uppercase; letter-spacing: 0.8px; color: var(--text-dim); margin-bottom: 6px; font-weight: 700; }
.coord-grid { display: grid; grid-template-columns: repeat(3, 1fr); gap: 8px; font-size: 13px; font-family: monospace; }
.coord-val { color: #38bdf8; font-weight: 600; }

.map-controls { position: absolute; top: 16px; right: 16px; z-index: 10; display: flex; flex-direction: column; gap: 8px; }
.map-btn { background: rgba(18, 20, 30, 0.9); backdrop-filter: blur(8px); border: 1px solid var(--border); color: var(--text-main); width: 38px; height: 38px; border-radius: 8px; cursor: pointer; display: flex; align-items: center; justify-content: center; font-size: 18px; font-weight: bold; transition: all 0.2s; box-shadow: 0 2px 8px rgba(0,0,0,0.3); }
.map-btn:hover { background: var(--accent); color: #fff; }

.map-filters { position: absolute; bottom: 20px; left: 16px; z-index: 10; display: flex; gap: 8px; }
.filter-chip { background: rgba(18, 20, 30, 0.85); backdrop-filter: blur(8px); border: 1px solid var(--border); padding: 6px 12px; border-radius: 20px; font-size: 12px; cursor: pointer; display: flex; align-items: center; gap: 6px; transition: all 0.2s; user-select: none; }
.filter-chip.active { border-color: var(--accent); background: rgba(59, 130, 246, 0.2); }
.filter-dot { width: 8px; height: 8px; border-radius: 50%; }

/* Waypoint Teleport Card */
.waypoint-card { position: absolute; bottom: 20px; right: 16px; z-index: 10; display: none; flex-direction: column; gap: 8px; width: 260px; }
.tp-btn { background: linear-gradient(135deg, #10b981, #059669); color: white; border: none; padding: 10px; border-radius: 8px; font-weight: 700; font-size: 13px; cursor: pointer; box-shadow: 0 4px 12px rgba(16, 185, 129, 0.4); transition: all 0.2s; }
.tp-btn:hover { transform: translateY(-1px); filter: brightness(1.1); }

/* Admin Alert Banner */
.admin-banner { position: absolute; top: 16px; left: 50%; transform: translateX(-50%); z-index: 20; background: linear-gradient(135deg, #dc2626, #991b1b); color: #fff; padding: 10px 20px; border-radius: 30px; font-size: 13px; font-weight: 700; display: none; align-items: center; gap: 10px; box-shadow: 0 0 20px rgba(220, 38, 38, 0.6); animation: banner-glow 1.5s infinite alternate; }
@keyframes banner-glow { 0% { box-shadow: 0 0 15px rgba(220, 38, 38, 0.5); } 100% { box-shadow: 0 0 30px rgba(220, 38, 38, 0.9); } }

/* Config View */
#tab-config { overflow-y: auto; padding: 24px; width: 100%; display: none; flex-direction: column; gap: 24px; }
.config-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(320px, 1fr)); gap: 20px; max-width: 1400px; margin: 0 auto; width: 100%; }
.config-card { background: var(--bg-secondary); border: 1px solid var(--border); border-radius: 12px; padding: 20px; box-shadow: 0 4px 16px rgba(0,0,0,0.25); display: flex; flex-direction: column; gap: 16px; }
.card-header { display: flex; align-items: center; justify-content: space-between; border-bottom: 1px solid var(--border); padding-bottom: 12px; font-weight: 700; font-size: 16px; }
.control-row { display: flex; align-items: center; justify-content: space-between; gap: 12px; font-size: 13px; }
.control-label { color: var(--text-main); font-weight: 500; }
.switch { position: relative; display: inline-block; width: 44px; height: 24px; flex-shrink: 0; }
.switch input { opacity: 0; width: 0; height: 0; }
.slider-toggle { position: absolute; cursor: pointer; top: 0; left: 0; right: 0; bottom: 0; background-color: #334155; transition: .3s; border-radius: 24px; }
.slider-toggle:before { position: absolute; content: ""; height: 18px; width: 18px; left: 3px; bottom: 3px; background-color: white; transition: .3s; border-radius: 50%; }
input:checked + .slider-toggle { background-color: var(--accent); }
input:checked + .slider-toggle:before { transform: translateX(20px); }
.range-container { display: flex; flex-direction: column; gap: 6px; width: 100%; }
.range-header { display: flex; justify-content: space-between; font-size: 12px; color: var(--text-dim); }
input[type=range] { width: 100%; accent-color: var(--accent); cursor: pointer; }

/* Admin View */
#tab-admins, #tab-players { overflow-y: auto; padding: 24px; width: 100%; display: none; flex-direction: column; gap: 20px; max-width: 1400px; margin: 0 auto; }
.data-table { width: 100%; border-collapse: collapse; background: var(--bg-secondary); border-radius: 12px; overflow: hidden; border: 1px solid var(--border); }
.data-table th, .data-table td { padding: 12px 16px; text-align: left; font-size: 13px; border-bottom: 1px solid var(--border); }
.data-table th { background: var(--bg-tertiary); font-weight: 700; color: var(--text-dim); text-transform: uppercase; font-size: 11px; letter-spacing: 0.5px; }
.admin-badge { background: rgba(239, 68, 68, 0.2); border: 1px solid var(--admin-color); color: var(--admin-color); padding: 3px 8px; border-radius: 12px; font-weight: 700; font-size: 11px; display: inline-block; }
.table-btn { background: var(--accent); border: none; color: white; padding: 6px 12px; border-radius: 6px; cursor: pointer; font-size: 12px; font-weight: 600; }
.table-btn:hover { opacity: 0.9; }
.input-row { display: flex; gap: 10px; margin-bottom: 10px; }
.text-input { background: var(--bg-tertiary); border: 1px solid var(--border); border-radius: 8px; padding: 8px 14px; color: white; font-size: 13px; flex: 1; }
</style>
</head>
<body>
)WEBAPP";
			page += R"WEBAPP(
<header>
  <div class="brand">
    <span class="brand-badge">FiveM</span>
    <span>p4m External Controller</span>
  </div>

  <nav class="nav-tabs">
    <button class="tab-btn active" onclick="switchTab('map')">🗺️ GTA Map Radar</button>
    <button class="tab-btn" onclick="switchTab('config')">⚙️ Cheat Config</button>
    <button class="tab-btn" onclick="switchTab('admins')">🛡️ Admin Watch</button>
    <button class="tab-btn" onclick="switchTab('players')">👥 Entity List</button>
  </nav>

  <div class="status-pill">
    <div class="status-dot"></div>
    <span id="header-status">Live Connected</span>
  </div>
</header>

<div class="main-container">

  <!-- ADMIN WARNING BANNER -->
  <div class="admin-banner" id="admin-banner">
    <span>⚠️</span>
    <span id="admin-banner-text">ADMIN DETECTED IN AREA</span>
  </div>

  <!-- TAB 1: GTA MAP RADAR -->
  <div id="tab-map" class="tab-content active">
    <canvas id="map-canvas"></canvas>

    <!-- Map HUD: Local Info -->
    <div class="map-hud">
      <div class="hud-card">
        <div class="hud-title">Local Player Telemetry</div>
        <div class="coord-grid">
          <div>X: <span id="hud-x" class="coord-val">0.0</span></div>
          <div>Y: <span id="hud-y" class="coord-val">0.0</span></div>
          <div>Z: <span id="hud-z" class="coord-val">0.0</span></div>
        </div>
        <div style="margin-top: 8px; font-size: 12px; color: var(--text-dim); display: flex; justify-content: space-between;">
          <span>HP: <b id="hud-hp" style="color: #10b981;">100%</b></span>
          <span>Armor: <b id="hud-arm" style="color: #38bdf8;">100%</b></span>
          <span>Admins Nearby: <b id="hud-admins" style="color: #ef4444;">0</b></span>
        </div>
      </div>
    </div>

    <!-- Map Controls -->
    <div class="map-controls">
      <button class="map-btn" title="Zoom In" onclick="zoomMap(1.25)">+</button>
      <button class="map-btn" title="Zoom Out" onclick="zoomMap(0.8)">-</button>
      <button class="map-btn" title="Center on Me" onclick="recenterMap()">🎯</button>
    </div>

    <!-- Map Filter Chips -->
    <div class="map-filters">
      <div class="filter-chip active" id="f-admins" onclick="toggleFilter('admins')">
        <span class="filter-dot" style="background: var(--admin-color);"></span>
        <span>Admins (<b id="cnt-admins">0</b>)</span>
      </div>
      <div class="filter-chip active" id="f-players" onclick="toggleFilter('players')">
        <span class="filter-dot" style="background: var(--player-color);"></span>
        <span>Players (<b id="cnt-players">0</b>)</span>
      </div>
      <div class="filter-chip active" id="f-vehs" onclick="toggleFilter('vehs')">
        <span class="filter-dot" style="background: var(--vehicle-color);"></span>
        <span>Vehicles (<b id="cnt-vehs">0</b>)</span>
      </div>
      <div class="filter-chip active" id="f-follow" onclick="toggleFollow()">
        <span>🔒 Follow Player: <b id="lbl-follow">ON</b></span>
      </div>
    </div>

    <!-- Waypoint Teleport Card -->
    <div class="hud-card waypoint-card" id="wp-card">
      <div class="hud-title">Target Waypoint</div>
      <div style="font-size: 12px; color: var(--text-dim); margin-bottom: 8px;">
        Coords: <span id="wp-coords" style="color: #10b981; font-family: monospace;">X: 0, Y: 0</span>
      </div>
      <button class="tp-btn" onclick="teleportToWaypoint()">⚡ Teleport Here</button>
    </div>
  </div>

  <!-- TAB 2: CONFIGURATION -->
  <div id="tab-config" class="tab-content">
    <div class="config-grid">
      <!-- Visuals / ESP Card -->
      <div class="config-card">
        <div class="card-header">
          <span>👁️ Visuals & ESP</span>
        </div>
        <div class="control-row">
          <span class="control-label">Master ESP</span>
          <label class="switch"><input type="checkbox" id="cfg-esp_enabled" onchange="updateCfg('esp_enabled', this.checked)"><span class="slider-toggle"></span></label>
        </div>
        <div class="control-row" style="background: rgba(239,68,68,0.1); padding: 8px 12px; border-radius: 8px; border: 1px solid rgba(239,68,68,0.3);">
          <span class="control-label" style="color: #f87171; font-weight: 700;">🛡️ Admin ESP (Detect & Highlight)</span>
          <label class="switch"><input type="checkbox" id="cfg-esp_admin" onchange="updateCfg('esp_admin', this.checked)"><span class="slider-toggle"></span></label>
        </div>
        <div class="control-row" style="background: rgba(168,85,247,0.1); padding: 8px 12px; border-radius: 8px; border: 1px solid rgba(168,85,247,0.3);">
          <span class="control-label" style="color: #c084fc; font-weight: 700;">🌈 Admin Multi-Color / Rainbow</span>
          <label class="switch"><input type="checkbox" id="cfg-esp_admin_rainbow" onchange="updateCfg('esp_admin_rainbow', this.checked)"><span class="slider-toggle"></span></label>
        </div>
        <div class="control-row">
          <span class="control-label">Box ESP</span>
          <label class="switch"><input type="checkbox" id="cfg-esp_box" onchange="updateCfg('esp_box', this.checked)"><span class="slider-toggle"></span></label>
        </div>
        <div class="control-row">
          <span class="control-label">Skeleton ESP</span>
          <label class="switch"><input type="checkbox" id="cfg-esp_skeleton" onchange="updateCfg('esp_skeleton', this.checked)"><span class="slider-toggle"></span></label>
        </div>
        <div class="control-row">
          <span class="control-label">SnapLines</span>
          <label class="switch"><input type="checkbox" id="cfg-esp_snaplines" onchange="updateCfg('esp_snaplines', this.checked)"><span class="slider-toggle"></span></label>
        </div>
        <div class="control-row">
          <span class="control-label">Player Usernames</span>
          <label class="switch"><input type="checkbox" id="cfg-esp_names" onchange="updateCfg('esp_names', this.checked)"><span class="slider-toggle"></span></label>
        </div>
        <div class="control-row">
          <span class="control-label">Health & Armor Bars</span>
          <label class="switch"><input type="checkbox" id="cfg-esp_health" onchange="updateCfg('esp_health', this.checked); updateCfg('esp_armor', this.checked);"><span class="slider-toggle"></span></label>
        </div>
        <div class="control-row">
          <span class="control-label">Weapon Name Tag</span>
          <label class="switch"><input type="checkbox" id="cfg-esp_weapon" onchange="updateCfg('esp_weapon', this.checked)"><span class="slider-toggle"></span></label>
        </div>
        <div class="control-row">
          <span class="control-label">Head Circle</span>
          <label class="switch"><input type="checkbox" id="cfg-esp_headcircle" onchange="updateCfg('esp_headcircle', this.checked)"><span class="slider-toggle"></span></label>
        </div>
        <div class="control-row">
          <span class="control-label">Distance Tag</span>
          <label class="switch"><input type="checkbox" id="cfg-esp_distance" onchange="updateCfg('esp_distance', this.checked)"><span class="slider-toggle"></span></label>
        </div>
        <div class="control-row">
          <span class="control-label">Vehicle ESP</span>
          <label class="switch"><input type="checkbox" id="cfg-vehesp_enabled" onchange="updateCfg('vehesp_enabled', this.checked); updateCfg('vehesp_names', this.checked); updateCfg('vehesp_distance', this.checked);"><span class="slider-toggle"></span></label>
        </div>
        <div class="range-container">
          <div class="range-header"><span>ESP Max Distance</span><b id="val-esp_max_distance">200m</b></div>
          <input type="range" min="50" max="1000" id="cfg-esp_max_distance" onchange="updateCfg('esp_max_distance', this.value); document.getElementById('val-esp_max_distance').innerText = this.value + 'm';">
        </div>
      </div>
)WEBAPP";
			page += R"WEBAPP(
      <!-- Combat / Aimbot Card -->
      <div class="config-card">
        <div class="card-header">
          <span>🎯 Combat & Aimbot</span>
        </div>
        <div class="control-row">
          <span class="control-label">Aimbot Enable</span>
          <label class="switch"><input type="checkbox" id="cfg-aimbot_enabled" onchange="updateCfg('aimbot_enabled', this.checked)"><span class="slider-toggle"></span></label>
        </div>
        <div class="range-container">
          <div class="range-header"><span>Aimbot FOV</span><b id="val-aimbot_fov">180°</b></div>
          <input type="range" min="10" max="360" id="cfg-aimbot_fov" onchange="updateCfg('aimbot_fov', this.value); document.getElementById('val-aimbot_fov').innerText = this.value + '°';">
        </div>
        <div class="range-container">
          <div class="range-header"><span>Aimbot Speed</span><b id="val-aimbot_speed">12</b></div>
          <input type="range" min="1" max="50" id="cfg-aimbot_speed" onchange="updateCfg('aimbot_speed', this.value); document.getElementById('val-aimbot_speed').innerText = this.value;">
        </div>
        <div class="control-row">
          <span class="control-label">Silent Aim</span>
          <label class="switch"><input type="checkbox" id="cfg-silentaim_enabled" onchange="updateCfg('silentaim_enabled', this.checked)"><span class="slider-toggle"></span></label>
        </div>
        <div class="control-row">
          <span class="control-label">Triggerbot</span>
          <label class="switch"><input type="checkbox" id="cfg-triggerbot_enabled" onchange="updateCfg('triggerbot_enabled', this.checked)"><span class="slider-toggle"></span></label>
        </div>
      </div>

      <!-- Player Exploits Card -->
      <div class="config-card">
        <div class="card-header">
          <span>⚡ Player Exploits</span>
        </div>
        <div class="control-row" style="background: rgba(16,185,129,0.1); padding: 8px 12px; border-radius: 8px; border: 1px solid rgba(16,185,129,0.3);">
          <span class="control-label" style="color: #34d399; font-weight: 700;">GodMode (Invincibility)</span>
          <label class="switch"><input type="checkbox" id="cfg-godmode" onchange="updateCfg('godmode', this.checked)"><span class="slider-toggle"></span></label>
        </div>
        <div class="control-row">
          <span class="control-label">NoClip (Fly Mode)</span>
          <label class="switch"><input type="checkbox" id="cfg-noclip" onchange="updateCfg('noclip', this.checked)"><span class="slider-toggle"></span></label>
        </div>
        <div class="control-row">
          <span class="control-label">Fast Run</span>
          <label class="switch"><input type="checkbox" id="cfg-fastrun" onchange="updateCfg('fastrun', this.checked)"><span class="slider-toggle"></span></label>
        </div>
        <div class="control-row">
          <span class="control-label">Infinite Stamina</span>
          <label class="switch"><input type="checkbox" id="cfg-stamina" onchange="updateCfg('stamina', this.checked)"><span class="slider-toggle"></span></label>
        </div>
        <div class="control-row">
          <span class="control-label">Infinite Ammo</span>
          <label class="switch"><input type="checkbox" id="cfg-ammo" onchange="updateCfg('ammo', this.checked)"><span class="slider-toggle"></span></label>
        </div>
        <div class="control-row">
          <span class="control-label">No Recoil & No Spread</span>
          <label class="switch"><input type="checkbox" id="cfg-norecoil" onchange="updateCfg('norecoil', this.checked); updateCfg('nospread', this.checked);"><span class="slider-toggle"></span></label>
        </div>
      </div>
    </div>
  </div>

  <!-- TAB 3: ADMIN WATCH -->
  <div id="tab-admins" class="tab-content">
    <div class="hud-card" style="margin-bottom: 12px;">
      <div class="hud-title">Add Custom Admin Tag / Name</div>
      <div class="input-row">
        <input type="text" id="admin-name-input" class="text-input" placeholder="Enter Admin Name or Identifier (e.g. StaffJohn, Mod_Mike)...">
        <button class="table-btn" onclick="addAdminName()">+ Add Admin</button>
      </div>
    </div>
    <table class="data-table">
      <thead>
        <tr>
          <th>Status</th>
          <th>Player Name</th>
          <th>ID</th>
          <th>Distance</th>
          <th>Health</th>
          <th>Actions</th>
        </tr>
      </thead>
      <tbody id="admin-table-body">
        <tr><td colspan="6" style="text-align: center; color: var(--text-dim);">No admins currently nearby.</td></tr>
      </tbody>
    </table>
  </div>

  <!-- TAB 4: ENTITY LIST -->
  <div id="tab-players" class="tab-content">
    <table class="data-table">
      <thead>
        <tr>
          <th>Type</th>
          <th>Name</th>
          <th>ID</th>
          <th>Distance</th>
          <th>Health</th>
          <th>Armor</th>
          <th>Weapon</th>
          <th>Action</th>
        </tr>
      </thead>
      <tbody id="players-table-body">
        <tr><td colspan="8" style="text-align: center; color: var(--text-dim);">Scanning for players...</td></tr>
      </tbody>
    </table>
  </div>

</div>

<script>
// State
let activeTab = 'map';
let localData = null;
let playersData = [];
let vehiclesData = [];
let mapBlipsData = [];
let followPlayer = true;
let targetWaypoint = null;

let filterAdmins = true;
let filterPlayers = true;
let filterVehs = true;

// Map Navigation State
let scale = 1.0;
let panX = 0;
let panY = 0;
let isDragging = false;
let startX, startY;

const canvas = document.getElementById('map-canvas');
const ctx = canvas.getContext('2d');
)WEBAPP";
			page += R"WEBAPP(
function resizeCanvas() {
  canvas.width = canvas.parentElement.clientWidth;
  canvas.height = canvas.parentElement.clientHeight;
}
window.addEventListener('resize', resizeCanvas);
resizeCanvas();

// GTA V World Bounds
const GTA_X_MIN = -4000;
const GTA_X_MAX = 4500;
const GTA_Y_MIN = -4000;
const GTA_Y_MAX = 8000;
const GTA_WIDTH = GTA_X_MAX - GTA_X_MIN;
const GTA_HEIGHT = GTA_Y_MAX - GTA_Y_MIN;

function worldToCanvas(wx, wy) {
  // Normalize (0..1)
  const nx = (wx - GTA_X_MIN) / GTA_WIDTH;
  const ny = 1.0 - ((wy - GTA_Y_MIN) / GTA_HEIGHT);
  
  // Base map dimensions (virtual size 2400 x 3200)
  const baseW = 2400;
  const baseH = 3200;
  
  const screenX = (nx * baseW * scale) + panX + (canvas.width / 2);
  const screenY = (ny * baseH * scale) + panY + (canvas.height / 2);
  return { x: screenX, y: screenY };
}

function canvasToWorld(cx, cy) {
  const baseW = 2400;
  const baseH = 3200;
  const nx = (cx - panX - (canvas.width / 2)) / (baseW * scale);
  const ny = (cy - panY - (canvas.height / 2)) / (baseH * scale);
  
  const wx = GTA_X_MIN + (nx * GTA_WIDTH);
  const wy = GTA_Y_MIN + ((1.0 - ny) * GTA_HEIGHT);
  return { x: wx, y: wy };
}

// Draw stylized GTA V topography radar map
function drawMap() {
  ctx.clearRect(0, 0, canvas.width, canvas.height);
  
  // Ocean Background
  ctx.fillStyle = '#060a14';
  ctx.fillRect(0, 0, canvas.width, canvas.height);

  // Virtual base center
  const origin = worldToCanvas(0, 0);

  // Draw Grid Lines
  ctx.strokeStyle = '#141a29';
  ctx.lineWidth = 1;
  const step = 500;
  for (let x = GTA_X_MIN; x <= GTA_X_MAX; x += step) {
    const p1 = worldToCanvas(x, GTA_Y_MIN);
    const p2 = worldToCanvas(x, GTA_Y_MAX);
    ctx.beginPath();
    ctx.moveTo(p1.x, p1.y);
    ctx.lineTo(p2.x, p2.y);
    ctx.stroke();
  }
  for (let y = GTA_Y_MIN; y <= GTA_Y_MAX; y += step) {
    const p1 = worldToCanvas(GTA_X_MIN, y);
    const p2 = worldToCanvas(GTA_X_MAX, y);
    ctx.beginPath();
    ctx.moveTo(p1.x, p1.y);
    ctx.lineTo(p2.x, p2.y);
    ctx.stroke();
  }

  // Draw San Andreas Landmass Outline (Stylized Vector Contour)
  ctx.save();
  ctx.fillStyle = '#101626';
  ctx.strokeStyle = '#22304d';
  ctx.lineWidth = 2 * scale;
  
  // Island shape path
  const islandPoints = [
    [-3000, -2500], [-3500, -1000], [-3200, 1500], [-2500, 3500], [-1800, 5000],
    [-800, 6500], [400, 7500], [1800, 7000], [3200, 5000], [3600, 2500],
    [3200, 500], [2500, -1500], [2000, -3200], [500, -3800], [-1200, -3600],
    [-2200, -3000]
  ];
  ctx.beginPath();
  islandPoints.forEach((pt, i) => {
    const cp = worldToCanvas(pt[0], pt[1]);
    if (i === 0) ctx.moveTo(cp.x, cp.y);
    else ctx.lineTo(cp.x, cp.y);
  });
  ctx.closePath();
  ctx.fill();
  ctx.stroke();

  // Alamo Sea (Lake)
  ctx.fillStyle = '#060a14';
  ctx.beginPath();
  const lakePoints = [
    [500, 3500], [1800, 3800], [2200, 4200], [1500, 4500], [600, 4300], [200, 3800]
  ];
  lakePoints.forEach((pt, i) => {
    const cp = worldToCanvas(pt[0], pt[1]);
    if (i === 0) ctx.moveTo(cp.x, cp.y);
    else ctx.lineTo(cp.x, cp.y);
  });
  ctx.closePath();
  ctx.fill();
  ctx.stroke();

  // Major Highways & Roads
  ctx.strokeStyle = '#253555';
  ctx.lineWidth = 2 * scale;
  ctx.beginPath();
  // West Coast Highway
  let p = worldToCanvas(-2800, -2000); ctx.moveTo(p.x, p.y);
  p = worldToCanvas(-3000, 1000); ctx.lineTo(p.x, p.y);
  p = worldToCanvas(-2000, 4000); ctx.lineTo(p.x, p.y);
  p = worldToCanvas(0, 6800); ctx.lineTo(p.x, p.y);
  // East Highway
  p = worldToCanvas(1500, -3000); ctx.moveTo(p.x, p.y);
  p = worldToCanvas(2500, 1000); ctx.lineTo(p.x, p.y);
  p = worldToCanvas(2800, 4500); ctx.lineTo(p.x, p.y);
  p = worldToCanvas(500, 7200); ctx.lineTo(p.x, p.y);
  ctx.stroke();

  // Los Santos Urban Area Indicator
  ctx.fillStyle = 'rgba(40, 55, 90, 0.4)';
  const lsPt = worldToCanvas(0, -1500);
  ctx.beginPath();
  ctx.arc(lsPt.x, lsPt.y, 220 * scale, 0, Math.PI * 2);
  ctx.fill();

  // Labels for major zones
  ctx.fillStyle = '#475569';
  ctx.font = `${Math.max(10, 13 * scale)}px sans-serif`;
  ctx.textAlign = 'center';
  const labelPos = (text, wx, wy) => {
    const cp = worldToCanvas(wx, wy);
    ctx.fillText(text, cp.x, cp.y);
  };
  labelPos("LOS SANTOS", 0, -1400);
  labelPos("LSIA AIRPORT", -1100, -3000);
  labelPos("SANDY SHORES", 1600, 3600);
  labelPos("MT CHILIAD", 400, 5600);
  labelPos("PALETO BAY", -200, 6400);
  labelPos("FORT ZANCUDO", -2200, 3100);

  ctx.restore();

  // Draw Native GTA Map Blips (Waypoint / Objectives)
  if (mapBlipsData && mapBlipsData.length > 0) {
    mapBlipsData.forEach(b => {
      const pos = worldToCanvas(b.x, b.y);
      const isWaypoint = (b.sprite === 8);
      ctx.fillStyle = isWaypoint ? '#c084fc' : '#64748b';
      ctx.beginPath();
      ctx.arc(pos.x, pos.y, isWaypoint ? 6 : 3, 0, Math.PI * 2);
      ctx.fill();
      if (isWaypoint) {
        ctx.strokeStyle = '#e879f9';
        ctx.lineWidth = 2;
        ctx.stroke();
        ctx.fillStyle = '#f0abfc';
        ctx.font = 'bold 10px sans-serif';
        ctx.fillText('IN-GAME WAYPOINT', pos.x, pos.y - 10);
      }
    });
  }

  // Draw Vehicles
  if (filterVehs && vehiclesData) {
    vehiclesData.forEach(v => {
      const pos = worldToCanvas(v.x, v.y);
      ctx.fillStyle = '#a855f7';
      ctx.beginPath();
      ctx.arc(pos.x, pos.y, 4, 0, Math.PI * 2);
      ctx.fill();

      ctx.fillStyle = '#c084fc';
      ctx.font = '10px sans-serif';
      ctx.fillText(v.name, pos.x, pos.y - 6);
    });
  }

  // Draw Players & Admins
)WEBAPP";
			page += R"WEBAPP(
  let adminCount = 0;
  if (playersData) {
    playersData.forEach(p => {
      if (p.is_admin) adminCount++;
      if (p.is_admin && !filterAdmins) return;
      if (!p.is_admin && !filterPlayers) return;

      const pos = worldToCanvas(p.x, p.y);

      if (p.is_admin) {
        // ADMIN BLIP: Flashing Red / Rainbow with Warning Halo
        const time = Date.now() / 300;
        const pulseR = 8 + (Math.sin(time) * 4);

        // Warning Pulse Ring
        ctx.strokeStyle = 'rgba(239, 68, 68, 0.5)';
        ctx.lineWidth = 2;
        ctx.beginPath();
        ctx.arc(pos.x, pos.y, pulseR + 6, 0, Math.PI * 2);
        ctx.stroke();

        // Inner Admin Blip
        ctx.fillStyle = '#ef4444';
        ctx.beginPath();
        ctx.arc(pos.x, pos.y, 7, 0, Math.PI * 2);
        ctx.fill();

        // Crown / Admin Label
        ctx.fillStyle = '#fca5a5';
        ctx.font = 'bold 11px sans-serif';
        ctx.fillText(`[ADMIN] ${p.name}`, pos.x, pos.y - 12);

        // Health bar
        ctx.fillStyle = '#334155';
        ctx.fillRect(pos.x - 14, pos.y + 10, 28, 4);
        ctx.fillStyle = '#ef4444';
        ctx.fillRect(pos.x - 14, pos.y + 10, Math.max(0, Math.min(28, (p.health / 200) * 28)), 4);
      } else {
        // Normal Player Blip
        ctx.fillStyle = '#38bdf8';
        ctx.beginPath();
        ctx.arc(pos.x, pos.y, 5, 0, Math.PI * 2);
        ctx.fill();

        ctx.fillStyle = '#e2e8f0';
        ctx.font = '10px sans-serif';
        ctx.fillText(p.name, pos.x, pos.y - 8);
      }
    });
  }

  // Draw Waypoint Marker
  if (targetWaypoint) {
    const wp = worldToCanvas(targetWaypoint.x, targetWaypoint.y);
    ctx.strokeStyle = '#10b981';
    ctx.lineWidth = 2;
    ctx.beginPath();
    ctx.arc(wp.x, wp.y, 10, 0, Math.PI * 2);
    ctx.stroke();

    ctx.fillStyle = '#10b981';
    ctx.beginPath();
    ctx.arc(wp.x, wp.y, 4, 0, Math.PI * 2);
    ctx.fill();

    ctx.fillText("TARGET WAYPOINT", wp.x, wp.y - 14);
  }

  // Draw Local Player Blip (Self)
  if (localData) {
    const myPos = worldToCanvas(localData.x, localData.y);

    // Pulse Ring
    const t = (Date.now() % 2000) / 2000;
    ctx.strokeStyle = `rgba(245, 158, 11, ${1 - t})`;
    ctx.lineWidth = 2;
    ctx.beginPath();
    ctx.arc(myPos.x, myPos.y, 6 + (t * 24), 0, Math.PI * 2);
    ctx.stroke();

    // Local Player Core
    ctx.fillStyle = '#f59e0b';
    ctx.beginPath();
    ctx.arc(myPos.x, myPos.y, 8, 0, Math.PI * 2);
    ctx.fill();

    ctx.fillStyle = '#ffffff';
    ctx.beginPath();
    ctx.arc(myPos.x, myPos.y, 3, 0, Math.PI * 2);
    ctx.fill();

    ctx.fillStyle = '#fbbf24';
    ctx.font = 'bold 12px sans-serif';
    ctx.fillText("YOU", myPos.x, myPos.y - 12);
  }

  // Update Admin Banner
  const banner = document.getElementById('admin-banner');
  if (adminCount > 0) {
    banner.style.display = 'flex';
    document.getElementById('admin-banner-text').innerText = `⚠️ ${adminCount} ADMIN(S) DETECTED IN SESSION!`;
  } else {
    banner.style.display = 'none';
  }
}

// Map Interaction Handlers (Pan & Zoom)
canvas.addEventListener('mousedown', e => {
  isDragging = true;
  startX = e.clientX - panX;
  startY = e.clientY - panY;
});

window.addEventListener('mousemove', e => {
  if (!isDragging) return;
  panX = e.clientX - startX;
  panY = e.clientY - startY;
  followPlayer = false;
  document.getElementById('lbl-follow').innerText = 'OFF';
  drawMap();
});

window.addEventListener('mouseup', () => { isDragging = false; });

canvas.addEventListener('wheel', e => {
  e.preventDefault();
  const zoomFactor = e.deltaY < 0 ? 1.15 : 0.87;
  zoomMap(zoomFactor);
});

// Click to set Waypoint
canvas.addEventListener('click', e => {
  if (isDragging) return;
  const rect = canvas.getBoundingClientRect();
  const clickX = e.clientX - rect.left;
  const clickY = e.clientY - rect.top;
  
  const worldPos = canvasToWorld(clickX, clickY);
  targetWaypoint = worldPos;
  
  document.getElementById('wp-card').style.display = 'flex';
  document.getElementById('wp-coords').innerText = `X: ${worldPos.x.toFixed(1)}, Y: ${worldPos.y.toFixed(1)}`;
  drawMap();
});

function zoomMap(factor) {
  scale = Math.max(0.2, Math.min(8.0, scale * factor));
  drawMap();
}
)WEBAPP";
			page += R"WEBAPP(
function recenterMap() {
  if (localData) {
    const baseW = 2400, baseH = 3200;
    const nx = (localData.x - GTA_X_MIN) / GTA_WIDTH;
    const ny = 1.0 - ((localData.y - GTA_Y_MIN) / GTA_HEIGHT);
    panX = -(nx * baseW * scale);
    panY = -(ny * baseH * scale);
  } else {
    panX = 0; panY = 0;
  }
  drawMap();
}

function toggleFollow() {
  followPlayer = !followPlayer;
  document.getElementById('lbl-follow').innerText = followPlayer ? 'ON' : 'OFF';
  if (followPlayer) recenterMap();
}

function toggleFilter(type) {
  if (type === 'admins') { filterAdmins = !filterAdmins; document.getElementById('f-admins').classList.toggle('active', filterAdmins); }
  if (type === 'players') { filterPlayers = !filterPlayers; document.getElementById('f-players').classList.toggle('active', filterPlayers); }
  if (type === 'vehs') { filterVehs = !filterVehs; document.getElementById('f-vehs').classList.toggle('active', filterVehs); }
  drawMap();
}

function teleportToWaypoint() {
  if (!targetWaypoint) return;
  fetch(`/api/teleport?x=${targetWaypoint.x.toFixed(1)}&y=${targetWaypoint.y.toFixed(1)}`)
    .then(r => r.json())
    .then(res => {
      alert(`Teleported to X: ${targetWaypoint.x.toFixed(1)}, Y: ${targetWaypoint.y.toFixed(1)}!`);
    })
    .catch(err => alert("Teleport failed: " + err));
}

function teleportTo(x, y, z) {
  fetch(`/api/teleport?x=${x}&y=${y}&z=${z}`)
    .then(r => r.json())
    .then(() => alert(`Teleported to ${x}, ${y}, ${z}!`));
}

// Navigation Tabs
function switchTab(tab) {
  activeTab = tab;
  document.querySelectorAll('.tab-btn').forEach(btn => btn.classList.remove('active'));
  document.querySelectorAll('.tab-content').forEach(c => c.classList.remove('active'));
  
  event.target.classList.add('active');
  document.getElementById(`tab-${tab}`).classList.add('active');
  
  if (tab === 'map') {
    resizeCanvas();
    drawMap();
  }
}

// Config Synchronization
function updateCfg(key, val) {
  fetch(`/api/set?${key}=${val}`)
    .catch(e => console.error("Update cfg error", e));
}

function fetchConfig() {
  fetch('/api/config')
    .then(r => r.json())
    .then(cfg => {
      for (const [k, v] of Object.entries(cfg)) {
        const el = document.getElementById(`cfg-${k}`);
        if (!el) continue;
        if (el.type === 'checkbox') el.checked = v;
        else {
          el.value = v;
          const valEl = document.getElementById(`val-${k}`);
          if (valEl) valEl.innerText = v;
        }
      }
    })
    .catch(() => {});
}

// Admin Management
function addAdminName() {
  const inp = document.getElementById('admin-name-input');
  const name = inp.value.trim();
  if (!name) return;
  fetch(`/api/admin/add?name=${encodeURIComponent(name)}`)
    .then(r => r.json())
    .then(() => {
      inp.value = '';
      alert(`Admin '${name}' added to watchlist!`);
    });
}

function removeAdminName(name) {
  fetch(`/api/admin/remove?name=${encodeURIComponent(name)}`)
    .then(r => r.json())
    .then(() => alert(`Admin '${name}' removed.`));
}

// Main Polling Loop
function pollTelemetry() {
  fetch('/api/blips')
    .then(r => r.json())
    .then(data => {
      localData = data.local;
      playersData = data.players || [];
      vehiclesData = data.vehicles || [];
      mapBlipsData = data.map_blips || [];

      // Update Counts
      let adminsCount = playersData.filter(p => p.is_admin).length;
      document.getElementById('cnt-admins').innerText = adminsCount;
      document.getElementById('cnt-players').innerText = playersData.length;
      document.getElementById('cnt-vehs').innerText = vehiclesData.length;
      document.getElementById('hud-admins').innerText = adminsCount;

      // Update Local Telemetry HUD
      if (localData) {
        document.getElementById('hud-x').innerText = localData.x.toFixed(1);
        document.getElementById('hud-y').innerText = localData.y.toFixed(1);
        document.getElementById('hud-z').innerText = localData.z.toFixed(1);
        document.getElementById('hud-hp').innerText = Math.round(localData.health) + ' HP';
        document.getElementById('hud-arm').innerText = Math.round(localData.armor) + ' Arm';

        if (followPlayer) {
          recenterMap();
        }
      }

      // Update Tables if in respective tabs
      if (activeTab === 'admins') renderAdminTable();
      if (activeTab === 'players') renderPlayerTable();

      drawMap();
    })
    .catch(() => {
      document.getElementById('header-status').innerText = 'Offline / Waiting for FiveM';
    });
}

function renderAdminTable() {
  const tbody = document.getElementById('admin-table-body');
  const admins = playersData.filter(p => p.is_admin);
  if (admins.length === 0) {
    tbody.innerHTML = '<tr><td colspan="6" style="text-align: center; color: var(--text-dim);">No admins currently nearby.</td></tr>';
    return;
  }
  tbody.innerHTML = admins.map(a => `
    <tr>
      <td><span class="admin-badge">ADMIN</span></td>
      <td><b>${a.name}</b></td>
      <td>${a.id}</td>
      <td>${Math.round(a.dist)}m</td>
      <td>${Math.round(a.health)} HP</td>
      <td><button class="table-btn" onclick="teleportTo(${a.x}, ${a.y}, ${a.z})">Teleport To</button></td>
    </tr>
  `).join('');
}

function renderPlayerTable() {
  const tbody = document.getElementById('players-table-body');
  if (playersData.length === 0) {
    tbody.innerHTML = '<tr><td colspan="8" style="text-align: center; color: var(--text-dim);">No players in range.</td></tr>';
    return;
  }
  tbody.innerHTML = playersData.map(p => `
    <tr>
      <td>${p.is_admin ? '<span class="admin-badge">ADMIN</span>' : '<span style="color:#38bdf8;">Player</span>'}</td>
      <td><b>${p.name}</b></td>
      <td>${p.id}</td>
      <td>${Math.round(p.dist)}m</td>
      <td>${Math.round(p.health)}</td>
      <td>${Math.round(p.armor)}</td>
      <td>${p.weapon || 'Unarmed'}</td>
      <td><button class="table-btn" onclick="teleportTo(${p.x}, ${p.y}, ${p.z})">Teleport To</button></td>
    </tr>
  `).join('');
}

// Start
fetchConfig();
setInterval(pollTelemetry, 150);
drawMap();
</script>
</body>
</html>
)WEBAPP";
			return page;
		}
	}
}
