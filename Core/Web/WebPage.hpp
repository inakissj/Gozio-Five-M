#pragma once
#include <Core/Web/WebServer.hpp>

namespace Core
{
	namespace Web
	{
		inline std::string cWebServer::GetHtmlPage()
		{
			std::string page;
			page.reserve(65536);
			page += R"WEBAPP(<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1.0">
<title>p4m</title>
<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/leaflet/1.9.4/leaflet.min.css"/>
<style>
:root{--bg:#0b0c10;--bg2:#101217;--bg3:#161920;--bg4:#1c1f28;--bg5:#22252f;--text:#e2e4e9;--white:#fff;--dim:#6b7080;--dim2:#484c58;--border:#1a1d25;--accent:#4f6ef7;--accent2:#3b5bdb;--sidebar-w:190px;--topbar-h:44px}
*{box-sizing:border-box;margin:0;padding:0}
body{font-family:-apple-system,'Segoe UI',system-ui,sans-serif;background:var(--bg);color:var(--text);height:100vh;overflow:hidden;display:flex;-webkit-font-smoothing:antialiased;font-size:13px}

/* SIDEBAR */
.sidebar{width:var(--sidebar-w);background:var(--bg2);border-right:1px solid var(--border);display:flex;flex-direction:column;flex-shrink:0;z-index:20;overflow:hidden}
.sidebar-brand{padding:16px 14px 12px;display:flex;align-items:center;gap:10px;border-bottom:1px solid var(--border)}
.brand-icon{width:28px;height:28px;border-radius:6px;background:var(--white);display:flex;align-items:center;justify-content:center;font-weight:900;font-size:9px;color:var(--bg);letter-spacing:-.3px;flex-shrink:0}
.brand-text{display:flex;flex-direction:column}
.brand-name{font-size:13px;font-weight:800;color:var(--white);letter-spacing:.3px}
.brand-sub{font-size:8px;color:var(--dim);text-transform:uppercase;letter-spacing:1.5px;font-weight:600}
.sidebar-nav{flex:1;overflow-y:auto;padding:8px 0}
.sidebar-nav::-webkit-scrollbar{width:2px}
.sidebar-nav::-webkit-scrollbar-thumb{background:var(--dim2);border-radius:2px}
.nav-section{padding:0 10px;margin-bottom:4px}
.nav-section-title{font-size:8.5px;text-transform:uppercase;letter-spacing:1.4px;color:var(--dim);font-weight:700;padding:8px 8px 5px;display:flex;align-items:center;justify-content:space-between;cursor:pointer;user-select:none}
.nav-section-title svg{width:10px;height:10px;transition:transform .15s}
.nav-section.collapsed .nav-section-title svg{transform:rotate(-90deg)}
.nav-section.collapsed .nav-items{display:none}
.nav-item{display:block;width:100%;padding:6px 12px;border-radius:6px;font-size:12px;color:var(--dim);cursor:pointer;border:none;background:none;text-align:left;font-weight:500;transition:all .1s;font-family:inherit;letter-spacing:.1px}
.nav-item:hover{color:var(--text);background:var(--bg3)}
.nav-item.active{color:var(--white);background:var(--accent);font-weight:600}
.sidebar-bottom{border-top:1px solid var(--border);padding:10px 14px}
.sidebar-user{display:flex;align-items:center;gap:8px}
.user-avatar{width:28px;height:28px;border-radius:50%;background:var(--bg5);display:flex;align-items:center;justify-content:center;font-size:10px;font-weight:700;color:var(--dim)}
.user-info{display:flex;flex-direction:column}
.user-name{font-size:11px;font-weight:600;color:var(--text)}
.user-status{font-size:9px;color:#22c55e;display:flex;align-items:center;gap:3px}
.user-status-dot{width:5px;height:5px;border-radius:50%;background:#22c55e}

/* MAIN */
.main{flex:1;display:flex;flex-direction:column;overflow:hidden;position:relative}
.topbar{height:var(--topbar-h);background:var(--bg2);border-bottom:1px solid var(--border);display:flex;align-items:center;justify-content:space-between;padding:0 18px;flex-shrink:0}
.topbar-left{display:flex;align-items:center;gap:10px}
.topbar-page{font-size:13px;font-weight:700;color:var(--white)}
.topbar-desc{font-size:11px;color:var(--dim)}
.topbar-right{display:flex;align-items:center;gap:10px}
.badge-live{display:flex;align-items:center;gap:5px;background:rgba(239,68,68,.12);border:1px solid rgba(239,68,68,.25);padding:3px 10px;border-radius:12px;font-size:9px;font-weight:700;color:#ef4444;letter-spacing:.3px}
.badge-live-dot{width:6px;height:6px;border-radius:50%;background:#ef4444;animation:blink 1.2s infinite}
@keyframes blink{0%,100%{opacity:1}50%{opacity:.3}}
.badge-conn{display:flex;align-items:center;gap:5px;background:rgba(34,197,94,.1);border:1px solid rgba(34,197,94,.2);padding:3px 10px;border-radius:12px;font-size:9px;font-weight:600;color:#22c55e}

/* CONTENT */
.content{flex:1;position:relative;overflow:hidden}
.page{display:none;width:100%;height:100%;flex-direction:column}
.page.active{display:flex}

/* MAP PAGE */
.map-bar{display:flex;align-items:center;justify-content:space-between;padding:8px 18px;flex-shrink:0;background:var(--bg);border-bottom:1px solid var(--border)}
.map-pills{display:flex;gap:2px;background:var(--bg3);border-radius:8px;padding:2px;border:1px solid var(--border)}
.map-pill{padding:5px 14px;border-radius:6px;font-size:10px;font-weight:600;color:var(--dim);cursor:pointer;border:none;background:none;transition:all .12s;letter-spacing:.2px;font-family:inherit}
.map-pill:hover{color:var(--text)}
.map-pill.active{background:var(--white);color:var(--bg);box-shadow:0 1px 3px rgba(0,0,0,.3)}
.map-right{display:flex;align-items:center;gap:8px}
.map-info-chip{display:flex;align-items:center;gap:4px;padding:4px 10px;border-radius:14px;font-size:9px;font-weight:600;color:var(--dim);background:var(--bg3);border:1px solid var(--border)}
.map-info-dot{width:5px;height:5px;border-radius:50%;flex-shrink:0}
#map-container{flex:1;position:relative}
#gta-map{width:100%;height:100%;background:#0a1020}
.leaflet-container{background:#0a1020!important;font-family:inherit}
.leaflet-control-zoom{display:none!important}
.leaflet-popup-content-wrapper{background:var(--bg3)!important;color:var(--text)!important;border-radius:8px!important;border:1px solid var(--border)!important;box-shadow:0 4px 20px rgba(0,0,0,.6)!important}
.leaflet-popup-tip{background:var(--bg3)!important;border:1px solid var(--border)!important}
.leaflet-popup-content{margin:8px 10px!important;font-size:10px!important}
.leaflet-popup-close-button{color:var(--dim)!important}

/* Map HUD */
.map-hud{position:absolute;top:12px;left:12px;z-index:800;background:rgba(11,12,16,.88);backdrop-filter:blur(12px);border:1px solid var(--border);border-radius:8px;padding:8px 12px;min-width:160px}
.map-hud-title{font-size:7px;text-transform:uppercase;letter-spacing:1.2px;color:var(--dim);font-weight:700;margin-bottom:4px}
.hud-row{display:flex;gap:8px;margin-bottom:2px}
.hud-item{font-family:'Cascadia Code',Consolas,monospace;font-size:9.5px}
.hud-lbl{color:var(--dim);font-size:8px;margin-right:2px}
.hud-val{color:var(--white)}
.hud-bar{height:2px;background:var(--bg4);border-radius:1px;margin-top:2px;overflow:hidden}
.hud-bar-fill{height:100%;border-radius:1px;transition:width .3s}
.hud-bar-hp{background:#22c55e}
.hud-bar-arm{background:#38bdf8}
.map-zoom{position:absolute;top:12px;right:12px;z-index:800;display:flex;flex-direction:column;gap:3px}
.zbtn{width:28px;height:28px;background:rgba(11,12,16,.88);border:1px solid var(--border);border-radius:6px;color:var(--text);font-size:14px;font-weight:700;cursor:pointer;display:flex;align-items:center;justify-content:center;transition:all .12s;backdrop-filter:blur(12px)}
.zbtn:hover{background:var(--white);color:var(--bg);border-color:var(--white)}
.admin-alert{position:absolute;top:12px;left:50%;transform:translateX(-50%);z-index:900;background:#ef4444;color:#fff;padding:4px 14px;border-radius:12px;font-size:10px;font-weight:700;display:none;letter-spacing:.3px;white-space:nowrap}
.wp-card{position:absolute;bottom:12px;right:12px;z-index:800;background:rgba(11,12,16,.92);border:1px solid var(--border);border-radius:8px;padding:10px 14px;display:none;flex-direction:column;gap:5px;min-width:200px;backdrop-filter:blur(12px)}
.wp-title{font-size:8px;text-transform:uppercase;letter-spacing:1px;color:var(--dim);font-weight:700}
.wp-coords{font-family:Consolas,monospace;font-size:11px;color:var(--white)}
.wp-btns{display:flex;gap:4px}
.wp-tp{background:var(--white);color:var(--bg);border:none;padding:6px;border-radius:5px;font-weight:700;font-size:10px;cursor:pointer;flex:1}
.wp-tp:hover{opacity:.85}
.wp-clear{background:var(--bg4);color:var(--dim);border:1px solid var(--border);padding:3px 8px;border-radius:4px;font-size:9px;cursor:pointer;font-weight:600}

/* Markers */
.gta-self{width:14px;height:14px;border-radius:50%;background:#f59e0b;border:2px solid #fff;box-shadow:0 0 8px #f59e0b;animation:pulse 1.5s infinite}
.gta-admin{width:12px;height:12px;border-radius:50%;background:#ef4444;border:2px solid #fff;box-shadow:0 0 8px rgba(239,68,68,.5);animation:pulse 1.2s infinite}
.gta-player{width:8px;height:8px;border-radius:50%;background:#38bdf8;border:1.5px solid rgba(255,255,255,.6);box-shadow:0 0 4px rgba(56,189,248,.4)}
.gta-vehicle{width:7px;height:7px;border-radius:2px;background:#a855f7;border:1px solid rgba(255,255,255,.4);box-shadow:0 0 4px rgba(168,85,247,.4)}
.gta-wp{width:12px;height:12px;border-radius:50%;background:#22c55e;border:2px solid #fff;box-shadow:0 0 8px #22c55e}
.marker-label{position:absolute;white-space:nowrap;font-size:8px;font-weight:700;left:50%;transform:translateX(-50%);top:-14px;text-shadow:0 1px 2px #000,0 0 6px rgba(0,0,0,.8);pointer-events:none}
@keyframes pulse{0%,100%{opacity:1;transform:scale(1)}50%{opacity:.7;transform:scale(1.12)}}
.pop-title{font-weight:700;font-size:11px;margin-bottom:3px}
.pop-info{color:var(--dim);font-size:9px;line-height:1.5}

/* CONFIG PAGES */
.cfg-page{display:none;flex-direction:column;overflow-y:auto;padding:20px 24px;gap:0}
.cfg-page.active{display:flex}
.cfg-title{font-size:16px;font-weight:700;color:var(--white);margin-bottom:4px}
.cfg-desc{font-size:11px;color:var(--dim);margin-bottom:16px}
.cfg-section{margin-bottom:18px}
.cfg-section-title{font-size:9px;text-transform:uppercase;letter-spacing:1.2px;color:var(--dim);font-weight:700;margin-bottom:8px;padding-bottom:6px;border-bottom:1px solid var(--border)}
.cfg-grid{display:grid;grid-template-columns:1fr 1fr;gap:0 28px}
.ctrl{display:flex;align-items:center;justify-content:space-between;padding:6px 0;min-height:30px}
.clbl{font-size:12px;color:var(--text);font-weight:500;display:flex;align-items:center;gap:5px}
.cwarn{font-size:8px;color:#ef4444;font-weight:700;opacity:.7}
.switch{position:relative;width:34px;height:17px;flex-shrink:0}
.switch input{opacity:0;width:0;height:0;position:absolute}
.sw-s{position:absolute;cursor:pointer;inset:0;background:var(--bg4);border-radius:17px;transition:.15s;border:1px solid var(--border)}
.sw-s::before{content:'';position:absolute;height:11px;width:11px;left:2px;bottom:2px;background:var(--dim);border-radius:50%;transition:.15s}
input:checked+.sw-s{background:var(--accent);border-color:var(--accent)}
input:checked+.sw-s::before{transform:translateX(17px);background:var(--white)}
.rng{padding:5px 0}
.rng-h{display:flex;justify-content:space-between;font-size:11px;margin-bottom:3px}
.rng-h span{color:var(--dim)}
.rng-h b{color:var(--white);font-weight:700;font-family:Consolas,monospace;font-size:10px}
input[type=range]{width:100%;height:3px;-webkit-appearance:none;appearance:none;background:var(--bg4);border-radius:3px;outline:0;cursor:pointer}
input[type=range]::-webkit-slider-thumb{-webkit-appearance:none;width:12px;height:12px;border-radius:50%;background:var(--white);cursor:pointer;box-shadow:0 0 4px rgba(0,0,0,.4)}
.info-text{color:var(--dim);font-size:11px;padding:6px 0;line-height:1.6}
.tp-list{display:flex;flex-direction:column;gap:4px}
.tp-row{display:flex;align-items:center;justify-content:space-between;padding:8px 12px;background:var(--bg3);border:1px solid var(--border);border-radius:6px}
.tp-name{font-weight:600;font-size:12px}
.tp-coords{font-size:9px;color:var(--dim);font-family:Consolas,monospace}
.tp-go{background:var(--white);color:var(--bg);border:none;padding:4px 14px;border-radius:5px;font-size:10px;font-weight:700;cursor:pointer}
.tp-go:hover{opacity:.85}
.dtable{width:100%;border-collapse:collapse;font-size:11.5px}
.dtable th{text-align:left;font-size:8px;text-transform:uppercase;letter-spacing:.7px;color:var(--dim);font-weight:700;padding:6px 8px;border-bottom:1px solid var(--border)}
.dtable td{padding:6px 8px;border-bottom:1px solid var(--border);color:var(--text)}
.dtable tr:hover td{background:rgba(255,255,255,.02)}
.badge-admin{color:#ef4444;font-size:9px;font-weight:700}
.badge-player{color:var(--dim);font-size:9px;font-weight:600}

/* CONNECT SCREEN */
.connect-overlay{position:fixed;inset:0;background:var(--bg);z-index:9999;display:flex;align-items:center;justify-content:center}
.connect-box{width:360px;text-align:center}
.connect-logo{width:56px;height:56px;border-radius:12px;background:var(--white);display:flex;align-items:center;justify-content:center;font-weight:900;font-size:16px;color:var(--bg);margin:0 auto 20px;letter-spacing:-.5px}
.connect-title{font-size:22px;font-weight:800;color:var(--white);margin-bottom:6px}
.connect-desc{font-size:12px;color:var(--dim);margin-bottom:28px;line-height:1.5}
.connect-input{width:100%;padding:14px 16px;background:var(--bg3);border:1px solid var(--border);border-radius:8px;color:var(--white);font-size:20px;font-weight:700;text-align:center;letter-spacing:12px;outline:none;font-family:Consolas,monospace;transition:border-color .15s}
.connect-input:focus{border-color:var(--accent)}
.connect-input::placeholder{color:var(--dim2);letter-spacing:4px;font-size:13px;font-weight:500}
.connect-btn{width:100%;padding:12px;background:var(--accent);border:none;border-radius:8px;color:var(--white);font-size:13px;font-weight:700;cursor:pointer;margin-top:12px;transition:all .15s;letter-spacing:.3px}
.connect-btn:hover{background:var(--accent2)}
.connect-btn:disabled{opacity:.5;cursor:not-allowed}
.connect-error{color:#ef4444;font-size:11px;margin-top:10px;display:none;font-weight:600}
.connect-hint{color:var(--dim2);font-size:10px;margin-top:20px;line-height:1.5}
</style>
</head>
<body>

<!-- CONNECT SCREEN -->
<div class="connect-overlay" id="connect-screen">
<div class="connect-box">
<div class="connect-logo">p4</div>
<div class="connect-title">p4m Web Panel</div>
<div class="connect-desc">Enter the connection code shown in the p4m overlay menu<br>Settings > Web Map Radar Server</div>
<input class="connect-input" id="connect-code" type="text" maxlength="6" placeholder="000000" autocomplete="off" autofocus>
<button class="connect-btn" id="connect-btn" onclick="doConnect()">Connect</button>
<div class="connect-error" id="connect-error">Invalid code. Check the code in the overlay menu.</div>
<div class="connect-hint">The code is generated when the web server starts.<br>You can find it in Settings inside the p4m menu.</div>
</div>
</div>

<!-- SIDEBAR -->
<nav class="sidebar">
<div class="sidebar-brand">
<div class="brand-icon">p4</div>
<div class="brand-text"><span class="brand-name">p4m</span><span class="brand-sub">Web Panel</span></div>
</div>
<div class="sidebar-nav">
<div class="nav-section" id="sec-menu">
<div class="nav-section-title" onclick="toggleSection('sec-menu')">Menu <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5"><polyline points="6 9 12 15 18 9"/></svg></div>
<div class="nav-items">
<button class="nav-item" onclick="go('aimbot')">Aimbot</button>
<button class="nav-item" onclick="go('silentaim')">SilentAim</button>
<button class="nav-item" onclick="go('triggerbot')">TriggerBot</button>
<button class="nav-item" onclick="go('autopeek')">Auto Peek</button>
<button class="nav-item" onclick="go('esp_players')">ESP Players</button>
<button class="nav-item" onclick="go('esp_vehicles')">ESP Vehicles</button>
<button class="nav-item" onclick="go('misc_esp')">Misc ESP</button>
<button class="nav-item active" onclick="go('radar')">Radar</button>
<button class="nav-item" onclick="go('players')">Players</button>
<button class="nav-item" onclick="go('vehicles')">Vehicles</button>
<button class="nav-item" onclick="go('exploits')">Exploits</button>
<button class="nav-item" onclick="go('settings')">Settings</button>
</div>
</div>
</div>
<div class="sidebar-bottom">
<div class="sidebar-user">
<div class="user-avatar">P4</div>
<div class="user-info"><span class="user-name" id="user-name">p4m</span><span class="user-status"><span class="user-status-dot"></span> Online</span></div>
</div>
</div>
</nav>

<!-- MAIN -->
<div class="main">
<div class="topbar">
<div class="topbar-left">
<span class="topbar-page" id="topbar-title">Radar</span>
<span class="topbar-desc" id="topbar-desc">Live map of the session</span>
</div>
<div class="topbar-right">
<div class="badge-conn" id="conn-badge"><span class="user-status-dot"></span> Connected</div>
<div class="badge-live"><span class="badge-live-dot"></span> Live</div>
</div>
</div>
<div class="content">

<!-- RADAR PAGE -->
<div class="page active" id="pg-radar">
<div class="map-bar">
<div class="map-pills">
<button class="map-pill active" onclick="setMapStyle('atlas',this)">Atlas</button>
<button class="map-pill" onclick="setMapStyle('satellite',this)">Satellite</button>
<button class="map-pill" onclick="setMapStyle('grid',this)">Grid</button>
<button class="map-pill" id="pill-names" onclick="toggleNames(this)">Names</button>
</div>
<div class="map-right">
<div class="map-info-chip"><span class="map-info-dot" style="background:#ef4444"></span><span id="fc-adm">0</span> Admins</div>
<div class="map-info-chip"><span class="map-info-dot" style="background:#38bdf8"></span><span id="fc-ply">0</span> Players</div>
<div class="map-info-chip"><span class="map-info-dot" style="background:#a855f7"></span><span id="fc-veh">0</span> Vehicles</div>
</div>
</div>
<div id="map-container">
<div id="gta-map"></div>
<div class="map-hud" id="map-hud">
<div class="map-hud-title">LOCAL PLAYER</div>
<div class="hud-row"><span class="hud-item"><span class="hud-lbl">X</span><span class="hud-val" id="hud-x">--</span></span><span class="hud-item"><span class="hud-lbl">Y</span><span class="hud-val" id="hud-y">--</span></span><span class="hud-item"><span class="hud-lbl">Z</span><span class="hud-val" id="hud-z">--</span></span></div>
<div style="margin-top:3px"><span class="hud-item"><span class="hud-lbl">HP</span><span class="hud-val" id="hud-hp">--</span></span><div class="hud-bar"><div class="hud-bar-fill hud-bar-hp" id="bar-hp" style="width:0"></div></div></div>
<div style="margin-top:2px"><span class="hud-item"><span class="hud-lbl">ARM</span><span class="hud-val" id="hud-arm">--</span></span><div class="hud-bar"><div class="hud-bar-fill hud-bar-arm" id="bar-arm" style="width:0"></div></div></div>
</div>
<div class="map-zoom">
<button class="zbtn" onclick="gtaMap&&gtaMap.zoomIn()">+</button>
<button class="zbtn" onclick="gtaMap&&gtaMap.zoomOut()">&minus;</button>
<button class="zbtn" onclick="centerOnSelf()" style="font-size:10px">O</button>
</div>
<div class="admin-alert" id="admin-alert"></div>
<div class="wp-card" id="wp-card">
<div class="wp-title">WAYPOINT</div>
<div class="wp-coords" id="wp-coords">X: 0, Y: 0</div>
<div class="wp-btns"><button class="wp-tp" onclick="teleportWP()">TELEPORT</button><button class="wp-clear" onclick="clearWP()">Clear</button></div>
</div>
</div>
</div>

<!-- CONFIG PAGES -->
<div class="page" id="pg-aimbot"><div class="cfg-page active" id="cfg-aimbot"></div></div>
<div class="page" id="pg-silentaim"><div class="cfg-page active" id="cfg-silentaim"></div></div>
<div class="page" id="pg-triggerbot"><div class="cfg-page active" id="cfg-triggerbot"></div></div>
<div class="page" id="pg-autopeek"><div class="cfg-page active" id="cfg-autopeek"></div></div>
<div class="page" id="pg-esp_players"><div class="cfg-page active" id="cfg-esp_players"></div></div>
<div class="page" id="pg-esp_vehicles"><div class="cfg-page active" id="cfg-esp_vehicles"></div></div>
<div class="page" id="pg-misc_esp"><div class="cfg-page active" id="cfg-misc_esp"></div></div>
<div class="page" id="pg-players"><div class="cfg-page active" id="cfg-players"></div></div>
<div class="page" id="pg-vehicles"><div class="cfg-page active" id="cfg-vehicles"></div></div>
<div class="page" id="pg-exploits"><div class="cfg-page active" id="cfg-exploits"></div></div>
<div class="page" id="pg-settings"><div class="cfg-page active" id="cfg-settings"></div></div>

</div>
</div>

<script src="https://cdnjs.cloudflare.com/ajax/libs/leaflet/1.9.4/leaflet.min.js"></script>
<script>
var cfg={},blipData={},gtaMap=null,tileLayers={},currentStyle='atlas';
var layerGroups={},selfMarker=null,wpMarker=null,wpData=null;
var showNames=true,curPage='radar';
var authCode=localStorage.getItem('p4m_auth')||'';
var authenticated=false;

function doConnect(){
var code=document.getElementById('connect-code').value.trim();
if(code.length!==6){document.getElementById('connect-error').style.display='block';document.getElementById('connect-error').textContent='Enter a 6-digit code.';return}
var btn=document.getElementById('connect-btn');btn.disabled=true;btn.textContent='Connecting...';
fetch('/api/auth?code='+code).then(function(r){return r.json()}).then(function(d){
if(d.authenticated){
authCode=code;authenticated=true;localStorage.setItem('p4m_auth',code);
document.getElementById('connect-screen').style.display='none';
initMap();poll();setInterval(poll,350);
}else{
document.getElementById('connect-error').style.display='block';
document.getElementById('connect-error').textContent='Invalid code. Check the code in the overlay menu.';
btn.disabled=false;btn.textContent='Connect';
}
}).catch(function(){
document.getElementById('connect-error').style.display='block';
document.getElementById('connect-error').textContent='Cannot reach p4m server. Make sure the cheat is running.';
btn.disabled=false;btn.textContent='Connect';
});
}
document.getElementById('connect-code').addEventListener('keydown',function(e){if(e.key==='Enter')doConnect()});

function api(path){return path+(path.indexOf('?')>=0?'&':'?')+'auth='+authCode}


var PAGE_INFO={
radar:['Radar','Live map of the session'],
aimbot:['Aimbot','Aimbot configuration'],
silentaim:['SilentAim','Silent aim configuration'],
triggerbot:['TriggerBot','TriggerBot configuration'],
autopeek:['Auto Peek','Auto peek configuration'],
esp_players:['ESP Players','Player ESP settings'],
esp_vehicles:['ESP Vehicles','Vehicle ESP settings'],
misc_esp:['Misc ESP','Miscellaneous ESP and local player options'],
players:['Players','Online players list'],
vehicles:['Vehicles','Vehicle list'],
exploits:['Exploits','Weapon exploits'],
settings:['Settings','General settings']
};

var TELEPORTS=[
{name:'Legion Square',x:215.4,y:-810.2,z:30.7},
{name:'Del Perro Pier',x:-1655.2,y:-1032.1,z:13.1},
{name:'Paleto Bay',x:-446.1,y:6013.8,z:31.7},
{name:'Central Bank',x:221.8,y:217.3,z:106.7},
{name:'Diamond Casino',x:925.3,y:46.2,z:80.9},
{name:'LS Airport',x:-1037,y:-2737,z:20.1},
{name:'Sandy Shores',x:1747,y:3273.7,z:41.1},
{name:'Fort Zancudo',x:-2047.4,y:3132.1,z:32.8},
{name:'Mount Chiliad',x:501.8,y:5604.5,z:797.9},
{name:'Maze Bank Tower',x:-75,y:-818.9,z:326.2},
];

function tog(label,key,ac){
var w=ac?'<span class="cwarn">AC</span>':'';
return '<div class="ctrl"><span class="clbl">'+label+' '+w+'</span><label class="switch"><input type="checkbox" id="c-'+key+'" onchange="setVal(\''+key+'\',this.checked)"><span class="sw-s"></span></label></div>';
}
function sld(label,key,min,max,step,suffix){
step=step||1;suffix=suffix||'';
return '<div class="rng"><div class="rng-h"><span>'+label+'</span><b id="v-'+key+'">--</b></div><input type="range" min="'+min+'" max="'+max+'" step="'+step+'" id="c-'+key+'" oninput="document.getElementById(\'v-'+key+'\').textContent=this.value+\''+suffix+'\'" onchange="setVal(\''+key+'\',this.value)"></div>';
}

function go(id){
curPage=id;
document.querySelectorAll('.page').forEach(function(p){p.classList.remove('active')});
document.getElementById('pg-'+id).classList.add('active');
document.querySelectorAll('.nav-item').forEach(function(b){b.classList.remove('active')});
var items=document.querySelectorAll('.nav-item');
for(var i=0;i<items.length;i++){if(items[i].textContent.trim()===PAGE_INFO[id][0].replace('ESP ','').replace('Misc ','Misc '))items[i].classList.add('active')}
var found=false;items.forEach(function(b){var t=b.textContent.trim();
var map={'Aimbot':'aimbot','SilentAim':'silentaim','TriggerBot':'triggerbot','Auto Peek':'autopeek','ESP Players':'esp_players','ESP Vehicles':'esp_vehicles','Misc ESP':'misc_esp','Radar':'radar','Players':'players','Vehicles':'vehicles','Exploits':'exploits','Settings':'settings'};
if(map[t]===id){b.classList.add('active');found=true}
});
var info=PAGE_INFO[id]||[id,''];
document.getElementById('topbar-title').textContent=info[0];
document.getElementById('topbar-desc').textContent=info[1];
if(id==='radar'&&gtaMap)setTimeout(function(){gtaMap.invalidateSize()},50);
if(id!=='radar')renderCfgPage(id);
}

function renderCfgPage(id){
var el=document.getElementById('cfg-'+id);
if(!el)return;
var h='';
if(id==='aimbot'){
h='<div class="cfg-title">Aimbot</div><div class="cfg-desc">Configure aimbot targeting and smoothing</div>';
h+='<div class="cfg-section"><div class="cfg-section-title">TOGGLES</div><div class="cfg-grid">'+tog('Enable','aimbot_enabled')+tog('Only Visible','aimbot_visible')+tog('Ignore NPCs','aimbot_ignore_npcs')+tog('Show FOV Circle','aimbot_showfov')+'</div></div>';
h+='<div class="cfg-section"><div class="cfg-section-title">VALUES</div>'+sld('Field of View','aimbot_fov',0,400)+sld('Smooth','aimbot_speed',0,100)+sld('Max Distance','aimbot_distance',0,1000,1,'m')+'</div>';
}else if(id==='silentaim'){
h='<div class="cfg-title">SilentAim</div><div class="cfg-desc">Configure silent aim settings</div>';
h+='<div class="cfg-section"><div class="cfg-section-title">TOGGLES</div><div class="cfg-grid">'+tog('Enable','silentaim_enabled')+tog('Magic Bullets','silentaim_magic')+tog('Only Visible','silentaim_visible')+tog('Ignore NPCs','silentaim_ignore_npcs')+tog('Show FOV Circle','silentaim_showfov')+'</div></div>';
h+='<div class="cfg-section"><div class="cfg-section-title">VALUES</div>'+sld('Field of View','silentaim_fov',0,400)+sld('Miss Chance','silentaim_miss',0,100,1,'%')+sld('Max Distance','silentaim_distance',0,1000,1,'m')+'</div>';
}else if(id==='triggerbot'){
h='<div class="cfg-title">TriggerBot</div><div class="cfg-desc">Configure triggerbot settings</div>';
h+='<div class="cfg-section"><div class="cfg-section-title">TOGGLES</div><div class="cfg-grid">'+tog('Enable','triggerbot_enabled')+tog('Smart Triggerbot','triggerbot_smart')+tog('Only Visible','triggerbot_visible')+tog('Ignore NPCs','triggerbot_ignore_npcs')+tog('Show FOV Circle','triggerbot_showfov')+'</div></div>';
h+='<div class="cfg-section"><div class="cfg-section-title">VALUES</div>'+sld('Field of View','triggerbot_fov',0,400)+sld('Max Distance','triggerbot_distance',0,1000,1,'m')+sld('Delay','triggerbot_delay',0,10,1,'ms')+'</div>';
}else if(id==='autopeek'){
h='<div class="cfg-title">Auto Peek</div><div class="cfg-desc">Peek around corners and teleport back</div>';
h+='<div class="cfg-section"><div class="cfg-section-title">TOGGLES</div>'+tog('Enable','autopeek')+'</div>';
h+='<div class="cfg-section"><p class="info-text">Press the peek key once to save your position. Peek around corners, then press again to teleport back. Configure the keybind in the overlay menu.</p></div>';
}else if(id==='esp_players'){
h='<div class="cfg-title">ESP Players</div><div class="cfg-desc">Player visual overlays</div>';
h+='<div class="cfg-section"><div class="cfg-section-title">TOGGLES</div><div class="cfg-grid">'+tog('Enable','esp_enabled')+tog('Admin ESP','esp_admin')+tog('Admin Multi-Color','esp_admin_rainbow')+tog('Box','esp_box')+tog('Skeleton','esp_skeleton')+tog('SnapLines','esp_snaplines')+tog('Head Circle','esp_headcircle')+tog('Weapon Names','esp_weapon')+tog('Player Names','esp_names')+tog('Distance','esp_distance')+tog('Health Bar','esp_health')+tog('Armor Bar','esp_armor')+'</div></div>';
h+='<div class="cfg-section"><div class="cfg-section-title">RANGE</div>'+sld('Max Distance','esp_max_distance',0,1000,1,'m')+'</div>';
}else if(id==='esp_vehicles'){
h='<div class="cfg-title">ESP Vehicles</div><div class="cfg-desc">Vehicle visual overlays</div>';
h+='<div class="cfg-section"><div class="cfg-section-title">TOGGLES</div><div class="cfg-grid">'+tog('Enable','vehesp_enabled')+tog('Vehicle Names','vehesp_names')+tog('Locked/Unlocked','vehesp_lockunlock')+tog('SnapLines','vehesp_snaplines')+tog('Distance','vehesp_distance')+'</div></div>';
h+='<div class="cfg-section"><div class="cfg-section-title">RANGE</div>'+sld('Max Distance','vehesp_max_distance',0,1000,1,'m')+'</div>';
}else if(id==='misc_esp'){
h='<div class="cfg-title">Misc ESP</div><div class="cfg-desc">Local player options and miscellaneous</div>';
h+='<div class="cfg-section"><div class="cfg-section-title">ESP OPTIONS</div><div class="cfg-grid">'+tog('Show LocalPlayer','esp_showlocal')+tog('Ignore Dead','esp_ignore_dead')+tog('Ignore NPCs','esp_ignore_npcs')+tog('Friend Marker','esp_friends')+'</div></div>';
h+='<div class="cfg-section"><div class="cfg-section-title">LOCAL PLAYER</div><div class="cfg-grid">'+tog('GodMode','godmode',1)+tog('NoClip','noclip',1)+tog('Fast Run','fastrun',1)+tog('Anti Headshot','antihs',1)+tog('Shrink','shrink',1)+tog('No RagDoll','noragdoll')+tog('Steal Car','stealcar')+tog('Infinite CombatRoll','combatroll')+tog('Force Weapon Wheel','weaponwheel')+tog('No Reload','noreload')+tog('Infinite Ammo','ammo')+tog('Infinite Stamina','stamina')+'</div></div>';
h+='<div class="cfg-section"><div class="cfg-section-title">SPEED</div>'+sld('NoClip Speed','noclip_speed',0.1,20,0.1)+sld('Run Speed','fastrun_speed',0.1,10,0.1)+'</div>';
h+='<div class="cfg-section"><div class="cfg-section-title">VEHICLE</div><div class="cfg-grid">'+tog('Vehicle GodMode','veh_godmode')+tog('SeatBelt','seatbelt')+'</div></div>';
h+='<div class="cfg-section"><div class="cfg-section-title">WEAPONS</div><div class="cfg-grid">'+tog('No Recoil','norecoil')+tog('No Spread','nospread')+'</div></div>';
}else if(id==='players'){
h='<div class="cfg-title">Players</div><div class="cfg-desc">Online players in the session</div>';
var bd=blipData;
if(bd.players&&bd.players.length){
h+='<table class="dtable"><thead><tr><th>Name</th><th>Weapon</th><th>HP</th><th>Armor</th><th>Distance</th><th></th></tr></thead><tbody>';
bd.players.forEach(function(p){
var badge=p.is_admin?'<span class="badge-admin">ADMIN</span>':'<span class="badge-player">PLAYER</span>';
h+='<tr><td>'+p.name+' '+badge+'</td><td style="color:var(--dim);font-size:10px">'+p.weapon.replace('WEAPON_','')+'</td><td>'+p.health+'</td><td>'+p.armor+'</td><td>'+p.dist+'m</td><td><button class="tp-go" style="font-size:8px;padding:2px 8px" onclick="fetch(api(\'/api/teleport?x='+p.x+'&y='+p.y+'\'))">TP</button></td></tr>';
});
h+='</tbody></table>';
}else h+='<p class="info-text">No players detected.</p>';
}else if(id==='vehicles'){
h='<div class="cfg-title">Vehicles</div><div class="cfg-desc">Vehicles in the session</div>';
var bd=blipData;
if(bd.vehicles&&bd.vehicles.length){
h+='<table class="dtable"><thead><tr><th>Name</th><th>Driver</th><th>Distance</th></tr></thead><tbody>';
bd.vehicles.forEach(function(v){
h+='<tr><td>'+v.name+'</td><td>'+(v.has_driver?'Yes':'No')+'</td><td>'+v.dist+'m</td></tr>';
});
h+='</tbody></table>';
}else h+='<p class="info-text">No vehicles detected.</p>';
}else if(id==='exploits'){
h='<div class="cfg-title">Exploits</div><div class="cfg-desc">Weapon spawn exploits</div>';
h+='<div class="cfg-section"><div class="cfg-section-title">TELEPORT LOCATIONS</div><div class="tp-list">';
TELEPORTS.forEach(function(t){
h+='<div class="tp-row"><div><div class="tp-name">'+t.name+'</div><div class="tp-coords">'+t.x.toFixed(1)+', '+t.y.toFixed(1)+', '+t.z.toFixed(1)+'</div></div><button class="tp-go" onclick="fetch(api(\'/api/teleport?x='+t.x+'&y='+t.y+'&z='+t.z+'\'))">GO</button></div>';
});
h+='</div></div>';
}else if(id==='settings'){
h='<div class="cfg-title">Settings</div><div class="cfg-desc">General application settings</div>';
h+='<div class="cfg-section"><div class="cfg-section-title">GENERAL</div><div class="cfg-grid">'+tog('WaterMark','watermark')+tog('Stream Proof','streamproof')+'</div></div>';
h+='<div class="cfg-section"><div class="cfg-section-title">WEB RADAR</div><div class="cfg-grid">'+tog('Web Server','web_enabled')+tog('Cloud Sync','web_cloudsync')+'</div></div>';
h+='<div class="cfg-section"><p class="info-text">Menu Key / Panic Key / Config Export-Import are available in the overlay menu.</p></div>';
}
el.innerHTML=h;
syncCfgUI();
}

function toggleSection(id){document.getElementById(id).classList.toggle('collapsed')}
function setVal(key,val){
var v=(val===true||val===false)?val:parseFloat(val);
cfg[key]=v;
fetch(api('/api/set?'+key+'='+(v===true?'true':v===false?'false':v)));
}
function syncCfgUI(){
Object.keys(cfg).forEach(function(k){
var el=document.getElementById('c-'+k);
if(!el)return;
if(el.type==='checkbox')el.checked=!!cfg[k];
else if(el.type==='range'){el.value=cfg[k];var vl=document.getElementById('v-'+k);if(vl)vl.textContent=cfg[k]}
});
}

// MAP
function initMap(){
var GTACRS=L.extend({},L.CRS.Simple,{transformation:new L.Transformation(0.02072,117.3,-0.0205,172.8)});
gtaMap=L.map('gta-map',{crs:GTACRS,minZoom:1,maxZoom:5,zoom:3,center:[200,0],zoomControl:false,attributionControl:false,maxBounds:L.latLngBounds(L.latLng(-4600,-6000),L.latLng(8600,6800))});
var base=location.protocol+'//'+location.host;
['atlas','satellite','grid'].forEach(function(s){
var folder=s==='satellite'?'styleSatelite':s==='grid'?'styleGrid':'styleAtlas';
tileLayers[s]=L.tileLayer(base+'/mapStyles/'+folder+'/{z}/{x}/{y}.jpg',{minZoom:1,maxZoom:5,tileSize:256,noWrap:true,errorTileUrl:base+'/mapStyles/styleAtlas/empty.jpg'});
});
tileLayers.atlas.addTo(gtaMap);
layerGroups={players:L.layerGroup().addTo(gtaMap),vehicles:L.layerGroup().addTo(gtaMap),admins:L.layerGroup().addTo(gtaMap)};
gtaMap.on('click',function(e){setWaypoint(e.latlng)});
}
function setMapStyle(s,btn){
if(tileLayers[currentStyle])gtaMap.removeLayer(tileLayers[currentStyle]);
currentStyle=s;tileLayers[s].addTo(gtaMap);
document.querySelectorAll('.map-pill').forEach(function(b,i){if(i<3)b.classList.remove('active')});
if(btn)btn.classList.add('active');
}
function toggleNames(btn){showNames=!showNames;btn.classList.toggle('active',showNames)}
function centerOnSelf(){if(blipData.local)gtaMap.setView([blipData.local.y,blipData.local.x],4)}
function setWaypoint(ll){
wpData={lat:ll.lat,lng:ll.lng};
if(wpMarker)gtaMap.removeLayer(wpMarker);
wpMarker=L.marker(ll,{icon:L.divIcon({className:'gta-wp',iconSize:[12,12],iconAnchor:[6,6]})}).addTo(gtaMap);
document.getElementById('wp-card').style.display='flex';
document.getElementById('wp-coords').textContent='X: '+ll.lng.toFixed(1)+', Y: '+ll.lat.toFixed(1);
}
function teleportWP(){if(wpData)fetch(api('/api/teleport?x='+wpData.lng.toFixed(1)+'&y='+wpData.lat.toFixed(1)));clearWP()}
function clearWP(){
if(wpMarker){gtaMap.removeLayer(wpMarker);wpMarker=null}wpData=null;
document.getElementById('wp-card').style.display='none';
}

function updateMarkers(data){
if(!gtaMap||!data)return;
blipData=data;var lp=data.local;
if(lp){
document.getElementById('hud-x').textContent=lp.x.toFixed(1);
document.getElementById('hud-y').textContent=lp.y.toFixed(1);
document.getElementById('hud-z').textContent=lp.z.toFixed(1);
document.getElementById('hud-hp').textContent=lp.health;
document.getElementById('hud-arm').textContent=lp.armor||0;
document.getElementById('bar-hp').style.width=(lp.health/200*100)+'%';
document.getElementById('bar-arm').style.width=((lp.armor||0)/100*100)+'%';
if(!selfMarker)selfMarker=L.marker([lp.y,lp.x],{icon:L.divIcon({className:'gta-self',iconSize:[14,14],iconAnchor:[7,7]})}).addTo(gtaMap);
else selfMarker.setLatLng([lp.y,lp.x]);
}
['players','vehicles','admins'].forEach(function(k){layerGroups[k].clearLayers()});
var ac=0,pc=0,vc=0,hasAdmin=false;
if(data.players){data.players.forEach(function(p){
var nameHtml=showNames?'<div class="marker-label" style="color:'+(p.is_admin?'#ef4444':'#38bdf8')+'">'+p.name+'</div>':'';
if(p.is_admin){ac++;hasAdmin=true;
var m=L.marker([p.y,p.x],{icon:L.divIcon({className:'',html:nameHtml+'<div class="gta-admin"></div>',iconSize:[12,12],iconAnchor:[6,6]})});
m.bindPopup('<div class="pop-title" style="color:#ef4444">'+p.name+'</div><div class="pop-info">HP: '+p.health+' | Armor: '+p.armor+'<br>Weapon: '+p.weapon+'<br>Distance: '+p.dist+'m</div>');
layerGroups.admins.addLayer(m);
}else{pc++;
var m=L.marker([p.y,p.x],{icon:L.divIcon({className:'',html:nameHtml+'<div class="gta-player"></div>',iconSize:[8,8],iconAnchor:[4,4]})});
m.bindPopup('<div class="pop-title">'+p.name+'</div><div class="pop-info">HP: '+p.health+' | Armor: '+p.armor+'<br>Weapon: '+p.weapon+'<br>Distance: '+p.dist+'m</div>');
layerGroups.players.addLayer(m);
}})}
if(data.vehicles){data.vehicles.forEach(function(v){vc++;
var nameHtml=showNames?'<div class="marker-label" style="color:#a855f7">'+v.name+'</div>':'';
var m=L.marker([v.y,v.x],{icon:L.divIcon({className:'',html:nameHtml+'<div class="gta-vehicle"></div>',iconSize:[7,7],iconAnchor:[3,3]})});
m.bindPopup('<div class="pop-title" style="color:#a855f7">'+v.name+'</div><div class="pop-info">Distance: '+v.dist+'m<br>Driver: '+(v.has_driver?'Yes':'No')+'</div>');
layerGroups.vehicles.addLayer(m);
})}
document.getElementById('fc-adm').textContent=ac;
document.getElementById('fc-ply').textContent=pc;
document.getElementById('fc-veh').textContent=vc;
var aa=document.getElementById('admin-alert');
if(hasAdmin){aa.textContent=ac+' ADMIN'+(ac>1?'S':'')+' NEARBY';aa.style.display='block'}else aa.style.display='none';
if(curPage==='players'||curPage==='vehicles')renderCfgPage(curPage);
}

function poll(){
fetch(api('/api/blips')).then(function(r){return r.json()}).then(function(d){updateMarkers(d)}).catch(function(){});
fetch(api('/api/config')).then(function(r){return r.json()}).then(function(d){cfg=d;syncCfgUI()}).catch(function(){});
fetch(api('/api/status')).then(function(r){return r.json()}).then(function(d){
var b=document.getElementById('conn-badge');
if(d.connected){b.innerHTML='<span class="user-status-dot"></span> Connected';b.style.borderColor='rgba(34,197,94,.2)';b.style.color='#22c55e'}
else{b.innerHTML='Disconnected';b.style.borderColor='rgba(239,68,68,.2)';b.style.color='#ef4444'}
}).catch(function(){
var b=document.getElementById('conn-badge');b.innerHTML='Offline';b.style.borderColor='rgba(239,68,68,.2)';b.style.color='#ef4444';
});
}

// Try auto-reconnect with saved code
if(authCode){
fetch('/api/auth?code='+authCode).then(function(r){return r.json()}).then(function(d){
if(d.authenticated){
authenticated=true;
document.getElementById('connect-screen').style.display='none';
initMap();poll();setInterval(poll,350);
}else{localStorage.removeItem('p4m_auth');authCode=''}
}).catch(function(){});
}
</script>
</body>
</html>

)WEBAPP";
			return page;
		}
	}
}