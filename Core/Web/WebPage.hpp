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
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>p4m Controller</title>
<style>
:root{--bg:#0a0b0f;--bg2:#111318;--bg3:#181b22;--bg4:#1e222b;--accent:#38bdf8;--accent2:#0ea5e9;--red:#ef4444;--green:#22c55e;--purple:#a855f7;--gold:#f59e0b;--text:#e8eaed;--dim:#6b7280;--border:#252830;--sidebar:56px;--header:48px;--footer:40px}
*{box-sizing:border-box;margin:0;padding:0}
body{font-family:-apple-system,BlinkMacSystemFont,"Segoe UI",Roboto,Helvetica,sans-serif;background:var(--bg);color:var(--text);height:100vh;overflow:hidden;display:flex;flex-direction:column;font-size:13px}
/* LAYOUT */
.app{display:flex;flex:1;overflow:hidden}
.sidebar{width:var(--sidebar);background:var(--bg2);border-right:1px solid var(--border);display:flex;flex-direction:column;align-items:center;padding:12px 0;gap:4px;z-index:10;flex-shrink:0}
.sidebar .logo{width:32px;height:32px;border-radius:8px;background:linear-gradient(135deg,var(--accent),var(--purple));display:flex;align-items:center;justify-content:center;font-weight:900;font-size:11px;color:#fff;margin-bottom:12px;letter-spacing:-0.5px}
.stab{width:40px;height:40px;border-radius:10px;display:flex;align-items:center;justify-content:center;cursor:pointer;color:var(--dim);transition:all .15s;border:none;background:transparent}
.stab:hover{background:var(--bg3);color:var(--text)}
.stab.active{background:var(--accent);color:#fff;box-shadow:0 2px 12px rgba(56,189,248,.3)}
.stab svg{width:18px;height:18px;fill:currentColor}
.main-area{flex:1;display:flex;flex-direction:column;overflow:hidden}
/* HEADER */
.top-bar{height:var(--header);background:var(--bg2);border-bottom:1px solid var(--border);display:flex;align-items:center;justify-content:space-between;padding:0 20px;flex-shrink:0}
.top-label{font-size:11px;letter-spacing:1.5px;text-transform:uppercase;color:var(--dim);font-weight:600}
.top-right{display:flex;align-items:center;gap:16px}
.status-dot{width:7px;height:7px;border-radius:50%;background:var(--green);box-shadow:0 0 6px var(--green);display:inline-block;margin-right:6px}
.sub-label{font-size:11px;color:var(--dim);letter-spacing:.5px}
.sub-label b{color:var(--text)}
/* CONTENT */
.content{flex:1;overflow:hidden;position:relative}
.page{display:none;width:100%;height:100%;flex-direction:column;overflow:hidden}
.page.active{display:flex}
/* SUBTABS */
.subtabs{display:flex;gap:6px;padding:14px 20px 10px;flex-shrink:0}
.subtab{background:var(--bg3);border:1px solid var(--border);color:var(--dim);padding:6px 16px;border-radius:6px;font-size:12px;font-weight:600;cursor:pointer;transition:all .15s}
.subtab:hover{color:var(--text);border-color:var(--accent)}
.subtab.active{background:var(--accent);color:#fff;border-color:var(--accent)}
/* PANELS */
.panels{display:flex;flex:1;gap:16px;padding:0 20px 14px;overflow-y:auto}
.panel{flex:1;background:var(--bg2);border:1px solid var(--border);border-radius:10px;padding:16px;display:flex;flex-direction:column;gap:2px;overflow-y:auto}
.panel-title{font-size:10px;text-transform:uppercase;letter-spacing:1px;color:var(--dim);font-weight:700;margin-bottom:10px;padding-bottom:8px;border-bottom:1px solid var(--border)}
/* CONTROLS */
.ctrl{display:flex;align-items:center;justify-content:space-between;padding:7px 0;min-height:34px}
.ctrl-label{font-size:12.5px;color:var(--text);font-weight:500}
.ctrl-warn{font-size:10px;color:var(--red);opacity:.7;font-weight:400;margin-left:4px}
.switch{position:relative;width:38px;height:20px;flex-shrink:0}
.switch input{opacity:0;width:0;height:0;position:absolute}
.slider{position:absolute;cursor:pointer;inset:0;background:#2a2d36;border-radius:20px;transition:.2s}
.slider::before{content:'';position:absolute;height:14px;width:14px;left:3px;bottom:3px;background:#fff;border-radius:50%;transition:.2s}
input:checked+.slider{background:var(--accent)}
input:checked+.slider::before{transform:translateX(18px)}
.range-row{padding:6px 0}
.range-head{display:flex;justify-content:space-between;font-size:11px;margin-bottom:4px}
.range-head span{color:var(--dim)}
.range-head b{color:var(--accent);font-weight:600}
input[type=range]{width:100%;height:4px;-webkit-appearance:none;appearance:none;background:#2a2d36;border-radius:4px;outline:none;cursor:pointer}
input[type=range]::-webkit-slider-thumb{-webkit-appearance:none;width:14px;height:14px;border-radius:50%;background:var(--accent);border:2px solid #fff;cursor:pointer}
/* TABLE */
.dtable{width:100%;border-collapse:collapse;font-size:12px}
.dtable th{text-align:left;font-size:10px;text-transform:uppercase;letter-spacing:.5px;color:var(--dim);font-weight:700;padding:8px 10px;border-bottom:1px solid var(--border)}
.dtable td{padding:7px 10px;border-bottom:1px solid var(--border);color:var(--text)}
.dtable tr:hover td{background:rgba(56,189,248,.04)}
.badge-admin{background:rgba(239,68,68,.15);color:var(--red);padding:2px 8px;border-radius:10px;font-size:10px;font-weight:700;border:1px solid rgba(239,68,68,.3)}
.badge-player{color:var(--accent);font-size:11px;font-weight:600}
.tbtn{background:var(--bg3);border:1px solid var(--border);color:var(--text);padding:4px 10px;border-radius:5px;font-size:11px;cursor:pointer;font-weight:600;transition:all .15s}
.tbtn:hover{background:var(--accent);color:#fff;border-color:var(--accent)}
.tbtn-red{border-color:rgba(239,68,68,.4)}
.tbtn-red:hover{background:var(--red);border-color:var(--red)}
/* FOOTER */
footer{height:var(--footer);background:var(--bg2);border-top:1px solid var(--border);display:flex;align-items:center;justify-content:space-between;padding:0 20px;flex-shrink:0}
.user-info{display:flex;align-items:center;gap:10px}
.avatar{width:24px;height:24px;border-radius:50%;background:linear-gradient(135deg,var(--accent),var(--purple))}
.uname{font-size:12px;font-weight:600}
.ftime{font-size:11px;color:var(--dim)}
/* MAP */
#map-wrap{position:relative;flex:1;background:#060810;overflow:hidden}
#map-canvas{width:100%;height:100%;display:block;cursor:crosshair}
.map-hud{position:absolute;top:12px;left:12px;z-index:5;background:rgba(17,19,24,.88);backdrop-filter:blur(8px);border:1px solid var(--border);border-radius:8px;padding:10px 14px;min-width:180px}
.map-hud-title{font-size:9px;text-transform:uppercase;letter-spacing:1px;color:var(--dim);font-weight:700;margin-bottom:6px}
.coord{font-family:monospace;font-size:12px;color:var(--accent)}
.map-ctrls{position:absolute;top:12px;right:12px;z-index:5;display:flex;flex-direction:column;gap:6px}
.mbtn{width:32px;height:32px;background:rgba(17,19,24,.9);border:1px solid var(--border);border-radius:6px;color:var(--text);font-size:16px;font-weight:700;cursor:pointer;display:flex;align-items:center;justify-content:center;transition:all .15s}
.mbtn:hover{background:var(--accent);color:#fff}
.map-filters{position:absolute;bottom:12px;left:12px;z-index:5;display:flex;gap:6px}
.fchip{background:rgba(17,19,24,.88);border:1px solid var(--border);padding:5px 10px;border-radius:16px;font-size:11px;cursor:pointer;display:flex;align-items:center;gap:5px;color:var(--dim);user-select:none;transition:all .15s}
.fchip.on{border-color:var(--accent);color:var(--text)}
.fdot{width:7px;height:7px;border-radius:50%;flex-shrink:0}
.wp-card{position:absolute;bottom:12px;right:12px;z-index:5;background:rgba(17,19,24,.92);border:1px solid var(--border);border-radius:8px;padding:10px 14px;display:none;flex-direction:column;gap:6px;min-width:200px}
.tp-btn{background:var(--green);color:#fff;border:none;padding:8px;border-radius:6px;font-weight:700;font-size:12px;cursor:pointer;transition:all .15s}
.tp-btn:hover{filter:brightness(1.15)}
.admin-alert{position:absolute;top:12px;left:50%;transform:translateX(-50%);z-index:10;background:linear-gradient(135deg,#dc2626,#991b1b);color:#fff;padding:8px 18px;border-radius:20px;font-size:12px;font-weight:700;display:none;box-shadow:0 0 16px rgba(220,38,38,.5)}
</style>
</head>
<body>
<div class="app">
<!-- SIDEBAR -->
<nav class="sidebar">
<div class="logo">p4</div>
<button class="stab active" onclick="switchPage('combat')" title="Combat"><svg viewBox="0 0 24 24"><path d="M12 2C6.48 2 2 6.48 2 12s4.48 10 10 10 10-4.48 10-10S17.52 2 12 2zm0 18c-4.41 0-8-3.59-8-8s3.59-8 8-8 8 3.59 8 8-3.59 8-8 8zm-1-13h2v6h-2zm0 8h2v2h-2z"/><circle cx="12" cy="12" r="3" fill="none" stroke="currentColor" stroke-width="1.5"/><line x1="12" y1="2" x2="12" y2="6" stroke="currentColor" stroke-width="1.5"/><line x1="12" y1="18" x2="12" y2="22" stroke="currentColor" stroke-width="1.5"/><line x1="2" y1="12" x2="6" y2="12" stroke="currentColor" stroke-width="1.5"/><line x1="18" y1="12" x2="22" y2="12" stroke="currentColor" stroke-width="1.5"/></svg></button>
<button class="stab" onclick="switchPage('visuals')" title="Visuals"><svg viewBox="0 0 24 24"><path d="M12 4.5C7 4.5 2.73 7.61 1 12c1.73 4.39 6 7.5 11 7.5s9.27-3.11 11-7.5c-1.73-4.39-6-7.5-11-7.5zM12 17c-2.76 0-5-2.24-5-5s2.24-5 5-5 5 2.24 5 5-2.24 5-5 5zm0-8c-1.66 0-3 1.34-3 3s1.34 3 3 3 3-1.34 3-3-1.34-3-3-3z"/></svg></button>
<button class="stab" onclick="switchPage('player')" title="Player"><svg viewBox="0 0 24 24"><path d="M12 12c2.21 0 4-1.79 4-4s-1.79-4-4-4-4 1.79-4 4 1.79 4 4 4zm0 2c-2.67 0-8 1.34-8 4v2h16v-2c0-2.66-5.33-4-8-4z"/></svg></button>
<button class="stab" onclick="switchPage('world')" title="World"><svg viewBox="0 0 24 24"><path d="M12 2C6.48 2 2 6.48 2 12s4.48 10 10 10 10-4.48 10-10S17.52 2 12 2zm-1 17.93c-3.95-.49-7-3.85-7-7.93 0-.62.08-1.21.21-1.79L9 15v1c0 1.1.9 2 2 2v1.93zm6.9-2.54c-.26-.81-1-1.39-1.9-1.39h-1v-3c0-.55-.45-1-1-1H8v-2h2c.55 0 1-.45 1-1V7h2c1.1 0 2-.9 2-2v-.41c2.93 1.19 5 4.06 5 7.41 0 2.08-.8 3.97-2.1 5.39z"/></svg></button>
<button class="stab" onclick="switchPage('settings')" title="Settings"><svg viewBox="0 0 24 24"><path d="M19.14 12.94c.04-.3.06-.61.06-.94 0-.32-.02-.64-.07-.94l2.03-1.58c.18-.14.23-.41.12-.61l-1.92-3.32c-.12-.22-.37-.29-.59-.22l-2.39.96c-.5-.38-1.03-.7-1.62-.94l-.36-2.54c-.04-.24-.24-.41-.48-.41h-3.84c-.24 0-.43.17-.47.41l-.36 2.54c-.59.24-1.13.57-1.62.94l-2.39-.96c-.22-.08-.47 0-.59.22L2.74 8.87c-.12.21-.08.47.12.61l2.03 1.58c-.05.3-.07.62-.07.94s.02.64.07.94l-2.03 1.58c-.18.14-.23.41-.12.61l1.92 3.32c.12.22.37.29.59.22l2.39-.96c.5.38 1.03.7 1.62.94l.36 2.54c.05.24.24.41.48.41h3.84c.24 0 .44-.17.47-.41l.36-2.54c.59-.24 1.13-.56 1.62-.94l2.39.96c.22.08.47 0 .59-.22l1.92-3.32c.12-.22.07-.47-.12-.61l-2.01-1.58zM12 15.6c-1.98 0-3.6-1.62-3.6-3.6s1.62-3.6 3.6-3.6 3.6 1.62 3.6 3.6-1.62 3.6-3.6 3.6z"/></svg></button>
<button class="stab" onclick="switchPage('radar')" title="Radar Map"><svg viewBox="0 0 24 24"><path d="M20.5 3l-.16.03L15 5.1 9 3 3.36 4.9c-.21.07-.36.25-.36.48V20.5c0 .28.22.5.5.5l.16-.03L9 18.9l6 2.1 5.64-1.9c.21-.07.36-.25.36-.48V3.5c0-.28-.22-.5-.5-.5zM15 19l-6-2.11V5l6 2.11V19z"/></svg></button>
</nav>
<div class="main-area">
<!-- TOP BAR -->
<div class="top-bar">
<span class="top-label">overlay - in game menu</span>
<div class="top-right">
<span class="sub-label">SUB EXPIRES ON: <b>LIFETIME</b></span>
<span style="font-size:12px"><span class="status-dot"></span>Connected</span>
</div>
</div>
<!-- CONTENT -->
<div class="content">

<!-- COMBAT PAGE -->
<div class="page active" id="pg-combat">
<div class="subtabs" id="combat-tabs">
<button class="subtab active" onclick="subTab('combat',0)">Aimbot</button>
<button class="subtab" onclick="subTab('combat',1)">Silent</button>
<button class="subtab" onclick="subTab('combat',2)">TriggerBot</button>
<button class="subtab" onclick="subTab('combat',3)">Auto Peek</button>
</div>
<div class="panels">
<div class="panel" id="combat-left"></div>
<div class="panel" id="combat-right"></div>
</div>
</div>

<!-- VISUALS PAGE -->
<div class="page" id="pg-visuals">
<div class="subtabs" id="visuals-tabs">
<button class="subtab active" onclick="subTab('visuals',0)">Players</button>
<button class="subtab" onclick="subTab('visuals',1)">Vehicles</button>
<button class="subtab" onclick="subTab('visuals',2)">Others</button>
</div>
<div class="panels">
<div class="panel" id="visuals-left"></div>
<div class="panel" id="visuals-right"></div>
</div>
</div>

<!-- PLAYER PAGE -->
<div class="page" id="pg-player">
<div class="subtabs" id="player-tabs">
<button class="subtab active" onclick="subTab('player',0)">Main</button>
<button class="subtab" onclick="subTab('player',1)">Weapons</button>
<button class="subtab" onclick="subTab('player',2)">Teleports</button>
</div>
<div class="panels">
<div class="panel" id="player-left"></div>
<div class="panel" id="player-right"></div>
</div>
</div>

<!-- WORLD PAGE -->
<div class="page" id="pg-world">
<div class="subtabs" id="world-tabs">
<button class="subtab active" onclick="subTab('world',0)">Players</button>
<button class="subtab" onclick="subTab('world',1)">Vehicles</button>
</div>
<div class="panels"><div class="panel" id="world-content" style="overflow-y:auto"></div></div>
</div>

<!-- SETTINGS PAGE -->
<div class="page" id="pg-settings">
<div class="subtabs"><button class="subtab active">Main</button></div>
<div class="panels">
<div class="panel" id="settings-left"></div>
<div class="panel" id="settings-right"></div>
</div>
</div>

<!-- RADAR MAP PAGE -->
<div class="page" id="pg-radar">
<div id="map-wrap">
<canvas id="map-canvas"></canvas>
<div class="map-hud">
<div class="map-hud-title">LOCAL PLAYER</div>
<div style="display:flex;gap:12px;margin-bottom:4px"><span>X: <span class="coord" id="hx">0</span></span><span>Y: <span class="coord" id="hy">0</span></span><span>Z: <span class="coord" id="hz">0</span></span></div>
<div style="display:flex;gap:12px;font-size:11px"><span style="color:var(--green)">HP: <b id="hhp">0</b></span><span style="color:var(--accent)">ARM: <b id="harm">0</b></span><span style="color:var(--red)">ADM: <b id="hadm">0</b></span></div>
</div>
<div class="map-ctrls">
<button class="mbtn" onclick="zoomMap(1.25)">+</button>
<button class="mbtn" onclick="zoomMap(.8)">-</button>
<button class="mbtn" onclick="recenter()" style="font-size:12px">O</button>
</div>
<div class="admin-alert" id="admin-alert"></div>
<div class="map-filters">
<div class="fchip on" id="fa" onclick="togFilt('a')"><span class="fdot" style="background:var(--red)"></span>Admins (<b id="ca">0</b>)</div>
<div class="fchip on" id="fp" onclick="togFilt('p')"><span class="fdot" style="background:var(--accent)"></span>Players (<b id="cp">0</b>)</div>
<div class="fchip on" id="fv" onclick="togFilt('v')"><span class="fdot" style="background:var(--purple)"></span>Vehicles (<b id="cv">0</b>)</div>
<div class="fchip on" id="ff" onclick="togFollow()">Follow: <b id="lf">ON</b></div>
</div>
<div class="wp-card" id="wpc">
<div style="font-size:10px;text-transform:uppercase;letter-spacing:1px;color:var(--dim);font-weight:700">TARGET WAYPOINT</div>
<div style="font-size:11px;color:var(--dim)">Coords: <span class="coord" id="wcoord">0, 0</span></div>
<button class="tp-btn" onclick="tpWaypoint()">TELEPORT HERE</button>
</div>
</div>
</div>

</div>
<!-- FOOTER -->
<footer>
<div class="user-info"><div class="avatar"></div><span class="uname" id="foot-user">p4m user</span></div>
<span class="ftime" id="foot-time"></span>
</footer>
</div>
</div>

<script>
/* STATE */
let cfg={},local=null,players=[],vehicles=[],blips=[];
let curPage='combat',subPages={combat:0,visuals:0,player:0,world:0};
let filtA=true,filtP=true,filtV=true,follow=true,waypoint=null;
let mScale=1,panX=0,panY=0,dragging=false,dragSX=0,dragSY=0;
const GXN=-4000,GXX=4500,GYN=-4000,GYX=8000,GW=GXX-GXN,GH=GYX-GYN;
const canvas=document.getElementById('map-canvas');
const ctx=canvas.getContext('2d');

/* UTILS */
function tog(k,el){let v=el.checked;fetch('/api/set?'+k+'='+v);cfg[k]=v}
function sld(k,v,lbl){fetch('/api/set?'+k+'='+v);cfg[k]=parseFloat(v);if(lbl)lbl.textContent=v}
function mkToggle(id,label,warn){
  let w=warn?`<span class="ctrl-warn">[AC]</span>`:'';
  return`<div class="ctrl"><span class="ctrl-label">${label}${w}</span><label class="switch"><input type="checkbox" id="c-${id}" onchange="tog('${id}',this)"><span class="slider"></span></label></div>`;
}
function mkSlider(id,label,min,max,unit){
  return`<div class="range-row"><div class="range-head"><span>${label}</span><b id="v-${id}">${cfg[id]||0}${unit||''}</b></div><input type="range" min="${min}" max="${max}" id="c-${id}" oninput="sld('${id}',this.value,document.getElementById('v-${id}'));document.getElementById('v-${id}').textContent=this.value+'${unit||''}'"></div>`;
}

/* SYNC CONFIG FROM API */
function syncUI(){
  document.querySelectorAll('[id^="c-"]').forEach(el=>{
    let k=el.id.slice(2);
    if(k in cfg){
      if(el.type==='checkbox')el.checked=!!cfg[k];
      else{el.value=cfg[k];let vl=document.getElementById('v-'+k);if(vl)vl.textContent=cfg[k]+(vl.dataset.u||'')}
    }
  });
}

/* PAGE SWITCHING */
function switchPage(p){
  curPage=p;
  document.querySelectorAll('.stab').forEach((b,i)=>{b.classList.toggle('active',['combat','visuals','player','world','settings','radar'][i]===p)});
  document.querySelectorAll('.page').forEach(pg=>pg.classList.remove('active'));
  document.getElementById('pg-'+p).classList.add('active');
  if(p==='radar'){resizeCanvas();drawMap()}
  if(p==='settings'||p==='combat'||p==='visuals'||p==='player')fetch('/api/config').then(r=>r.json()).then(d=>{cfg=d;syncUI();renderPage()}).catch(()=>{});
  renderPage();
}
function subTab(page,idx){
  subPages[page]=idx;
  let tabs=document.getElementById(page+'-tabs');
  if(tabs)tabs.querySelectorAll('.subtab').forEach((b,i)=>b.classList.toggle('active',i===idx));
  renderPage();
}

/* RENDER PAGE CONTENT */
function renderPage(){
  switch(curPage){
    case'combat':renderCombat();break;
    case'visuals':renderVisuals();break;
    case'player':renderPlayer();break;
    case'world':renderWorld();break;
    case'settings':renderSettings();break;
  }
  syncUI();
}

function renderCombat(){
  let L=document.getElementById('combat-left'),R=document.getElementById('combat-right');
  let s=subPages.combat;
  if(s===0){
    L.innerHTML=`<div class="panel-title">AIMBOT</div>${mkToggle('aimbot_enabled','Enable')}${mkToggle('aimbot_visible','Only Visible')}`;
    R.innerHTML=`<div class="panel-title">SETTINGS</div>${mkSlider('aimbot_fov','Field of View',10,360,'deg')}${mkSlider('aimbot_speed','Smooth Speed',1,50,'')}${mkSlider('aimbot_distance','Max Distance',50,1000,'m')}`;
  }else if(s===1){
    L.innerHTML=`<div class="panel-title">SILENT AIM</div>${mkToggle('silentaim_enabled','Enable')}`;
    R.innerHTML=`<div class="panel-title">SETTINGS</div>${mkSlider('silentaim_fov','Field of View',10,360,'deg')}${mkSlider('silentaim_distance','Max Distance',50,1000,'m')}`;
  }else if(s===2){
    L.innerHTML=`<div class="panel-title">TRIGGERBOT</div>${mkToggle('triggerbot_enabled','Enable')}`;
    R.innerHTML=`<div class="panel-title">INFO</div><p style="color:var(--dim);font-size:12px;line-height:1.6">Triggerbot automatically fires when crosshair is over a target. Configure keybind and FOV in the overlay menu.</p>`;
  }else{
    L.innerHTML=`<div class="panel-title">AUTO PEEK</div><p style="color:var(--dim);font-size:12px;line-height:1.6">Auto Peek saves your position, lets you peek around corners, then teleports you back. Configure keybind in the overlay menu.</p>`;
    R.innerHTML=`<div class="panel-title">HOW TO USE</div><p style="color:var(--dim);font-size:12px;line-height:1.6">1. Press the peek key to save position.<br>2. Move and peek around corners.<br>3. Press again to teleport back.</p>`;
  }
  syncUI();
}

function renderVisuals(){
  let L=document.getElementById('visuals-left'),R=document.getElementById('visuals-right');
  let s=subPages.visuals;
  if(s===0){
    L.innerHTML=`<div class="panel-title">PLAYER ESP</div>${mkToggle('esp_enabled','Enable')}${mkToggle('esp_admin','Admin ESP')}${mkToggle('esp_admin_rainbow','Admin Rainbow')}${mkToggle('esp_box','Box')}${mkToggle('esp_skeleton','Skeleton')}${mkToggle('esp_snaplines','SnapLines')}${mkToggle('esp_headcircle','Head Circle')}${mkToggle('esp_names','Player Names')}${mkToggle('esp_weapon','Weapon Names')}${mkToggle('esp_distance','Distance')}${mkToggle('esp_health','Health Bar')}${mkToggle('esp_armor','Armor Bar')}`;
    R.innerHTML=`<div class="panel-title">FILTERS</div>${mkToggle('esp_ignore_npcs','Ignore NPCs')}${mkToggle('esp_ignore_dead','Ignore Dead')}${mkSlider('esp_max_distance','Max Distance',50,1000,'m')}`;
  }else if(s===1){
    L.innerHTML=`<div class="panel-title">VEHICLE ESP</div>${mkToggle('vehesp_enabled','Enable')}${mkToggle('vehesp_names','Vehicle Names')}${mkToggle('vehesp_snaplines','SnapLines')}${mkToggle('vehesp_distance','Distance')}`;
    R.innerHTML=`<div class="panel-title">INFO</div><p style="color:var(--dim);font-size:12px;line-height:1.6">Vehicle ESP shows nearby vehicles with names, distance, and lock status. Configure max distance in the overlay.</p>`;
  }else{
    L.innerHTML=`<div class="panel-title">OTHER SETTINGS</div><p style="color:var(--dim);font-size:12px;line-height:1.6">Additional visual settings like Friend Markers and color customization are available in the overlay menu.</p>`;
    R.innerHTML=`<div class="panel-title">WEB RADAR</div><p style="color:var(--dim);font-size:12px;line-height:1.6">The Radar Map tab shows all players, vehicles, and blips on a live interactive GTA V map.</p>`;
  }
  syncUI();
}

function renderPlayer(){
  let L=document.getElementById('player-left'),R=document.getElementById('player-right');
  let s=subPages.player;
  if(s===0){
    L.innerHTML=`<div class="panel-title">PLAYER EXPLOITS</div>${mkToggle('godmode','GodMode',true)}${mkToggle('noclip','NoClip',true)}${mkToggle('fastrun','Fast Run',true)}${mkToggle('stamina','Infinite Stamina')}`;
    R.innerHTML=`<div class="panel-title">WEAPON MODS</div>${mkToggle('norecoil','No Recoil')}${mkToggle('nospread','No Spread')}${mkToggle('ammo','Infinite Ammo')}${mkSlider('noclip_speed','NoClip Speed',1,20,'')}`;
  }else if(s===1){
    L.innerHTML=`<div class="panel-title">WEAPONS</div><p style="color:var(--dim);font-size:12px;line-height:1.6">Weapon spawn is only available from the in-game overlay menu.</p>`;
    R.innerHTML=`<div class="panel-title">AMMO</div><p style="color:var(--dim);font-size:12px;line-height:1.6">Use Add Ammo from the overlay to set ammo values for your current weapon.</p>`;
  }else{
    let locs=[['Waypoint','0','0'],['Plaza','156','-1043'],['Pier','-1848','-1223'],['Paleto Bay','-398','6048'],['Central Bank','222','217'],['Casino','885','17'],['LS Airport','-976','-2881'],['Sandy Shores','1681','3252']];
    L.innerHTML=`<div class="panel-title">TELEPORT LOCATIONS</div>`+locs.map(l=>`<div class="ctrl"><span class="ctrl-label">${l[0]}</span><button class="tbtn" onclick="fetch('/api/teleport?x=${l[1]}&y=${l[2]}').then(()=>alert('Teleported to ${l[0]}'))">GO</button></div>`).join('');
    R.innerHTML=`<div class="panel-title">INFO</div><p style="color:var(--dim);font-size:12px;line-height:1.6">Click GO to teleport to a preset location. Use the Radar Map to teleport to any point on the map by clicking.</p>`;
  }
  syncUI();
}

function renderWorld(){
  let C=document.getElementById('world-content');
  let s=subPages.world;
  if(s===0){
    if(!players.length){C.innerHTML='<div class="panel-title">PLAYERS</div><p style="color:var(--dim);text-align:center;padding:20px">No players in range</p>';return}
    C.innerHTML=`<div class="panel-title">PLAYERS (${players.length})</div><table class="dtable"><thead><tr><th>Type</th><th>Name</th><th>Dist</th><th>HP</th><th>Weapon</th><th>Actions</th></tr></thead><tbody>${players.map(p=>`<tr><td>${p.is_admin?'<span class="badge-admin">ADMIN</span>':'<span class="badge-player">Player</span>'}</td><td style="font-weight:600">${p.name}</td><td>${Math.round(p.dist)}m</td><td>${Math.round(p.health)}</td><td style="color:var(--dim)">${p.weapon||'Unarmed'}</td><td><button class="tbtn" onclick="fetch('/api/teleport?x=${p.x}&y=${p.y}&z=${p.z}')">TP</button></td></tr>`).join('')}</tbody></table>`;
  }else{
    if(!vehicles.length){C.innerHTML='<div class="panel-title">VEHICLES</div><p style="color:var(--dim);text-align:center;padding:20px">No vehicles in range</p>';return}
    C.innerHTML=`<div class="panel-title">VEHICLES (${vehicles.length})</div><table class="dtable"><thead><tr><th>Name</th><th>Dist</th><th>Driver</th><th>Actions</th></tr></thead><tbody>${vehicles.map(v=>`<tr><td style="font-weight:600">${v.name}</td><td>${Math.round(v.dist)}m</td><td style="color:var(--dim)">${v.has_driver?'Yes':'No'}</td><td><button class="tbtn" onclick="fetch('/api/teleport?x=${v.x}&y=${v.y}&z=${v.z}')">TP</button></td></tr>`).join('')}</tbody></table>`;
  }
}

function renderSettings(){
  let L=document.getElementById('settings-left'),R=document.getElementById('settings-right');
  L.innerHTML=`<div class="panel-title">GENERAL</div><p style="color:var(--dim);font-size:12px;line-height:1.6;margin-bottom:8px">Keybinds, Menu Color, Stream Proof, Watermark and Panic Key are configured from the overlay menu.</p><div class="ctrl"><span class="ctrl-label">Web Radar</span><span style="color:var(--green);font-size:12px;font-weight:600">Active</span></div>`;
  R.innerHTML=`<div class="panel-title">CONFIG</div><p style="color:var(--dim);font-size:12px;line-height:1.6">Export / Import configs from the overlay Settings page. Cloud sync URL can be set there as well.</p>`;
}

/* MAP */
function resizeCanvas(){canvas.width=canvas.parentElement.clientWidth;canvas.height=canvas.parentElement.clientHeight}
window.addEventListener('resize',()=>{if(curPage==='radar'){resizeCanvas();drawMap()}});

function w2c(wx,wy){
  let nx=(wx-GXN)/GW,ny=1-(wy-GYN)/GH;
  return{x:nx*2400*mScale+panX+canvas.width/2,y:ny*3200*mScale+panY+canvas.height/2};
}
function c2w(cx,cy){
  let nx=(cx-panX-canvas.width/2)/(2400*mScale),ny=(cy-panY-canvas.height/2)/(3200*mScale);
  return{x:GXN+nx*GW,y:GYN+(1-ny)*GH};
}

function drawMap(){
  if(!ctx)return;
  ctx.clearRect(0,0,canvas.width,canvas.height);
  ctx.fillStyle='#060810';ctx.fillRect(0,0,canvas.width,canvas.height);
  // Grid
  ctx.strokeStyle='#111825';ctx.lineWidth=1;
  for(let x=GXN;x<=GXX;x+=500){let a=w2c(x,GYN),b=w2c(x,GYX);ctx.beginPath();ctx.moveTo(a.x,a.y);ctx.lineTo(b.x,b.y);ctx.stroke()}
  for(let y=GYN;y<=GYX;y+=500){let a=w2c(GXN,y),b=w2c(GXX,y);ctx.beginPath();ctx.moveTo(a.x,a.y);ctx.lineTo(b.x,b.y);ctx.stroke()}
  // Island
  ctx.fillStyle='#0e1420';ctx.strokeStyle='#1a2640';ctx.lineWidth=2*mScale;
  let pts=[[-3000,-2500],[-3500,-1000],[-3200,1500],[-2500,3500],[-1800,5000],[-800,6500],[400,7500],[1800,7000],[3200,5000],[3600,2500],[3200,500],[2500,-1500],[2000,-3200],[500,-3800],[-1200,-3600],[-2200,-3000]];
  ctx.beginPath();pts.forEach((p,i)=>{let c=w2c(p[0],p[1]);i===0?ctx.moveTo(c.x,c.y):ctx.lineTo(c.x,c.y)});ctx.closePath();ctx.fill();ctx.stroke();
  // Lake
  ctx.fillStyle='#060810';ctx.beginPath();
  [[500,3500],[1800,3800],[2200,4200],[1500,4500],[600,4300],[200,3800]].forEach((p,i)=>{let c=w2c(p[0],p[1]);i===0?ctx.moveTo(c.x,c.y):ctx.lineTo(c.x,c.y)});ctx.closePath();ctx.fill();
  // Roads
  ctx.strokeStyle='#1a2844';ctx.lineWidth=2*mScale;ctx.beginPath();
  let r=w2c(-2800,-2000);ctx.moveTo(r.x,r.y);r=w2c(-3000,1000);ctx.lineTo(r.x,r.y);r=w2c(-2000,4000);ctx.lineTo(r.x,r.y);r=w2c(0,6800);ctx.lineTo(r.x,r.y);
  r=w2c(1500,-3000);ctx.moveTo(r.x,r.y);r=w2c(2500,1000);ctx.lineTo(r.x,r.y);r=w2c(2800,4500);ctx.lineTo(r.x,r.y);r=w2c(500,7200);ctx.lineTo(r.x,r.y);ctx.stroke();
  // LS glow
  ctx.fillStyle='rgba(30,45,75,.35)';let ls=w2c(0,-1500);ctx.beginPath();ctx.arc(ls.x,ls.y,200*mScale,0,Math.PI*2);ctx.fill();
  // Labels
  ctx.fillStyle='#3a4560';ctx.font=`${Math.max(9,12*mScale)}px sans-serif`;ctx.textAlign='center';
  [['LOS SANTOS',0,-1400],['LSIA',-1100,-3000],['SANDY SHORES',1600,3600],['MT CHILIAD',400,5600],['PALETO BAY',-200,6400],['FT ZANCUDO',-2200,3100]].forEach(l=>{let c=w2c(l[1],l[2]);ctx.fillText(l[0],c.x,c.y)});
  // Blips
  if(blips)blips.forEach(b=>{let p=w2c(b.x,b.y);let isWP=b.sprite===8;ctx.fillStyle=isWP?'#c084fc':'#475569';ctx.beginPath();ctx.arc(p.x,p.y,isWP?5:2.5,0,Math.PI*2);ctx.fill()});
  // Vehicles
  let vc=0;
  if(filtV&&vehicles){vc=vehicles.length;vehicles.forEach(v=>{let p=w2c(v.x,v.y);ctx.fillStyle='#a855f7';ctx.beginPath();ctx.arc(p.x,p.y,3.5,0,Math.PI*2);ctx.fill();ctx.fillStyle='#c084fc';ctx.font='9px sans-serif';ctx.textAlign='center';ctx.fillText(v.name,p.x,p.y-6)})}
  // Players
  let ac=0,pc=0;
  if(players)players.forEach(p=>{
    if(p.is_admin)ac++;
    if(p.is_admin&&!filtA)return;if(!p.is_admin&&!filtP)return;pc++;
    let pos=w2c(p.x,p.y);
    if(p.is_admin){
      let t=Date.now()/300,pr=7+Math.sin(t)*3;
      ctx.strokeStyle='rgba(239,68,68,.4)';ctx.lineWidth=1.5;ctx.beginPath();ctx.arc(pos.x,pos.y,pr+5,0,Math.PI*2);ctx.stroke();
      ctx.fillStyle='#ef4444';ctx.beginPath();ctx.arc(pos.x,pos.y,6,0,Math.PI*2);ctx.fill();
      ctx.fillStyle='#fca5a5';ctx.font='bold 10px sans-serif';ctx.textAlign='center';ctx.fillText('[ADMIN] '+p.name,pos.x,pos.y-10);
    }else{
      ctx.fillStyle='#38bdf8';ctx.beginPath();ctx.arc(pos.x,pos.y,4,0,Math.PI*2);ctx.fill();
      ctx.fillStyle='#cbd5e1';ctx.font='9px sans-serif';ctx.textAlign='center';ctx.fillText(p.name,pos.x,pos.y-7);
    }
  });
  // Waypoint
  if(waypoint){let wp=w2c(waypoint.x,waypoint.y);ctx.strokeStyle='var(--green,#22c55e)';ctx.lineWidth=2;ctx.beginPath();ctx.arc(wp.x,wp.y,9,0,Math.PI*2);ctx.stroke();ctx.fillStyle='#22c55e';ctx.beginPath();ctx.arc(wp.x,wp.y,3,0,Math.PI*2);ctx.fill()}
  // Local
  if(local){let m=w2c(local.x,local.y);let t=(Date.now()%2000)/2000;ctx.strokeStyle=`rgba(245,158,11,${1-t})`;ctx.lineWidth=1.5;ctx.beginPath();ctx.arc(m.x,m.y,6+t*20,0,Math.PI*2);ctx.stroke();ctx.fillStyle='#f59e0b';ctx.beginPath();ctx.arc(m.x,m.y,7,0,Math.PI*2);ctx.fill();ctx.fillStyle='#fff';ctx.beginPath();ctx.arc(m.x,m.y,2.5,0,Math.PI*2);ctx.fill();ctx.fillStyle='#fbbf24';ctx.font='bold 11px sans-serif';ctx.textAlign='center';ctx.fillText('YOU',m.x,m.y-11)}
  // Counters
  document.getElementById('ca').textContent=ac;
  document.getElementById('cp').textContent=players?players.length:0;
  document.getElementById('cv').textContent=vehicles?vehicles.length:0;
  let al=document.getElementById('admin-alert');
  if(ac>0){al.style.display='block';al.textContent=ac+' ADMIN(S) DETECTED IN SESSION'}else al.style.display='none';
}

// Map interaction
canvas.addEventListener('mousedown',e=>{dragging=true;dragSX=e.clientX-panX;dragSY=e.clientY-panY});
window.addEventListener('mousemove',e=>{if(!dragging)return;panX=e.clientX-dragSX;panY=e.clientY-dragSY;follow=false;document.getElementById('lf').textContent='OFF';document.getElementById('ff').classList.remove('on');drawMap()});
window.addEventListener('mouseup',()=>dragging=false);
canvas.addEventListener('wheel',e=>{e.preventDefault();zoomMap(e.deltaY<0?1.15:.87)},{passive:false});
canvas.addEventListener('click',e=>{if(dragging)return;let r=canvas.getBoundingClientRect();waypoint=c2w(e.clientX-r.left,e.clientY-r.top);document.getElementById('wpc').style.display='flex';document.getElementById('wcoord').textContent=waypoint.x.toFixed(1)+', '+waypoint.y.toFixed(1);drawMap()});

function zoomMap(f){mScale=Math.max(.15,Math.min(10,mScale*f));drawMap()}
function recenter(){if(local){let nx=(local.x-GXN)/GW,ny=1-(local.y-GYN)/GH;panX=-(nx*2400*mScale);panY=-(ny*3200*mScale)}else{panX=0;panY=0}drawMap()}
function togFollow(){follow=!follow;document.getElementById('lf').textContent=follow?'ON':'OFF';document.getElementById('ff').classList.toggle('on',follow);if(follow)recenter()}
function togFilt(t){if(t==='a'){filtA=!filtA;document.getElementById('fa').classList.toggle('on',filtA)}if(t==='p'){filtP=!filtP;document.getElementById('fp').classList.toggle('on',filtP)}if(t==='v'){filtV=!filtV;document.getElementById('fv').classList.toggle('on',filtV)}drawMap()}
function tpWaypoint(){if(!waypoint)return;fetch('/api/teleport?x='+waypoint.x.toFixed(1)+'&y='+waypoint.y.toFixed(1))}

/* POLLING */
function poll(){
  fetch('/api/blips').then(r=>r.json()).then(d=>{
    local=d.local;players=d.players||[];vehicles=d.vehicles||[];blips=d.map_blips||[];
    if(local){
      document.getElementById('hx').textContent=local.x.toFixed(1);
      document.getElementById('hy').textContent=local.y.toFixed(1);
      document.getElementById('hz').textContent=local.z.toFixed(1);
      document.getElementById('hhp').textContent=Math.round(local.health);
      document.getElementById('harm').textContent=Math.round(local.armor);
      document.getElementById('hadm').textContent=players.filter(p=>p.is_admin).length;
      if(follow&&curPage==='radar')recenter();
    }
    if(curPage==='radar')drawMap();
    if(curPage==='world')renderWorld();
  }).catch(()=>{});
}

/* FOOTER CLOCK */
function clock(){let d=new Date();document.getElementById('foot-time').textContent=d.toLocaleDateString('en',{weekday:'short'})+' '+d.toLocaleTimeString('en',{hour:'2-digit',minute:'2-digit'})}

/* INIT */
fetch('/api/config').then(r=>r.json()).then(d=>{cfg=d;renderPage()}).catch(()=>renderPage());
setInterval(poll,200);
setInterval(clock,1000);
clock();
resizeCanvas();
</script>
</body>
</html>
)WEBAPP";
			return page;
		}
	}
}