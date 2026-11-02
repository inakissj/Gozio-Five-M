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
:root{--bg:#0a0b0f;--bg2:#111318;--bg3:#181b22;--bg4:#1e2029;--text:#e8eaed;--white:#fff;--dim:#62656e;--dim2:#44474f;--border:#1c1f27;--border2:#252830}
*{box-sizing:border-box;margin:0;padding:0}
body{font-family:'Segoe UI',system-ui,-apple-system,sans-serif;background:var(--bg);color:var(--text);height:100vh;overflow:hidden;-webkit-font-smoothing:antialiased;font-size:12.5px}
#gta-map{width:100%;height:100%;position:absolute;inset:0;background:#0a1020;z-index:0}
.leaflet-container{background:#0a1020!important;font-family:inherit}
.leaflet-control-zoom{display:none!important}
.leaflet-popup-content-wrapper{background:var(--bg3)!important;color:var(--text)!important;border-radius:6px!important;border:1px solid var(--border)!important;box-shadow:0 4px 16px rgba(0,0,0,.6)!important}
.leaflet-popup-tip{background:var(--bg3)!important;border:1px solid var(--border)!important}
.leaflet-popup-content{margin:8px 10px!important;font-size:10px!important}
.leaflet-popup-close-button{color:var(--dim)!important}

/* Map HUD overlays */
.map-hud{position:fixed;top:12px;left:12px;z-index:1000;background:rgba(10,11,15,.92);backdrop-filter:blur(12px);border:1px solid var(--border);border-radius:8px;padding:10px 14px;min-width:180px}
.map-hud-title{font-size:7px;text-transform:uppercase;letter-spacing:1.2px;color:var(--dim);font-weight:700;margin-bottom:5px}
.hud-row{display:flex;gap:10px;margin-bottom:3px}
.hud-item{font-family:Consolas,monospace;font-size:10px}
.hud-lbl{color:var(--dim);font-size:8px;margin-right:2px}
.hud-val{color:var(--white)}
.hud-bar{height:2px;background:var(--bg4);border-radius:1px;margin-top:2px;overflow:hidden}
.hud-bar-fill{height:100%;border-radius:1px;transition:width .3s}
.hud-bar-hp{background:#22c55e}
.hud-bar-arm{background:#38bdf8}

.map-ctrls{position:fixed;top:12px;right:12px;z-index:1000;display:flex;flex-direction:column;gap:3px}
.mbtn{width:30px;height:30px;background:rgba(10,11,15,.92);border:1px solid var(--border);border-radius:6px;color:var(--text);font-size:15px;font-weight:700;cursor:pointer;display:flex;align-items:center;justify-content:center;transition:all .12s;backdrop-filter:blur(12px)}
.mbtn:hover{background:var(--white);color:var(--bg);border-color:var(--white)}

.map-styles{position:fixed;top:12px;left:50%;transform:translateX(-50%);z-index:1000;display:flex;background:rgba(10,11,15,.92);border:1px solid var(--border);border-radius:6px;overflow:hidden;backdrop-filter:blur(12px)}
.ms-btn{padding:5px 14px;font-size:9px;font-weight:600;color:var(--dim);cursor:pointer;border:none;background:0 0;transition:all .12s;letter-spacing:.3px}
.ms-btn:hover{color:var(--text)}
.ms-btn.active{background:var(--white);color:var(--bg)}

.map-chips{position:fixed;bottom:12px;left:12px;z-index:1000;display:flex;gap:4px;flex-wrap:wrap}
.fchip{background:rgba(10,11,15,.9);border:1px solid var(--border);padding:4px 10px;border-radius:14px;font-size:9px;cursor:pointer;display:flex;align-items:center;gap:4px;color:var(--dim);user-select:none;transition:all .12s;font-weight:600;backdrop-filter:blur(12px)}
.fchip.on{border-color:var(--dim);color:var(--text)}
.fdot{width:5px;height:5px;border-radius:50%;flex-shrink:0}

.admin-alert{position:fixed;top:50px;left:50%;transform:translateX(-50%);z-index:1001;background:#ef4444;color:#fff;padding:5px 16px;border-radius:14px;font-size:10px;font-weight:700;display:none;letter-spacing:.3px;white-space:nowrap}

/* Waypoint card */
.wp-card{position:fixed;bottom:12px;right:12px;z-index:1000;background:rgba(10,11,15,.95);border:1px solid var(--border);border-radius:8px;padding:10px 14px;display:none;flex-direction:column;gap:5px;min-width:200px;backdrop-filter:blur(12px)}
.wp-title{font-size:8px;text-transform:uppercase;letter-spacing:1px;color:var(--dim);font-weight:700}
.wp-coords{font-family:Consolas,monospace;font-size:11px;color:var(--white)}
.wp-btns{display:flex;gap:4px}
.wp-tp{background:var(--white);color:var(--bg);border:none;padding:6px;border-radius:5px;font-weight:700;font-size:10px;cursor:pointer;flex:1;letter-spacing:.3px}
.wp-tp:hover{opacity:.85}
.wp-clear{background:var(--bg4);color:var(--dim);border:1px solid var(--border);padding:3px 8px;border-radius:4px;font-size:9px;cursor:pointer;font-weight:600}

/* Location dropdown */
.loc-dropdown{position:fixed;top:12px;left:210px;z-index:1000}
.loc-btn{background:rgba(10,11,15,.92);border:1px solid var(--border);border-radius:6px;color:var(--dim);padding:5px 10px;font-size:9px;font-weight:600;cursor:pointer;display:flex;align-items:center;gap:4px;backdrop-filter:blur(12px)}
.loc-btn:hover{color:var(--text)}
.loc-list{position:absolute;top:100%;left:0;margin-top:4px;background:rgba(10,11,15,.96);border:1px solid var(--border);border-radius:8px;max-height:280px;overflow-y:auto;width:200px;display:none;backdrop-filter:blur(12px)}
.loc-list.open{display:block}
.loc-item{padding:6px 12px;font-size:10px;cursor:pointer;color:var(--dim);transition:all .1s;border-bottom:1px solid var(--border)}
.loc-item:hover{background:var(--bg3);color:var(--text)}
.loc-item:last-child{border-bottom:0}

/* Markers */
.gta-self{width:14px;height:14px;border-radius:50%;background:#f59e0b;border:2px solid #fff;box-shadow:0 0 8px #f59e0b;animation:pulse 1.5s infinite}
.gta-admin{width:12px;height:12px;border-radius:50%;background:#ef4444;border:2px solid #fff;box-shadow:0 0 8px rgba(239,68,68,.5);animation:pulse 1.2s infinite}
.gta-player{width:8px;height:8px;border-radius:50%;background:#38bdf8;border:1.5px solid rgba(255,255,255,.6);box-shadow:0 0 4px rgba(56,189,248,.4)}
.gta-vehicle{width:7px;height:7px;border-radius:2px;background:#a855f7;border:1px solid rgba(255,255,255,.4);box-shadow:0 0 4px rgba(168,85,247,.4)}
.gta-wp{width:12px;height:12px;border-radius:50%;background:#22c55e;border:2px solid #fff;box-shadow:0 0 8px #22c55e}
.marker-label{position:absolute;white-space:nowrap;font-size:8px;font-weight:700;left:50%;transform:translateX(-50%);top:-14px;text-shadow:0 1px 2px #000,0 0 6px rgba(0,0,0,.8);pointer-events:none}
@keyframes pulse{0%,100%{opacity:1;transform:scale(1)}50%{opacity:.7;transform:scale(1.12)}}
.pop-title{font-weight:700;font-size:11px;margin-bottom:3px}
.pop-info{color:var(--dim);font-size:9px;line-height:1.4}

/* MENU BUTTON */
.menu-toggle{position:fixed;bottom:12px;right:12px;z-index:1100;width:44px;height:44px;border-radius:50%;background:rgba(10,11,15,.92);border:1px solid var(--border);color:var(--text);cursor:pointer;display:flex;align-items:center;justify-content:center;transition:all .2s;backdrop-filter:blur(12px)}
.menu-toggle:hover{background:var(--white);color:var(--bg);border-color:var(--white);transform:scale(1.05)}
.menu-toggle svg{width:20px;height:20px}
.menu-toggle.active{background:var(--white);color:var(--bg);border-color:var(--white)}

/* SLIDE-OUT MENU PANEL */
.menu-panel{position:fixed;top:0;right:-420px;width:420px;height:100%;background:var(--bg2);border-left:1px solid var(--border);z-index:1050;transition:right .25s cubic-bezier(.4,0,.2,1);display:flex;flex-direction:column;box-shadow:-4px 0 24px rgba(0,0,0,.5)}
.menu-panel.open{right:0}
.menu-header{padding:14px 16px;border-bottom:1px solid var(--border);display:flex;align-items:center;justify-content:space-between;flex-shrink:0}
.menu-header-left{display:flex;align-items:center;gap:10px}
.menu-logo{width:26px;height:26px;border-radius:5px;background:var(--white);display:flex;align-items:center;justify-content:center;font-weight:900;font-size:8px;color:var(--bg);letter-spacing:-.3px}
.menu-title-text{font-size:11px;font-weight:700;letter-spacing:.5px;text-transform:uppercase;color:var(--text)}
.menu-close{background:none;border:1px solid var(--border);border-radius:5px;width:28px;height:28px;cursor:pointer;color:var(--dim);display:flex;align-items:center;justify-content:center;transition:all .12s;font-size:14px}
.menu-close:hover{background:var(--bg3);color:var(--text)}
.menu-nav{display:flex;gap:2px;padding:8px 12px;flex-shrink:0;border-bottom:1px solid var(--border);flex-wrap:wrap}
.menu-nav-btn{padding:5px 12px;border-radius:5px;font-size:10px;font-weight:600;cursor:pointer;color:var(--dim);border:1px solid transparent;background:none;transition:all .12s;letter-spacing:.2px}
.menu-nav-btn:hover{color:var(--text);background:var(--bg3)}
.menu-nav-btn.active{background:var(--white);color:var(--bg);border-color:var(--white)}
.menu-subtabs{display:flex;gap:3px;padding:8px 16px 4px;flex-shrink:0}
.menu-subtab{background:var(--bg3);border:1px solid var(--border);color:var(--dim);padding:4px 10px;border-radius:5px;font-size:9.5px;font-weight:600;cursor:pointer;transition:all .12s;letter-spacing:.2px}
.menu-subtab:hover{color:var(--text);border-color:var(--dim2)}
.menu-subtab.active{background:var(--white);color:var(--bg);border-color:var(--white)}
.menu-body{flex:1;overflow-y:auto;padding:10px 16px}
.menu-body::-webkit-scrollbar{width:3px}
.menu-body::-webkit-scrollbar-thumb{background:var(--dim2);border-radius:3px}
.menu-cols{display:flex;gap:10px}
.menu-col{flex:1;display:flex;flex-direction:column;gap:0}
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
.tp-list{display:flex;flex-direction:column;gap:3px}
.tp-row{display:flex;align-items:center;justify-content:space-between;padding:7px 10px;background:var(--bg3);border:1px solid var(--border);border-radius:6px}
.tp-name{font-weight:600;font-size:11.5px}
.tp-coords{font-size:9px;color:var(--dim);font-family:Consolas,monospace}
.tp-go{background:var(--white);color:var(--bg);border:none;padding:3px 12px;border-radius:4px;font-size:10px;font-weight:700;cursor:pointer;transition:all .12s}
.tp-go:hover{opacity:.85}
.btn-action{background:var(--bg3);border:1px solid var(--border);color:var(--text);padding:6px;border-radius:5px;font-size:11px;font-weight:600;cursor:pointer;width:100%;transition:all .12s;margin-top:4px}
.btn-action:hover{background:var(--white);color:var(--bg);border-color:var(--white)}
.dtable{width:100%;border-collapse:collapse;font-size:11px}
.dtable th{text-align:left;font-size:8px;text-transform:uppercase;letter-spacing:.7px;color:var(--dim);font-weight:700;padding:5px 6px;border-bottom:1px solid var(--border)}
.dtable td{padding:5px 6px;border-bottom:1px solid var(--border);color:var(--text)}
.dtable tr:hover td{background:rgba(255,255,255,.02)}
.badge-admin{color:#ef4444;font-size:9px;font-weight:700}
.badge-player{color:var(--dim);font-size:9px;font-weight:600}

/* Status bar at bottom */
.status-bar{position:fixed;bottom:0;left:0;right:0;height:28px;background:rgba(10,11,15,.92);border-top:1px solid var(--border);display:flex;align-items:center;justify-content:space-between;padding:0 12px;z-index:1000;backdrop-filter:blur(12px)}
.status-left{display:flex;align-items:center;gap:12px}
.status-item{font-size:9px;color:var(--dim);font-weight:600;display:flex;align-items:center;gap:4px}
.status-dot{width:5px;height:5px;border-radius:50%;background:#22c55e}
.status-right{font-size:9px;color:var(--dim2)}
</style>
</head>
<body>

<!-- FULL SCREEN MAP -->
<div id="gta-map"></div>

<!-- HUD overlay top-left -->
<div class="map-hud" id="map-hud">
<div class="map-hud-title">LOCAL PLAYER</div>
<div class="hud-row"><span class="hud-item"><span class="hud-lbl">X</span><span class="hud-val" id="hud-x">--</span></span><span class="hud-item"><span class="hud-lbl">Y</span><span class="hud-val" id="hud-y">--</span></span><span class="hud-item"><span class="hud-lbl">Z</span><span class="hud-val" id="hud-z">--</span></span></div>
<div style="margin-top:3px"><span class="hud-item"><span class="hud-lbl">HP</span><span class="hud-val" id="hud-hp">--</span></span><div class="hud-bar"><div class="hud-bar-fill hud-bar-hp" id="bar-hp" style="width:0"></div></div></div>
<div style="margin-top:2px"><span class="hud-item"><span class="hud-lbl">ARM</span><span class="hud-val" id="hud-arm">--</span></span><div class="hud-bar"><div class="hud-bar-fill hud-bar-arm" id="bar-arm" style="width:0"></div></div></div>
</div>

<!-- Map style selector -->
<div class="map-styles"><button class="ms-btn active" onclick="setMapStyle('atlas',this)">Atlas</button><button class="ms-btn" onclick="setMapStyle('satellite',this)">Satellite</button><button class="ms-btn" onclick="setMapStyle('grid',this)">Topo</button></div>

<!-- Location dropdown -->
<div class="loc-dropdown">
<button class="loc-btn" onclick="toggleLocList()">
<svg width="10" height="10" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M21 10c0 7-9 13-9 13s-9-6-9-13a9 9 0 0118 0z"/><circle cx="12" cy="10" r="3"/></svg>
Locations
</button>
<div class="loc-list" id="loc-list"></div>
</div>

<!-- Zoom/center controls -->
<div class="map-ctrls">
<button class="mbtn" onclick="gtaMap&&gtaMap.zoomIn()">+</button>
<button class="mbtn" onclick="gtaMap&&gtaMap.zoomOut()">-</button>
<button class="mbtn" onclick="centerOnSelf()" title="Center" style="font-size:11px">O</button>
</div>

<!-- Admin alert -->
<div class="admin-alert" id="admin-alert"></div>

<!-- Filter chips -->
<div class="map-chips">
<div class="fchip on" onclick="toggleFilter(this,'admins')"><span class="fdot" style="background:#ef4444"></span>Admins (<span id="fc-adm">0</span>)</div>
<div class="fchip on" onclick="toggleFilter(this,'players')"><span class="fdot" style="background:#38bdf8"></span>Players (<span id="fc-ply">0</span>)</div>
<div class="fchip on" onclick="toggleFilter(this,'vehicles')"><span class="fdot" style="background:#a855f7"></span>Vehicles (<span id="fc-veh">0</span>)</div>
<div class="fchip" onclick="toggleFollow(this)" id="chip-follow">Follow: OFF</div>
</div>

<!-- Waypoint card -->
<div class="wp-card" id="wp-card">
<div class="wp-title">WAYPOINT</div>
<div class="wp-coords" id="wp-coords">X: 0, Y: 0</div>
<div class="wp-btns"><button class="wp-tp" onclick="teleportWP()">TELEPORT</button><button class="wp-clear" onclick="clearWP()">Clear</button></div>
</div>

<!-- MENU TOGGLE BUTTON -->
<button class="menu-toggle" id="menu-toggle" onclick="toggleMenu()">
<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="3"/><path d="M19.4 15a1.65 1.65 0 00.33 1.82l.06.06a2 2 0 010 2.83 2 2 0 01-2.83 0l-.06-.06a1.65 1.65 0 00-1.82-.33 1.65 1.65 0 00-1 1.51V21a2 2 0 01-4 0v-.09A1.65 1.65 0 009 19.4a1.65 1.65 0 00-1.82.33l-.06.06a2 2 0 01-2.83-2.83l.06-.06A1.65 1.65 0 004.68 15a1.65 1.65 0 00-1.51-1H3a2 2 0 010-4h.09A1.65 1.65 0 004.6 9a1.65 1.65 0 00-.33-1.82l-.06-.06a2 2 0 012.83-2.83l.06.06A1.65 1.65 0 009 4.68a1.65 1.65 0 001-1.51V3a2 2 0 014 0v.09a1.65 1.65 0 001 1.51 1.65 1.65 0 001.82-.33l.06-.06a2 2 0 012.83 2.83l-.06.06A1.65 1.65 0 0019.4 9a1.65 1.65 0 001.51 1H21a2 2 0 010 4h-.09a1.65 1.65 0 00-1.51 1z"/></svg>
</button>

<!-- SLIDE-OUT MENU PANEL -->
<div class="menu-panel" id="menu-panel">
<div class="menu-header">
<div class="menu-header-left">
<div class="menu-logo">p4</div>
<span class="menu-title-text">p4m Control Panel</span>
</div>
<button class="menu-close" onclick="toggleMenu()">X</button>
</div>
<div class="menu-nav" id="menu-nav">
<button class="menu-nav-btn active" onclick="menuPage('combat',this)">Combat</button>
<button class="menu-nav-btn" onclick="menuPage('visuals',this)">Visuals</button>
<button class="menu-nav-btn" onclick="menuPage('local',this)">Local</button>
<button class="menu-nav-btn" onclick="menuPage('world',this)">World</button>
<button class="menu-nav-btn" onclick="menuPage('exploits',this)">Exploits</button>
<button class="menu-nav-btn" onclick="menuPage('settings',this)">Settings</button>
</div>
<div class="menu-subtabs" id="menu-subtabs"></div>
<div class="menu-body" id="menu-body"></div>
</div>

<!-- Status bar -->
<div class="status-bar">
<div class="status-left">
<span class="status-item"><span class="status-dot" id="conn-dot"></span><span id="conn-text">Connected</span></span>
<span class="status-item">EXPIRES: <b style="color:var(--white);margin-left:2px">LIFETIME</b></span>
<span class="status-item" id="entity-count"></span>
</div>
<div class="status-right" id="clock"></div>
</div>

<script src="https://cdnjs.cloudflare.com/ajax/libs/leaflet/1.9.4/leaflet.min.js"></script>
<script>
let cfg={},blipData={},gtaMap=null,tileLayers={},currentStyle='atlas';
let layerGroups={},selfMarker=null,wpMarker=null,wpData=null;
let followSelf=false,filters={admins:true,players:true,vehicles:true};
let curMenuPage='combat',menuSubIdx={combat:0,visuals:0,local:0,world:0};
let menuOpen=false;

const LOCS=[
{name:'Legion Square',x:215.4,y:-810.2,z:30.7},{name:'LS Airport',x:-1037,y:-2737,z:20.1},
{name:'Maze Bank Tower',x:-75,y:-818.9,z:326.2},{name:'Diamond Casino',x:925.3,y:46.2,z:80.9},
{name:'Vinewood Sign',x:711.4,y:1198.1,z:348.5},{name:'Del Perro Pier',x:-1655.2,y:-1032.1,z:13.1},
{name:'Grove Street',x:105.9,y:-1940.2,z:20.8},{name:'Fort Zancudo',x:-2047.4,y:3132.1,z:32.8},
{name:'Sandy Shores',x:1747,y:3273.7,z:41.1},{name:'Humane Labs',x:3619.7,y:3743.6,z:28.6},
{name:'Mount Chiliad',x:501.8,y:5604.5,z:797.9},{name:'Paleto Bay',x:-446.1,y:6013.8,z:31.7},
{name:'Observatory',x:-425,y:1123.5,z:325.9},{name:'Port of LS',x:101.3,y:-2970,z:6},
];

const TELEPORTS=[
{name:'Legion Square',x:215.4,y:-810.2,z:30.7},
{name:'Del Perro Pier',x:-1655.2,y:-1032.1,z:13.1},
{name:'Paleto Bay',x:-446.1,y:6013.8,z:31.7},
{name:'Central Bank',x:221.8,y:217.3,z:106.7},
{name:'Diamond Casino',x:925.3,y:46.2,z:80.9},
{name:'LS Airport',x:-1037,y:-2737,z:20.1},
{name:'Sandy Shores',x:1747,y:3273.7,z:41.1},
{name:'Fort Zancudo',x:-2047.4,y:3132.1,z:32.8},
];

// Init map
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

// Build locations dropdown
var ll=document.getElementById('loc-list');
LOCS.forEach(function(l){
var d=document.createElement('div');d.className='loc-item';d.textContent=l.name;
d.onclick=function(){gtaMap.setView([l.y,l.x],4);ll.classList.remove('open')};
ll.appendChild(d);
});
}

function setMapStyle(s,btn){
if(tileLayers[currentStyle])gtaMap.removeLayer(tileLayers[currentStyle]);
currentStyle=s;tileLayers[s].addTo(gtaMap);
document.querySelectorAll('.ms-btn').forEach(function(b){b.classList.remove('active')});
if(btn)btn.classList.add('active');
}
function toggleLocList(){document.getElementById('loc-list').classList.toggle('open')}
function centerOnSelf(){if(blipData.local)gtaMap.setView([blipData.local.y,blipData.local.x],4)}
function toggleFilter(el,key){filters[key]=!filters[key];el.classList.toggle('on')}
function toggleFollow(el){followSelf=!followSelf;el.textContent='Follow: '+(followSelf?'ON':'OFF');el.classList.toggle('on',followSelf)}

function setWaypoint(ll){
wpData={lat:ll.lat,lng:ll.lng};
if(wpMarker)gtaMap.removeLayer(wpMarker);
wpMarker=L.marker(ll,{icon:L.divIcon({className:'gta-wp',iconSize:[12,12],iconAnchor:[6,6]})}).addTo(gtaMap);
var c=document.getElementById('wp-card');c.style.display='flex';
document.getElementById('wp-coords').textContent='X: '+ll.lng.toFixed(1)+', Y: '+ll.lat.toFixed(1);
document.getElementById('menu-toggle').style.display='none';
}
function teleportWP(){if(wpData)fetch('/api/teleport?x='+wpData.lng.toFixed(1)+'&y='+wpData.lat.toFixed(1));clearWP()}
function clearWP(){
if(wpMarker){gtaMap.removeLayer(wpMarker);wpMarker=null}wpData=null;
document.getElementById('wp-card').style.display='none';
document.getElementById('menu-toggle').style.display='flex';
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
if(followSelf)gtaMap.setView([lp.y,lp.x]);
}
['players','vehicles','admins'].forEach(function(k){layerGroups[k].clearLayers()});

var ac=0,pc=0,vc=0,adminAlert=false;
if(data.players){
data.players.forEach(function(p){
if(p.is_admin){ac++;
if(filters.admins){
var m=L.marker([p.y,p.x],{icon:L.divIcon({className:'',html:'<div class="marker-label" style="color:#ef4444">'+p.name+'</div><div class="gta-admin"></div>',iconSize:[12,12],iconAnchor:[6,6]})});
m.bindPopup('<div class="pop-title" style="color:#ef4444">'+p.name+'</div><div class="pop-info">HP: '+p.health+' | Armor: '+p.armor+'<br>Weapon: '+p.weapon+'<br>Distance: '+p.dist+'m</div>');
layerGroups.admins.addLayer(m);
}adminAlert=true;
}else{pc++;
if(filters.players){
var m=L.marker([p.y,p.x],{icon:L.divIcon({className:'',html:'<div class="marker-label" style="color:#38bdf8">'+p.name+'</div><div class="gta-player"></div>',iconSize:[8,8],iconAnchor:[4,4]})});
m.bindPopup('<div class="pop-title">'+p.name+'</div><div class="pop-info">HP: '+p.health+' | Armor: '+p.armor+'<br>Weapon: '+p.weapon+'<br>Distance: '+p.dist+'m</div>');
layerGroups.players.addLayer(m);
}}});
}
if(data.vehicles){
data.vehicles.forEach(function(v){vc++;
if(filters.vehicles){
var m=L.marker([v.y,v.x],{icon:L.divIcon({className:'',html:'<div class="marker-label" style="color:#a855f7">'+v.name+'</div><div class="gta-vehicle"></div>',iconSize:[7,7],iconAnchor:[3,3]})});
m.bindPopup('<div class="pop-title" style="color:#a855f7">'+v.name+'</div><div class="pop-info">Distance: '+v.dist+'m<br>Driver: '+(v.has_driver?'Yes':'No')+'</div>');
layerGroups.vehicles.addLayer(m);
}});
}
document.getElementById('fc-adm').textContent=ac;
document.getElementById('fc-ply').textContent=pc;
document.getElementById('fc-veh').textContent=vc;
document.getElementById('entity-count').textContent=ac+' admins / '+pc+' players / '+vc+' vehicles';
var aa=document.getElementById('admin-alert');
if(adminAlert){aa.textContent=ac+' ADMIN'+(ac>1?'S':'')+' NEARBY';aa.style.display='block'}else{aa.style.display='none'}
}

// MENU
function toggleMenu(){
menuOpen=!menuOpen;
document.getElementById('menu-panel').classList.toggle('open',menuOpen);
document.getElementById('menu-toggle').classList.toggle('active',menuOpen);
if(menuOpen)renderMenuPage();
}
function menuPage(id,btn){
curMenuPage=id;
document.querySelectorAll('.menu-nav-btn').forEach(function(b){b.classList.remove('active')});
if(btn)btn.classList.add('active');
renderMenuPage();
}
function menuSubTab(idx){
menuSubIdx[curMenuPage]=idx;
renderMenuPage();
}

function tog(label,key,ac){
var w=ac?'<span class="cwarn">AC</span>':'';
return '<div class="ctrl"><span class="clbl">'+label+' '+w+'</span><label class="switch"><input type="checkbox" id="c-'+key+'" onchange="setVal(\''+key+'\',this.checked)"><span class="sw-s"></span></label></div>';
}
function sld(label,key,min,max,step,suffix){
step=step||1;suffix=suffix||'';
return '<div class="rng"><div class="rng-h"><span>'+label+'</span><b id="v-'+key+'">--</b></div><input type="range" min="'+min+'" max="'+max+'" step="'+step+'" id="c-'+key+'" oninput="document.getElementById(\'v-'+key+'\').textContent=this.value+\''+suffix+'\'" onchange="setVal(\''+key+'\',this.value)"></div>';
}

function renderMenuPage(){
var p=curMenuPage,s=menuSubIdx[p]||0;
var stDiv=document.getElementById('menu-subtabs');
var body=document.getElementById('menu-body');

// Build subtabs
var subDefs={combat:['Aimbot','SilentAim','TriggerBot','Auto Peek'],visuals:['Players','Vehicles','Others'],local:['Main','Vehicles','Teleports','Weapons'],world:['Players','Vehicles']};
var subs=subDefs[p];
if(subs){
stDiv.innerHTML=subs.map(function(n,i){return '<button class="menu-subtab'+(i===s?' active':'')+'" onclick="menuSubTab('+i+')">'+n+'</button>'}).join('');
stDiv.style.display='flex';
}else{stDiv.innerHTML='';stDiv.style.display='none'}

// Build body
var html='<div class="menu-cols"><div class="menu-col" id="mL"></div><div class="menu-col" id="mR"></div></div>';
body.innerHTML=html;
var L=document.getElementById('mL'),R=document.getElementById('mR');

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
R.innerHTML='<div class="ptitle">INFO</div><p class="info-text">Press the peek key once to save your position. Peek around corners, then press again to teleport back.</p>';
}
}else if(p==='visuals'){
if(s===0){
L.innerHTML='<div class="ptitle">PLAYER ESP</div>'+tog('Enable','esp_enabled')+tog('Admin ESP','esp_admin')+tog('Admin Multi-Color','esp_admin_rainbow')+tog('Box','esp_box')+tog('Skeleton','esp_skeleton')+tog('SnapLines','esp_snaplines')+tog('Head Circle','esp_headcircle')+tog('Weapon Names','esp_weapon')+tog('Player Names','esp_names')+tog('Distance','esp_distance')+tog('Health Bar','esp_health')+tog('Armor Bar','esp_armor');
R.innerHTML='<div class="ptitle">FILTERS</div>'+tog('Show LocalPlayer','esp_showlocal')+tog('Ignore Dead','esp_ignore_dead')+tog('Ignore NPCs','esp_ignore_npcs')+tog('Friend Marker','esp_friends')+sld('Max Distance','esp_max_distance',0,1000,1,'m');
}else if(s===1){
L.innerHTML='<div class="ptitle">VEHICLE ESP</div>'+tog('Toggle','vehesp_enabled')+tog('Vehicle Names','vehesp_names')+tog('Locked/Unlocked','vehesp_lockunlock')+tog('SnapLines','vehesp_snaplines')+tog('Distance','vehesp_distance');
R.innerHTML='<div class="ptitle">SETTINGS</div>'+sld('Max Distance','vehesp_max_distance',0,1000,1,'m');
}else{
L.innerHTML='<div class="ptitle">OTHER</div>'+tog('Web Map Radar','web_enabled');
R.innerHTML='';
}
}else if(p==='local'){
if(s===0){
L.innerHTML='<div class="ptitle">PLAYER</div>'+tog('GodMode','godmode',1)+tog('NoClip','noclip',1)+tog('Fast Run','fastrun',1)+tog('Anti Headshot','antihs',1)+tog('Shrink','shrink',1)+tog('No RagDoll','noragdoll')+tog('Steal Car','stealcar')+tog('Infinite CombatRoll','combatroll')+tog('Force Weapon Wheel','weaponwheel')+tog('No Reload','noreload')+tog('Infinite Ammo','ammo')+tog('Infinite Stamina','stamina');
R.innerHTML='<div class="ptitle">SPEED</div>'+sld('NoClip Speed','noclip_speed',0.1,20,0.1,'m/s')+sld('Run Speed','fastrun_speed',0.1,10,0.1,'m/s');
}else if(s===1){
L.innerHTML='<div class="ptitle">VEHICLE</div>'+tog('GodMode','veh_godmode')+tog('SeatBelt','seatbelt');
R.innerHTML='';
}else if(s===2){
var thtml='<div class="ptitle">TELEPORT LOCATIONS</div><div class="tp-list">';
TELEPORTS.forEach(function(t){
thtml+='<div class="tp-row"><div><div class="tp-name">'+t.name+'</div><div class="tp-coords">'+t.x.toFixed(1)+', '+t.y.toFixed(1)+', '+t.z.toFixed(1)+'</div></div><button class="tp-go" onclick="fetch(\'/api/teleport?x='+t.x+'&y='+t.y+'&z='+t.z+'\')">GO</button></div>';
});
thtml+='</div>';
L.innerHTML=thtml;R.innerHTML='';R.style.display='none';
}else{
L.innerHTML='<div class="ptitle">WEAPONS</div>'+tog('No Recoil','norecoil')+tog('No Spread','nospread');
R.innerHTML='';
}
}else if(p==='world'){
var bd=blipData;
if(s===0){
var thtml='<div class="ptitle">ONLINE PLAYERS</div>';
if(bd.players&&bd.players.length){
thtml+='<table class="dtable"><thead><tr><th>Name</th><th>Weapon</th><th>HP</th><th>Dist</th><th></th></tr></thead><tbody>';
bd.players.forEach(function(pl){
var badge=pl.is_admin?'<span class="badge-admin">ADMIN</span>':'<span class="badge-player">PLAYER</span>';
thtml+='<tr><td>'+pl.name+' '+badge+'</td><td style="font-size:9px;color:var(--dim)">'+pl.weapon.replace('WEAPON_','')+'</td><td>'+pl.health+'</td><td>'+pl.dist+'m</td><td><button class="tp-go" style="font-size:8px;padding:2px 6px" onclick="fetch(\'/api/teleport?x='+pl.x+'&y='+pl.y+'\')">TP</button></td></tr>';
});
thtml+='</tbody></table>';
}else thtml+='<p class="info-text">No players detected.</p>';
L.innerHTML=thtml;R.innerHTML='';R.style.display='none';
}else{
var thtml='<div class="ptitle">VEHICLES</div>';
if(bd.vehicles&&bd.vehicles.length){
thtml+='<table class="dtable"><thead><tr><th>Name</th><th>Driver</th><th>Dist</th></tr></thead><tbody>';
bd.vehicles.forEach(function(v){
thtml+='<tr><td>'+v.name+'</td><td>'+(v.has_driver?'Yes':'No')+'</td><td>'+v.dist+'m</td></tr>';
});
thtml+='</tbody></table>';
}else thtml+='<p class="info-text">No vehicles detected.</p>';
L.innerHTML=thtml;R.innerHTML='';R.style.display='none';
}
}else if(p==='exploits'){
L.innerHTML='<div class="ptitle">EXPLOITS</div><p class="info-text">Weapon spawn exploits are available through the overlay menu. Use the in-game interface for weapon spawning.</p>';
R.innerHTML='';
}else if(p==='settings'){
L.innerHTML='<div class="ptitle">GENERAL</div>'+tog('WaterMark','watermark')+tog('Stream Proof','streamproof');
R.innerHTML='<div class="ptitle">WEB RADAR</div>'+tog('Server','web_enabled')+tog('Remote Cloud Sync','web_cloudsync')+'<p class="info-text">Menu Key / Panic Key configuration is available in the overlay menu.</p>';
}
syncCfgUI();
}

function setVal(key,val){
var v=(val===true||val===false)?val:parseFloat(val);
cfg[key]=v;
fetch('/api/set?'+key+'='+(v===true?'true':v===false?'false':v));
}

function syncCfgUI(){
Object.keys(cfg).forEach(function(k){
var el=document.getElementById('c-'+k);
if(!el)return;
if(el.type==='checkbox')el.checked=!!cfg[k];
else if(el.type==='range'){el.value=cfg[k];var vl=document.getElementById('v-'+k);if(vl)vl.textContent=cfg[k]}
});
}

function poll(){
fetch('/api/blips').then(function(r){return r.json()}).then(function(d){updateMarkers(d)}).catch(function(){});
fetch('/api/config').then(function(r){return r.json()}).then(function(d){cfg=d;if(menuOpen)syncCfgUI()}).catch(function(){});
fetch('/api/status').then(function(r){return r.json()}).then(function(d){
var dot=document.getElementById('conn-dot'),txt=document.getElementById('conn-text');
if(d.connected){dot.style.background='#22c55e';txt.textContent='Connected'}
else{dot.style.background='#ef4444';txt.textContent='Disconnected'}
}).catch(function(){
document.getElementById('conn-dot').style.background='#ef4444';
document.getElementById('conn-text').textContent='Offline';
});
}

function updateClock(){
var d=new Date();
document.getElementById('clock').textContent=d.toLocaleTimeString();
}

document.addEventListener('click',function(e){
var ll=document.getElementById('loc-list');
if(!e.target.closest('.loc-dropdown'))ll.classList.remove('open');
});

// Keyboard shortcut: Escape to close menu
document.addEventListener('keydown',function(e){
if(e.key==='Escape'&&menuOpen)toggleMenu();
});

// Init
initMap();
poll();
setInterval(poll,350);
setInterval(updateClock,1000);
updateClock();
</script>
</body>
</html>

)WEBAPP";
			return page;
		}
	}
}