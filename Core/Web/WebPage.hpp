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
:root{--bg:#0a0b0f;--bg2:#111318;--bg3:#181b22;--bg4:#1e2029;--bg5:#252830;--text:#e8eaed;--white:#ffffff;--dim:#62656e;--dim2:#44474f;--border:#1c1f27;--border2:#252830;--sidebar:52px;--header:40px}
*{box-sizing:border-box;margin:0;padding:0}
body{font-family:'Segoe UI',system-ui,-apple-system,sans-serif;background:var(--bg);color:var(--text);height:100vh;overflow:hidden;display:flex;-webkit-font-smoothing:antialiased;font-size:12.5px}
.sidebar{width:var(--sidebar);background:var(--bg2);border-right:1px solid var(--border);display:flex;flex-direction:column;align-items:center;padding:8px 0;gap:1px;z-index:10;flex-shrink:0}
.logo{width:30px;height:30px;border-radius:6px;background:var(--white);display:flex;align-items:center;justify-content:center;font-weight:900;font-size:9px;color:var(--bg);margin-bottom:8px;letter-spacing:-.3px;cursor:default}
.stab{width:36px;height:36px;border-radius:8px;display:flex;align-items:center;justify-content:center;cursor:pointer;color:var(--dim);transition:all .15s;border:none;background:0 0;position:relative}
.stab:hover{background:var(--bg3);color:var(--text)}
.stab.active{background:var(--white);color:var(--bg)}
.stab svg{width:16px;height:16px}
.stab .badge{position:absolute;top:3px;right:3px;width:7px;height:7px;border-radius:50%;background:#ef4444;border:2px solid var(--bg2);display:none}
.main{flex:1;display:flex;flex-direction:column;overflow:hidden}
.topbar{height:var(--header);background:var(--bg2);border-bottom:1px solid var(--border);display:flex;align-items:center;justify-content:space-between;padding:0 16px;flex-shrink:0}
.topbar-left{font-size:9px;letter-spacing:1.5px;text-transform:uppercase;color:var(--dim);font-weight:600}
.topbar-right{display:flex;align-items:center;gap:14px}
.conn{display:flex;align-items:center;gap:5px;font-size:10px;color:var(--dim)}
.conn-dot{width:6px;height:6px;border-radius:50%;background:#22c55e}
.sub-exp{font-size:9px;color:var(--dim);letter-spacing:.3px}
.sub-exp b{color:var(--text);font-weight:700}
.content{flex:1;overflow:hidden;position:relative}
.page{display:none;width:100%;height:100%;flex-direction:column;overflow:hidden}
.page.active{display:flex}
.subtabs{display:flex;gap:3px;padding:10px 16px 6px;flex-shrink:0}
.subtab{background:var(--bg3);border:1px solid var(--border);color:var(--dim);padding:4px 12px;border-radius:5px;font-size:10px;font-weight:600;cursor:pointer;transition:all .12s;letter-spacing:.2px}
.subtab:hover{color:var(--text);border-color:var(--dim2)}
.subtab.active{background:var(--white);color:var(--bg);border-color:var(--white)}
.panels{display:flex;flex:1;gap:10px;padding:0 16px 10px;overflow-y:auto}
.panel{flex:1;background:var(--bg2);border:1px solid var(--border);border-radius:8px;padding:12px;display:flex;flex-direction:column;gap:0;overflow-y:auto}
.ptitle{font-size:8px;text-transform:uppercase;letter-spacing:1.2px;color:var(--dim);font-weight:700;margin-bottom:6px;padding-bottom:5px;border-bottom:1px solid var(--border)}
.ctrl{display:flex;align-items:center;justify-content:space-between;padding:5px 0;min-height:28px}
.clbl{font-size:11.5px;color:var(--text);font-weight:500;display:flex;align-items:center;gap:4px}
.cwarn{font-size:8px;color:#ef4444;font-weight:700;opacity:.7}
.switch{position:relative;width:32px;height:16px;flex-shrink:0}
.switch input{opacity:0;width:0;height:0;position:absolute}
.sw-s{position:absolute;cursor:pointer;inset:0;background:var(--bg4);border-radius:16px;transition:.15s;border:1px solid var(--border)}
.sw-s::before{content:'';position:absolute;height:10px;width:10px;left:2px;bottom:2px;background:var(--dim);border-radius:50%;transition:.15s}
input:checked+.sw-s{background:var(--white);border-color:var(--white)}
input:checked+.sw-s::before{transform:translateX(16px);background:var(--bg)}
.rng{padding:4px 0}
.rng-h{display:flex;justify-content:space-between;font-size:10.5px;margin-bottom:2px}
.rng-h span{color:var(--dim)}
.rng-h b{color:var(--white);font-weight:700;font-family:Consolas,monospace;font-size:10px}
input[type=range]{width:100%;height:2px;-webkit-appearance:none;appearance:none;background:var(--bg4);border-radius:2px;outline:0;cursor:pointer}
input[type=range]::-webkit-slider-thumb{-webkit-appearance:none;width:10px;height:10px;border-radius:50%;background:var(--white);cursor:pointer}
.info-text{color:var(--dim);font-size:11px;padding:6px 0;line-height:1.5}
.dtable{width:100%;border-collapse:collapse;font-size:11px}
.dtable th{text-align:left;font-size:8px;text-transform:uppercase;letter-spacing:.7px;color:var(--dim);font-weight:700;padding:5px 6px;border-bottom:1px solid var(--border)}
.dtable td{padding:5px 6px;border-bottom:1px solid var(--border);color:var(--text)}
.dtable tr:hover td{background:rgba(255,255,255,.02)}
.badge-admin{color:#ef4444;font-size:9px;font-weight:700}
.badge-player{color:var(--dim);font-size:9px;font-weight:600}
.tbtn{background:var(--bg3);border:1px solid var(--border);color:var(--text);padding:2px 8px;border-radius:3px;font-size:9px;cursor:pointer;font-weight:600;transition:all .12s}
.tbtn:hover{background:var(--white);color:var(--bg);border-color:var(--white)}
.tp-list{display:flex;flex-direction:column;gap:3px}
.tp-row{display:flex;align-items:center;justify-content:space-between;padding:7px 10px;background:var(--bg3);border:1px solid var(--border);border-radius:6px}
.tp-name{font-weight:600;font-size:11.5px}
.tp-coords{font-size:9px;color:var(--dim);font-family:Consolas,monospace}
.tp-go{background:var(--white);color:var(--bg);border:none;padding:3px 12px;border-radius:4px;font-size:10px;font-weight:700;cursor:pointer;transition:all .12s}
.tp-go:hover{opacity:.85}
.btn-action{background:var(--bg3);border:1px solid var(--border);color:var(--text);padding:6px;border-radius:5px;font-size:11px;font-weight:600;cursor:pointer;width:100%;transition:all .12s;margin-top:4px}
.btn-action:hover{background:var(--white);color:var(--bg);border-color:var(--white)}
footer{height:28px;background:var(--bg2);border-top:1px solid var(--border);display:flex;align-items:center;justify-content:space-between;padding:0 16px;flex-shrink:0}
.uinfo{display:flex;align-items:center;gap:6px}
.avatar{width:16px;height:16px;border-radius:50%;background:var(--dim2)}
.uname{font-size:10px;font-weight:600;color:var(--dim)}
.ftime{font-size:9px;color:var(--dim2)}
/* MAP */
#map-wrap{position:relative;flex:1;overflow:hidden}
#gta-map{width:100%;height:100%;background:#0a1020}
.map-hud{position:absolute;top:10px;left:10px;z-index:1000;background:rgba(10,11,15,.94);backdrop-filter:blur(10px);border:1px solid var(--border);border-radius:8px;padding:10px 14px;min-width:180px}
.map-hud-title{font-size:7px;text-transform:uppercase;letter-spacing:1.2px;color:var(--dim);font-weight:700;margin-bottom:5px}
.hud-row{display:flex;gap:10px;margin-bottom:3px}
.hud-item{font-family:Consolas,monospace;font-size:10px}
.hud-lbl{color:var(--dim);font-size:8px;margin-right:2px}
.hud-val{color:var(--white)}
.hud-bar{height:2px;background:var(--bg4);border-radius:1px;margin-top:2px;overflow:hidden}
.hud-bar-fill{height:100%;border-radius:1px;transition:width .3s}
.hud-bar-hp{background:#22c55e}
.hud-bar-arm{background:#38bdf8}
.map-ctrls{position:absolute;top:10px;right:10px;z-index:1000;display:flex;flex-direction:column;gap:3px}
.mbtn{width:26px;height:26px;background:rgba(10,11,15,.92);border:1px solid var(--border);border-radius:5px;color:var(--text);font-size:14px;font-weight:700;cursor:pointer;display:flex;align-items:center;justify-content:center;transition:all .12s}
.mbtn:hover{background:var(--white);color:var(--bg);border-color:var(--white)}
.map-styles{position:absolute;top:10px;left:50%;transform:translateX(-50%);z-index:1000;display:flex;background:rgba(10,11,15,.92);border:1px solid var(--border);border-radius:6px;overflow:hidden}
.ms-btn{padding:4px 12px;font-size:9px;font-weight:600;color:var(--dim);cursor:pointer;border:none;background:0 0;transition:all .12s;letter-spacing:.3px}
.ms-btn:hover{color:var(--text)}
.ms-btn.active{background:var(--white);color:var(--bg)}
.map-chips{position:absolute;bottom:10px;left:10px;z-index:1000;display:flex;gap:4px;flex-wrap:wrap}
.fchip{background:rgba(10,11,15,.9);border:1px solid var(--border);padding:3px 8px;border-radius:12px;font-size:9px;cursor:pointer;display:flex;align-items:center;gap:3px;color:var(--dim);user-select:none;transition:all .12s;font-weight:600}
.fchip.on{border-color:var(--dim);color:var(--text)}
.fdot{width:5px;height:5px;border-radius:50%;flex-shrink:0}
.wp-card{position:absolute;bottom:10px;right:10px;z-index:1000;background:rgba(10,11,15,.95);border:1px solid var(--border);border-radius:8px;padding:10px 14px;display:none;flex-direction:column;gap:5px;min-width:200px}
.wp-title{font-size:8px;text-transform:uppercase;letter-spacing:1px;color:var(--dim);font-weight:700}
.wp-coords{font-family:Consolas,monospace;font-size:11px;color:var(--white)}
.wp-btns{display:flex;gap:4px}
.wp-tp{background:var(--white);color:var(--bg);border:none;padding:6px;border-radius:5px;font-weight:700;font-size:10px;cursor:pointer;flex:1;letter-spacing:.3px}
.wp-tp:hover{opacity:.85}
.wp-clear{background:var(--bg4);color:var(--dim);border:1px solid var(--border);padding:3px 8px;border-radius:4px;font-size:9px;cursor:pointer;font-weight:600}
.admin-alert{position:absolute;top:10px;left:50%;transform:translateX(-50%);z-index:1001;background:#ef4444;color:#fff;padding:4px 14px;border-radius:12px;font-size:10px;font-weight:700;display:none;letter-spacing:.3px;white-space:nowrap}
.leaflet-container{background:#0a1020!important;font-family:inherit}
.leaflet-control-zoom{display:none!important}
.leaflet-popup-content-wrapper{background:var(--bg3)!important;color:var(--text)!important;border-radius:6px!important;border:1px solid var(--border)!important;box-shadow:0 4px 16px rgba(0,0,0,.6)!important}
.leaflet-popup-tip{background:var(--bg3)!important;border:1px solid var(--border)!important}
.leaflet-popup-content{margin:8px 10px!important;font-size:10px!important}
.leaflet-popup-close-button{color:var(--dim)!important}
.gta-self{width:14px;height:14px;border-radius:50%;background:#f59e0b;border:2px solid #fff;box-shadow:0 0 8px #f59e0b;animation:pulse 1.5s infinite}
.gta-admin{width:12px;height:12px;border-radius:50%;background:#ef4444;border:2px solid #fff;box-shadow:0 0 8px rgba(239,68,68,.5);animation:pulse 1.2s infinite}
.gta-player{width:8px;height:8px;border-radius:50%;background:#38bdf8;border:1.5px solid rgba(255,255,255,.6);box-shadow:0 0 4px rgba(56,189,248,.4)}
.gta-vehicle{width:7px;height:7px;border-radius:2px;background:#a855f7;border:1px solid rgba(255,255,255,.4);box-shadow:0 0 4px rgba(168,85,247,.4)}
.gta-poi{width:4px;height:4px;border-radius:50%;background:#f59e0b;opacity:.5}
.gta-wp{width:12px;height:12px;border-radius:50%;background:#22c55e;border:2px solid #fff;box-shadow:0 0 8px #22c55e}
.marker-label{position:absolute;white-space:nowrap;font-size:8px;font-weight:700;left:50%;transform:translateX(-50%);top:-14px;text-shadow:0 1px 2px #000,0 0 6px rgba(0,0,0,.8);pointer-events:none}
@keyframes pulse{0%,100%{opacity:1;transform:scale(1)}50%{opacity:.7;transform:scale(1.12)}}
.pop-title{font-weight:700;font-size:11px;margin-bottom:3px}
.pop-info{color:var(--dim);font-size:9px;line-height:1.4}
.pop-btn{background:var(--white);color:var(--bg);border:none;padding:3px 8px;border-radius:3px;font-size:9px;font-weight:700;cursor:pointer;margin-top:4px}
.pop-btn:hover{opacity:.85}
</style>
</head>
<body>
<nav class="sidebar">
<div class="logo">p4</div>
<button class="stab active" onclick="switchPage('combat')" title="Combat"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="3"/><line x1="12" y1="2" x2="12" y2="6"/><line x1="12" y1="18" x2="12" y2="22"/><line x1="2" y1="12" x2="6" y2="12"/><line x1="18" y1="12" x2="22" y2="12"/></svg></button>
<button class="stab" onclick="switchPage('visuals')" title="Visuals"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M1 12s4-8 11-8 11 8 11 8-4 8-11 8-11-8-11-8z"/><circle cx="12" cy="12" r="3"/></svg></button>
<button class="stab" onclick="switchPage('local')" title="Local"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M20 21v-2a4 4 0 00-4-4H8a4 4 0 00-4 4v2"/><circle cx="12" cy="7" r="4"/></svg></button>
<button class="stab" onclick="switchPage('world')" title="World"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="10"/><line x1="2" y1="12" x2="22" y2="12"/><path d="M12 2a15.3 15.3 0 014 10 15.3 15.3 0 01-4 10 15.3 15.3 0 01-4-10 15.3 15.3 0 014-10z"/></svg></button>
<button class="stab" onclick="switchPage('settings')" title="Settings"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="3"/><path d="M19.4 15a1.65 1.65 0 00.33 1.82l.06.06a2 2 0 010 2.83 2 2 0 01-2.83 0l-.06-.06a1.65 1.65 0 00-1.82-.33 1.65 1.65 0 00-1 1.51V21a2 2 0 01-4 0v-.09A1.65 1.65 0 009 19.4a1.65 1.65 0 00-1.82.33l-.06.06a2 2 0 01-2.83-2.83l.06-.06A1.65 1.65 0 004.68 15a1.65 1.65 0 00-1.51-1H3a2 2 0 010-4h.09A1.65 1.65 0 004.6 9a1.65 1.65 0 00-.33-1.82l-.06-.06a2 2 0 012.83-2.83l.06.06A1.65 1.65 0 009 4.68a1.65 1.65 0 001-1.51V3a2 2 0 014 0v.09a1.65 1.65 0 001 1.51 1.65 1.65 0 001.82-.33l.06-.06a2 2 0 012.83 2.83l-.06.06A1.65 1.65 0 0019.4 9a1.65 1.65 0 001.51 1H21a2 2 0 010 4h-.09a1.65 1.65 0 00-1.51 1z"/></svg></button>
<div style="flex:1"></div>
<button class="stab" onclick="switchPage('radar')" title="Radar" id="radar-tab"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><polygon points="1 6 1 22 8 18 16 22 23 18 23 2 16 6 8 2 1 6"/><line x1="8" y1="2" x2="8" y2="18"/><line x1="16" y1="6" x2="16" y2="22"/></svg><span class="badge" id="admin-badge"></span></button>
</nav>
<div class="main">
<div class="topbar">
<span class="topbar-left" id="page-title">Combat</span>
<div class="topbar-right">
<span class="sub-exp">EXPIRES: <b>LIFETIME</b></span>
<span class="conn"><span class="conn-dot" id="conn-dot"></span><span id="conn-text">Connected</span></span>
</div>
</div>
<div class="content">

<!-- COMBAT -->
<div class="page active" id="pg-combat">
<div class="subtabs" id="st-combat"><button class="subtab active" onclick="subTab('combat',0)">Aimbot</button><button class="subtab" onclick="subTab('combat',1)">SilentAim</button><button class="subtab" onclick="subTab('combat',2)">TriggerBot</button><button class="subtab" onclick="subTab('combat',3)">Auto Peek</button></div>
<div class="panels"><div class="panel" id="combat-L"></div><div class="panel" id="combat-R"></div></div>
</div>

<!-- VISUALS -->
<div class="page" id="pg-visuals">
<div class="subtabs" id="st-visuals"><button class="subtab active" onclick="subTab('visuals',0)">Players</button><button class="subtab" onclick="subTab('visuals',1)">Vehicles</button><button class="subtab" onclick="subTab('visuals',2)">Others</button></div>
<div class="panels"><div class="panel" id="visuals-L"></div><div class="panel" id="visuals-R"></div></div>
</div>

<!-- LOCAL -->
<div class="page" id="pg-local">
<div class="subtabs" id="st-local"><button class="subtab active" onclick="subTab('local',0)">Main</button><button class="subtab" onclick="subTab('local',1)">Vehicles</button><button class="subtab" onclick="subTab('local',2)">Teleports</button><button class="subtab" onclick="subTab('local',3)">Weapons</button></div>
<div class="panels"><div class="panel" id="local-L"></div><div class="panel" id="local-R"></div></div>
</div>

<!-- WORLD -->
<div class="page" id="pg-world">
<div class="subtabs" id="st-world"><button class="subtab active" onclick="subTab('world',0)">Players</button><button class="subtab" onclick="subTab('world',1)">Vehicles</button></div>
<div class="panels"><div class="panel" id="world-content" style="flex:1"></div></div>
</div>

<!-- SETTINGS -->
<div class="page" id="pg-settings">
<div class="subtabs"><button class="subtab active">Main</button></div>
<div class="panels"><div class="panel" id="settings-L"></div><div class="panel" id="settings-R"></div></div>
</div>

<!-- RADAR -->
<div class="page" id="pg-radar">
<div id="map-wrap">
<div class="admin-alert" id="admin-alert"></div>
<div class="map-styles"><button class="ms-btn active" onclick="setMapStyle('atlas',this)">Atlas</button><button class="ms-btn" onclick="setMapStyle('satellite',this)">Satellite</button><button class="ms-btn" onclick="setMapStyle('grid',this)">Topo</button></div>
<div class="map-hud" id="map-hud">
<div class="map-hud-title">LOCAL PLAYER</div>
<div class="hud-row"><span class="hud-item"><span class="hud-lbl">X</span><span class="hud-val" id="hud-x">--</span></span><span class="hud-item"><span class="hud-lbl">Y</span><span class="hud-val" id="hud-y">--</span></span><span class="hud-item"><span class="hud-lbl">Z</span><span class="hud-val" id="hud-z">--</span></span></div>
<div style="margin-top:3px"><span class="hud-item"><span class="hud-lbl">HP</span><span class="hud-val" id="hud-hp">--</span></span><div class="hud-bar"><div class="hud-bar-fill hud-bar-hp" id="bar-hp" style="width:0"></div></div></div>
<div style="margin-top:2px"><span class="hud-item"><span class="hud-lbl">ARM</span><span class="hud-val" id="hud-arm">--</span></span><div class="hud-bar"><div class="hud-bar-fill hud-bar-arm" id="bar-arm" style="width:0"></div></div></div>
</div>
<div class="map-ctrls"><button class="mbtn" onclick="gtaMap&&gtaMap.zoomIn()">+</button><button class="mbtn" onclick="gtaMap&&gtaMap.zoomOut()">-</button><button class="mbtn" onclick="centerOnSelf()" title="Center" style="font-size:11px">O</button></div>
<div class="map-chips">
<div class="fchip on" onclick="toggleFilter(this,'admins')"><span class="fdot" style="background:#ef4444"></span>Admins (<span id="fc-adm">0</span>)</div>
<div class="fchip on" onclick="toggleFilter(this,'players')"><span class="fdot" style="background:#38bdf8"></span>Players (<span id="fc-ply">0</span>)</div>
<div class="fchip on" onclick="toggleFilter(this,'vehicles')"><span class="fdot" style="background:#a855f7"></span>Vehicles (<span id="fc-veh">0</span>)</div>
<div class="fchip" onclick="toggleFollow(this)" id="chip-follow">Follow: OFF</div>
</div>
<div class="wp-card" id="wp-card">
<div class="wp-title">WAYPOINT</div>
<div class="wp-coords" id="wp-coords">X: 0, Y: 0</div>
<div class="wp-btns"><button class="wp-tp" onclick="teleportWP()">TELEPORT</button><button class="wp-clear" onclick="clearWP()">Clear</button></div>
</div>
<div id="gta-map"></div>
</div>
</div>

</div></div>
<footer><div class="uinfo"><div class="avatar"></div><span class="uname">p4m</span></div><span class="ftime" id="clock"></span></footer>

<script src="https://cdnjs.cloudflare.com/ajax/libs/leaflet/1.9.4/leaflet.min.js"></script>
<script>
let cfg={},blipData={},gtaMap=null,tileLayers={},currentStyle='atlas';
let layerGroups={},selfMarker=null,wpMarker=null,wpData=null;
let followSelf=false,filters={admins:true,players:true,vehicles:true};
let curPage='combat',subTabs={combat:0,visuals:0,local:0,world:0};

const TITLES={combat:'Combat',visuals:'Visuals',local:'Local',world:'World',settings:'Settings',radar:'Radar Map'};
const TAB_IDX={combat:0,visuals:1,local:2,world:3,settings:4,radar:5};

const TELEPORTS=[
{name:'Waypoint',x:156,y:-1043,z:29.3},
{name:'Square',x:156.2,y:-1043.2,z:29.3},
{name:'Pier',x:-1847.7,y:-1223.4,z:13.9},
{name:'Paleto Bay',x:-397.6,y:6047.6,z:32.2},
{name:'Central Bank',x:221.8,y:217.3,z:106.7},
{name:'Cassino',x:885.3,y:16.8,z:80.7},
{name:'Los Santos Airport',x:-975.5,y:-2880.9,z:16.3},
{name:'Sandy Shores',x:1681.5,y:3251.9,z:40.8},
];

const LOCS=[
{name:'Legion Square',x:215.4,y:-810.2,z:30.7},{name:'LS Airport',x:-1037,y:-2737,z:20.1},
{name:'Maze Bank Tower',x:-75,y:-818.9,z:326.2},{name:'Diamond Casino',x:925.3,y:46.2,z:80.9},
{name:'Vinewood Sign',x:711.4,y:1198.1,z:348.5},{name:'Del Perro Pier',x:-1655.2,y:-1032.1,z:13.1},
{name:'Grove Street',x:105.9,y:-1940.2,z:20.8},{name:'Fort Zancudo',x:-2047.4,y:3132.1,z:32.8},
{name:'Sandy Shores',x:1747,y:3273.7,z:41.1},{name:'Humane Labs',x:3619.7,y:3743.6,z:28.6},
{name:'Mount Chiliad',x:501.8,y:5604.5,z:797.9},{name:'Paleto Bay',x:-446.1,y:6013.8,z:31.7},
{name:'Observatory',x:-425,y:1123.5,z:325.9},{name:'Port of LS',x:101.3,y:-2970,z:6},
];

function switchPage(id){
  curPage=id;
  document.querySelectorAll('.page').forEach(p=>p.classList.remove('active'));
  document.getElementById('pg-'+id).classList.add('active');
  document.querySelectorAll('.sidebar .stab').forEach(b=>b.classList.remove('active'));
  document.querySelectorAll('.sidebar .stab')[TAB_IDX[id]].classList.add('active');
  document.getElementById('page-title').textContent=TITLES[id]||id;
  if(id==='radar'&&gtaMap)setTimeout(()=>gtaMap.invalidateSize(),50);
  if(id!=='radar')renderPage();
}
function subTab(page,i){
  subTabs[page]=i;
  const tabs=document.getElementById('st-'+page);
  if(tabs)tabs.querySelectorAll('.subtab').forEach((b,j)=>b.classList.toggle('active',j===i));
  renderPage();
}

function tog(label,key,ac){
  const w=ac?'<span class="cwarn">AC</span>':'';
  return '<div class="ctrl"><span class="clbl">'+label+' '+w+'</span><label class="switch"><input type="checkbox" id="c-'+key+'" onchange="setVal(\''+key+'\',this.checked)"><span class="sw-s"></span></label></div>';
}
function sld(label,key,min,max,step,suffix){
  step=step||1;suffix=suffix||'';
  return '<div class="rng"><div class="rng-h"><span>'+label+'</span><b id="v-'+key+'">--</b></div><input type="range" min="'+min+'" max="'+max+'" step="'+step+'" id="c-'+key+'" oninput="document.getElementById(\'v-'+key+'\').textContent=this.value+\''+suffix+'\'" onchange="setVal(\''+key+'\',this.value)"></div>';
}

function renderPage(){
  const p=curPage,s=subTabs[p]||0;
  const L=document.getElementById(p+'-L'),R=document.getElementById(p+'-R');
  if(!L)return;

  if(p==='combat'){
    if(s===0){
      L.innerHTML='<div class="ptitle">AIMBOT</div>'+tog('Enable','aimbot_enabled')+tog('Only Visible','aimbot_visible')+tog('Ignore NPCs','aimbot_ignore_npcs')+tog('Show Fov','aimbot_showfov');
      R.innerHTML='<div class="ptitle">SETTINGS</div>'+sld('Field of View','aimbot_fov',0,400)+sld('Smooth','aimbot_speed',0,100)+sld('Max Distance','aimbot_distance',0,1000,1,'m');
    }else if(s===1){
      L.innerHTML='<div class="ptitle">SILENT AIM</div>'+tog('Enable','silentaim_enabled')+tog('Magic Bullets','silentaim_magic')+tog('Only Visible','silentaim_visible')+tog('Ignore NPCs','silentaim_ignore_npcs')+tog('Show Fov','silentaim_showfov');
      R.innerHTML='<div class="ptitle">SETTINGS</div>'+sld('Field of View','silentaim_fov',0,400)+sld('Miss Chance','silentaim_miss',0,100)+sld('Max Distance','silentaim_distance',0,1000,1,'m');
    }else if(s===2){
      L.innerHTML='<div class="ptitle">TRIGGERBOT</div>'+tog('Enable','triggerbot_enabled')+tog('Smart Triggerbot','triggerbot_smart')+tog('Only Visible','triggerbot_visible')+tog('Ignore NPCs','triggerbot_ignore_npcs')+tog('Show Fov','triggerbot_showfov');
      R.innerHTML='<div class="ptitle">SETTINGS</div>'+sld('Field of View','triggerbot_fov',0,400)+sld('Max Distance','triggerbot_distance',0,1000,1,'m')+sld('Delay','triggerbot_delay',0,10);
    }else{
      L.innerHTML='<div class="ptitle">AUTO PEEK</div>'+tog('Enable','autopeek');
      R.innerHTML='<div class="ptitle">INFO</div><p class="info-text">Press the peek key once to save your position. Peek around corners, then press again to teleport back.</p><p class="info-text">Configure the peek keybind in the overlay menu.</p>';
    }
  }else if(p==='visuals'){
    if(s===0){
      L.innerHTML='<div class="ptitle">PLAYER ESP</div>'+tog('Enable','esp_enabled')+tog('Admin ESP','esp_admin')+tog('Admin Multi-Color','esp_admin_rainbow')+tog('Box','esp_box')+tog('Skeleton','esp_skeleton')+tog('SnapLines','esp_snaplines')+tog('Head Circle','esp_headcircle')+tog('Weapon Names','esp_weapon')+tog('Player Names','esp_names')+tog('Distance','esp_distance')+tog('Health Bar','esp_health')+tog('Armor Bar','esp_armor');
      R.innerHTML='<div class="ptitle">PREVIEW</div><p class="info-text">ESP preview is available in the overlay menu. Toggle options here and they sync to the game in real-time.</p>';
    }else if(s===1){
      L.innerHTML='<div class="ptitle">VEHICLE ESP</div>'+tog('Toggle','vehesp_enabled')+tog('Vehicle Names','vehesp_names')+tog('Locked/Unlocked','vehesp_lockunlock')+tog('SnapLines','vehesp_snaplines')+tog('Distance','vehesp_distance');
      R.innerHTML='<div class="ptitle">SETTINGS</div>'+sld('Max Distance','vehesp_max_distance',0,1000,1,'m');
    }else{
      L.innerHTML='<div class="ptitle">VISUAL SETTINGS</div>'+tog('Show LocalPlayer','esp_showlocal')+tog('Ignore Dead','esp_ignore_dead')+tog('Ignore NPCs','esp_ignore_npcs')+tog('Friend Marker','esp_friends')+sld('Max Distance','esp_max_distance',0,1000,1,'m')+tog('Web Map Radar','web_enabled');
      R.innerHTML='<div class="ptitle">COLORS</div><p class="info-text">Color customization is available in the overlay menu. Changes to ESP colors apply in real-time through the in-game interface.</p>';
    }
  }else if(p==='local'){
    if(s===0){
      L.innerHTML='<div class="ptitle">PLAYER</div>'+tog('GodMode','godmode',1)+tog('NoClip','noclip',1)+tog('Fast Run','fastrun',1)+tog('Anti Headshot','antihs',1)+tog('Shrink','shrink',1)+tog('No RagDoll','noragdoll')+tog('Steal Car','stealcar')+tog('Infinite CombatRoll','combatroll')+tog('Force Weapon Wheel','weaponwheel');
      R.innerHTML='<div class="ptitle">SPEED</div>'+sld('NoClip Speed','noclip_speed',0.1,20,0.1,'m/s')+sld('Run Speed','fastrun_speed',0.1,10,0.1,'m/s');
    }else if(s===1){
      L.innerHTML='<div class="ptitle">VEHICLE</div>'+tog('GodMode','veh_godmode')+tog('SeatBelt','seatbelt');
      R.innerHTML='<div class="ptitle">ACTIONS</div><p class="info-text">Doors Lock/Unlock, Repair Vehicle, and Handling Editor are available in the overlay menu while in a vehicle.</p>';
    }else if(s===2){
      const tpH=TELEPORTS.map(t=>'<div class="tp-row"><div><div class="tp-name">'+esc(t.name)+'</div><div class="tp-coords">'+t.x+', '+t.y+', '+t.z+'</div></div><button class="tp-go" onclick="doTP('+t.x+','+t.y+','+t.z+')">GO</button></div>').join('');
      L.innerHTML='<div class="ptitle">LOCATIONS</div><div class="tp-list">'+tpH+'</div>';
      R.innerHTML='<div class="ptitle">TELEPORT</div><p class="info-text">Click GO to teleport instantly. Use the Radar Map tab to click anywhere on the GTA V map to set a waypoint and teleport.</p>';
    }else{
      L.innerHTML='<div class="ptitle">WEAPONS</div>'+tog('Recoil Control','norecoil')+tog('Spread Control','nospread')+tog('No Reload','noreload')+tog('Infinite Ammo','ammo');
      R.innerHTML='<div class="ptitle">INFO</div><p class="info-text">Recoil/Spread value sliders and Ammo management are available in the overlay menu. Weapon spawn requires the in-game overlay.</p>';
    }
  }else if(p==='settings'){
    const sL=document.getElementById('settings-L'),sR=document.getElementById('settings-R');
    sL.innerHTML='<div class="ptitle">GENERAL</div>'+tog('WaterMark','watermark')+tog('Stream Proof','streamproof');
    sR.innerHTML='<div class="ptitle">WEB / CONFIG</div>'+tog('Web Map Radar Server','web_enabled')+tog('Render Cloud Sync','web_cloudsync')+'<p class="info-text" style="margin-top:8px">Menu Key, Panic Key, Menu Color, Export/Import Config are available in the overlay menu.</p>';
  }
  syncCfgUI();
  if(p==='world')renderWorld();
}

function renderWorld(){
  const wc=document.getElementById('world-content');
  const s=subTabs.world||0;
  if(s===0){
    const pls=blipData.players||[];
    let rows=pls.map(p=>{
      const badge=p.is_admin?'<span class="badge-admin">[Admin]</span>':'<span class="badge-player">Player</span>';
      return '<tr><td>'+badge+'</td><td style="font-weight:600">'+esc(p.name)+'</td><td>'+Math.round(p.dist||0)+'m</td><td>'+Math.round(p.health||0)+'</td><td>'+Math.round(p.armor||0)+'</td><td style="font-size:9px;color:var(--dim)">'+esc(p.weapon||'Unarmed')+'</td><td><button class="tbtn" onclick="doTP('+p.x+','+p.y+','+p.z+')">TP</button></td></tr>';
    }).join('');
    if(!rows)rows='<tr><td colspan="7" style="color:var(--dim);text-align:center;padding:16px">No players detected</td></tr>';
    wc.innerHTML='<div class="ptitle">PLAYERS ('+pls.length+')</div><table class="dtable"><thead><tr><th>Type</th><th>Name</th><th>Dist</th><th>HP</th><th>Armor</th><th>Weapon</th><th></th></tr></thead><tbody>'+rows+'</tbody></table>';
  }else{
    const vhs=blipData.vehicles||[];
    let rows=vhs.map(v=>'<tr><td style="font-weight:600">'+esc(v.name)+'</td><td>'+Math.round(v.dist||0)+'m</td><td>'+(v.has_driver?'Yes':'No')+'</td><td><button class="tbtn" onclick="doTP('+v.x+','+v.y+','+(v.z||30)+')">TP</button></td></tr>').join('');
    if(!rows)rows='<tr><td colspan="4" style="color:var(--dim);text-align:center;padding:16px">No vehicles detected</td></tr>';
    wc.innerHTML='<div class="ptitle">VEHICLES ('+vhs.length+')</div><table class="dtable"><thead><tr><th>Name</th><th>Dist</th><th>Driver</th><th></th></tr></thead><tbody>'+rows+'</tbody></table>';
  }
}

function setVal(k,v){fetch('/api/set?'+k+'='+v).catch(()=>{});}
function doTP(x,y,z){fetch('/api/teleport?x='+x+'&y='+y+'&z='+(z||30)).catch(()=>{});}

function syncCfgUI(){
  for(const[k,v]of Object.entries(cfg)){
    const el=document.getElementById('c-'+k);
    if(!el)continue;
    if(el.type==='checkbox')el.checked=!!v;
    else{el.value=v;const vEl=document.getElementById('v-'+k);if(vEl)vEl.textContent=v;}
  }
}

function fetchConfig(){
  fetch('/api/config').then(r=>r.json()).then(d=>{cfg=d;syncCfgUI();}).catch(()=>{});
}

function pollBlips(){
  fetch('/api/blips').then(r=>r.json()).then(d=>{
    blipData=d;
    updateMap();
    if(curPage==='world')renderWorld();
  }).catch(()=>{});
}

function createCRS(){
  return Object.assign({},L.CRS.Simple,{
    projection:L.Projection.LonLat,
    scale(z){return Math.pow(2,z)},
    zoom(s){return Math.log(s)/Math.LN2},
    distance(a,b){return Math.hypot(b.lng-a.lng,b.lat-a.lat)},
    transformation:new L.Transformation(0.02072,117.3,-0.0205,172.8),
    infinite:true
  });
}

function initMap(){
  if(gtaMap)return;
  const crs=createCRS();
  tileLayers={
    atlas:L.tileLayer('/mapStyles/styleAtlas/{z}/{x}/{y}.jpg',{minZoom:0,maxZoom:5,noWrap:true,errorTileUrl:'/mapStyles/styleAtlas/empty.jpg'}),
    satellite:L.tileLayer('/mapStyles/styleSatelite/{z}/{x}/{y}.jpg',{minZoom:0,maxZoom:5,noWrap:true,errorTileUrl:'/mapStyles/styleAtlas/empty.jpg'}),
    grid:L.tileLayer('/mapStyles/styleGrid/{z}/{x}/{y}.png',{minZoom:0,maxZoom:5,noWrap:true,errorTileUrl:'/mapStyles/styleAtlas/empty.jpg'})
  };
  gtaMap=L.map('gta-map',{
    crs,minZoom:1,maxZoom:5,zoom:3,center:[200,0],
    layers:[tileLayers.atlas],zoomControl:false,preferCanvas:true,
    maxBounds:L.latLngBounds(L.latLng(-4600,-6000),L.latLng(8600,6800)),
    maxBoundsViscosity:0.9
  });
  layerGroups={admins:L.layerGroup().addTo(gtaMap),players:L.layerGroup().addTo(gtaMap),vehicles:L.layerGroup().addTo(gtaMap),pois:L.layerGroup().addTo(gtaMap)};

  LOCS.forEach(loc=>{
    const ic=L.divIcon({className:'',html:'<div class="gta-poi"></div>',iconSize:[4,4],iconAnchor:[2,2]});
    const m=L.marker([loc.y,loc.x],{icon:ic}).addTo(layerGroups.pois);
    m.bindPopup('<div class="pop-title">'+esc(loc.name)+'</div><div class="pop-info">X: '+loc.x.toFixed(1)+', Y: '+loc.y.toFixed(1)+'</div><button class="pop-btn" onclick="doTP('+loc.x+','+loc.y+','+loc.z+')">Teleport</button>');
  });

  gtaMap.on('click',e=>{
    wpData={x:e.latlng.lng,y:e.latlng.lat,z:30};
    const ic=L.divIcon({className:'',html:'<div class="gta-wp"></div>',iconSize:[12,12],iconAnchor:[6,6]});
    if(wpMarker)wpMarker.setLatLng(e.latlng);
    else wpMarker=L.marker(e.latlng,{icon:ic}).addTo(gtaMap);
    document.getElementById('wp-card').style.display='flex';
    document.getElementById('wp-coords').textContent='X: '+wpData.x.toFixed(1)+' Y: '+wpData.y.toFixed(1);
  });
  gtaMap.on('dragstart',()=>{followSelf=false;updateFollowUI();});
}

function setMapStyle(style,btn){
  if(!gtaMap||!tileLayers[style])return;
  gtaMap.removeLayer(tileLayers[currentStyle]);
  tileLayers[style].addTo(gtaMap);
  currentStyle=style;
  document.querySelectorAll('.ms-btn').forEach(b=>b.classList.remove('active'));
  if(btn)btn.classList.add('active');
}

function updateMap(){
  if(!gtaMap)return;
  const loc=blipData.local;
  const pls=blipData.players||[];
  const vhs=blipData.vehicles||[];
  const admins=pls.filter(p=>p.is_admin);

  if(loc){
    document.getElementById('hud-x').textContent=loc.x.toFixed(1);
    document.getElementById('hud-y').textContent=loc.y.toFixed(1);
    document.getElementById('hud-z').textContent=(loc.z||0).toFixed(1);
    document.getElementById('hud-hp').textContent=Math.round(loc.health||0);
    document.getElementById('hud-arm').textContent=Math.round(loc.armor||0);
    document.getElementById('bar-hp').style.width=Math.min(100,(loc.health||0)/2)+'%';
    document.getElementById('bar-arm').style.width=Math.min(100,loc.armor||0)+'%';
  }

  document.getElementById('fc-adm').textContent=admins.length;
  document.getElementById('fc-ply').textContent=pls.length;
  document.getElementById('fc-veh').textContent=vhs.length;

  const aa=document.getElementById('admin-alert'),ab=document.getElementById('admin-badge');
  if(admins.length>0){
    aa.style.display='block';
    aa.textContent=admins.length+' ADMIN(S) -- '+admins.map(a=>a.name).join(', ');
    ab.style.display='block';
  }else{aa.style.display='none';ab.style.display='none';}

  if(loc){
    const ic=L.divIcon({className:'',html:'<div class="gta-self"></div><div class="marker-label" style="color:#f59e0b">YOU</div>',iconSize:[14,14],iconAnchor:[7,7]});
    if(selfMarker)selfMarker.setLatLng([loc.y,loc.x]);
    else selfMarker=L.marker([loc.y,loc.x],{icon:ic,zIndexOffset:1000}).addTo(gtaMap);
    if(followSelf)gtaMap.panTo([loc.y,loc.x],{animate:true});
  }

  layerGroups.admins.clearLayers();
  layerGroups.players.clearLayers();
  if(filters.admins||filters.players){
    pls.forEach(p=>{
      if(p.is_admin&&!filters.admins)return;
      if(!p.is_admin&&!filters.players)return;
      const isA=p.is_admin;
      const cls=isA?'gta-admin':'gta-player';
      const lbl=isA?'[ADMIN] '+p.name:p.name;
      const col=isA?'#ef4444':'#38bdf8';
      const sz=isA?12:8;
      const ic=L.divIcon({className:'',html:'<div class="'+cls+'"></div><div class="marker-label" style="color:'+col+'">'+esc(lbl)+'</div>',iconSize:[sz,sz],iconAnchor:[sz/2,sz/2]});
      const m=L.marker([p.y,p.x],{icon:ic,zIndexOffset:isA?900:500});
      m.bindPopup('<div class="pop-title">'+(isA?'[ADMIN] ':'')+esc(p.name)+'</div><div class="pop-info">ID #'+p.id+' | '+Math.round(p.dist||0)+'m<br>HP: '+Math.round(p.health||0)+' | ARM: '+Math.round(p.armor||0)+'<br>'+esc(p.weapon||'Unarmed')+'</div><button class="pop-btn" onclick="doTP('+p.x+','+p.y+','+p.z+')">Teleport</button>');
      m.addTo(isA?layerGroups.admins:layerGroups.players);
    });
  }

  layerGroups.vehicles.clearLayers();
  if(filters.vehicles){
    vhs.forEach(v=>{
      const ic=L.divIcon({className:'',html:'<div class="gta-vehicle"></div><div class="marker-label" style="color:#a855f7">'+esc(v.name)+'</div>',iconSize:[7,7],iconAnchor:[3,3]});
      const m=L.marker([v.y,v.x],{icon:ic});
      m.bindPopup('<div class="pop-title">'+esc(v.name)+'</div><div class="pop-info">'+Math.round(v.dist||0)+'m | Driver: '+(v.has_driver?'Yes':'No')+'</div><button class="pop-btn" onclick="doTP('+v.x+','+v.y+','+(v.z||30)+')">Teleport</button>');
      m.addTo(layerGroups.vehicles);
    });
  }
}

function centerOnSelf(){
  if(!gtaMap||!blipData.local)return;
  gtaMap.flyTo([blipData.local.y,blipData.local.x],4,{duration:.3});
}
function toggleFilter(el,name){filters[name]=!filters[name];el.classList.toggle('on',filters[name]);updateMap();}
function toggleFollow(el){followSelf=!followSelf;updateFollowUI();if(followSelf)centerOnSelf();}
function updateFollowUI(){const el=document.getElementById('chip-follow');el.textContent='Follow: '+(followSelf?'ON':'OFF');el.classList.toggle('on',followSelf);}
function teleportWP(){if(wpData)doTP(wpData.x,wpData.y,wpData.z);}
function clearWP(){wpData=null;if(wpMarker&&gtaMap){gtaMap.removeLayer(wpMarker);wpMarker=null;}document.getElementById('wp-card').style.display='none';}
function esc(s){return String(s).replace(/&/g,'&amp;').replace(/</g,'&lt;').replace(/>/g,'&gt;').replace(/"/g,'&quot;');}

function updateClock(){
  const d=new Date();
  document.getElementById('clock').textContent=d.toLocaleDateString('en-US',{weekday:'short'})+' '+d.toLocaleTimeString('en-US',{hour:'2-digit',minute:'2-digit'});
}

window.addEventListener('DOMContentLoaded',()=>{
  renderPage();
  fetchConfig();
  pollBlips();
  updateClock();
  setInterval(pollBlips,300);
  setInterval(updateClock,10000);
  setTimeout(initMap,200);
});
</script>
</body>
</html>

)WEBAPP";
			return page;
		}
	}
}