#pragma once
#include <Core/Web/WebServer.hpp>

namespace Core
{
	namespace Web
	{
		inline std::string cWebServer::GetHtmlPage()
		{
			std::string page;
			page.reserve(79476);
			page += R"WEBAPP1(<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1.0">
<title>Gozio</title>
<link rel="icon" type="image/png" href="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsMAAA7DAcdvqGQAAAMfSURBVFhHY2AYBaNgFIyCUTBYQN2+fewdu+/yoYvTDRSsPMaRt/3GxNKTHzoqdl8T+l9Tw9hz478wiN128gkcz/30T6RiyyUBdP24QM+jfxzoYjhBwvKDMhlbbtzLO/jiRc7Wq3uztt14nnfwxZfcfY//5+y+/z93z8P/GVtufA9qnR2JrhcdtF/6IJC949aFiEmrU9Dl8AJ7e3vu1DVnzhccf/8/7+ib/7HLjv8ImrR+rlfznOKYWdv6/drmt4DUtZz+IlF++KksTB/Ip9M+/xOt33lHtO/BP4miHZfUI6asm+1f3i4Pkq/bf4W77e4/VWS7cIL0tWfTCk98+J+88eofy7gs2+yFu1U7X/1TKd9xWQ8kn7vvYWH2nod/k1ad/ltw4OnK////M2ZuvjIrd9/Rb7n7n35MWXfhY1D9FNu8fY/WxE7f4ld68rVx3qGXL/IPv/qff+DppeKdNxTQ7UQBqS0tEllbb6yMW3biBYifMH2jS+GJ9/8jZu+6krF4rxoodPz716wI613qmXfwxd/kNedb/Nvn6SQsOtRYcv7nf8+mWW2tx++KFZ36/D9y2saM3ANPL0cvOnTHvWaab97+J1fSN145w8DAwIhuLxz41M8Ujp6+ZVrS4oMzQPzSUx8ycw+9/G8em2fddPOfXsm57/89ynsMQHK5+x7diFl0+CSIXXDs7fnQGVv3gdigaMjZ//R/zMxtxVl7Hv4Im7G5ASReePBpT8a2m/81NDS4UW1FA86lPWHRc3ddqDn30bPg2Lv//t0rNoHE6/a/5M7e++hu7oFnFzI3X+0FhYZ34+zSoqMvZ1Zc//8/de25Hbm77k7IWH7Yo/j05//BE1ZG5+59OCnv8KvPGRsudeTuffQ/dPLGA+j2YQVOZX3bQMGYsPL0dy0XH3iCS5i+WSVu6dGd0fP33vRpX9SY1D1HLmH5iUMxiw8fiF91+kzE7J2nI3sXmMYtPbLbv2WeW01NDWP88uNzoufvvRU+fctKTXtPcVSbcAA5ExvF8Nk7/wd0LmlEl6MbcC/tjDBPLhREFx8Fo2AUjIJRQC0AALOGjq/9oY30AAAAAElFTkSuQmCC">
<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/leaflet/1.9.4/leaflet.min.css"/>
<style>
:root{--bg:#0b0c10;--bg2:#101217;--bg3:#161920;--bg4:#1c1f28;--bg5:#22252f;--text:#e2e4e9;--white:#fff;--dim:#6b7080;--dim2:#484c58;--border:#1a1d25;--accent:#4f6ef7;--accent2:#3b5bdb;--sidebar-w:190px;--topbar-h:44px}
*{box-sizing:border-box;margin:0;padding:0}
body{font-family:-apple-system,'Segoe UI',system-ui,sans-serif;background:var(--bg);color:var(--text);height:100vh;overflow:hidden;display:flex;-webkit-font-smoothing:antialiased;font-size:13px}

/* SIDEBAR */
.sidebar{width:var(--sidebar-w);background:var(--bg2);border-right:1px solid var(--border);display:flex;flex-direction:column;flex-shrink:0;z-index:20;overflow:hidden}
.sidebar-brand{padding:16px 14px 12px;display:flex;align-items:center;gap:10px;border-bottom:1px solid var(--border)}
.brand-icon{width:28px;height:28px;border-radius:6px;display:flex;align-items:center;justify-content:center;flex-shrink:0;overflow:hidden;background:transparent}
.brand-icon img{width:100%;height:100%;object-fit:contain}
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
.connect-overlay{position:fixed;inset:0;background:var(--bg);z-index:9999;display:flex;align-items:center;justify-content:center;overflow:hidden}
.connect-canvas{position:absolute;inset:0;width:100%;height:100%;opacity:.35}
.connect-box{width:360px;text-align:center;position:relative;z-index:2}
.connect-logo{width:160px;height:auto;margin:0 auto 20px}
)WEBAPP1";
			page += R"WEBAPP2(.connect-logo img{width:100%;height:auto;filter:drop-shadow(0 0 20px rgba(79,110,247,.4))}
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

/* ESP PREVIEW */
.esp-wrap{display:flex;height:100%;overflow:hidden}
.esp-viewport{flex:1;display:flex;flex-direction:column;background:#000;position:relative}
.esp-toolbar{display:flex;align-items:center;justify-content:space-between;padding:6px 14px;font-size:10px;color:var(--dim);background:rgba(11,12,16,.6);border-bottom:1px solid var(--border);flex-shrink:0}
.esp-toolbar-right{display:flex;align-items:center;gap:12px}
.esp-toolbar label{display:flex;align-items:center;gap:4px;cursor:pointer}
.esp-toolbar button{background:none;border:1px solid var(--border);color:var(--dim);padding:3px 10px;border-radius:4px;font-size:9px;cursor:pointer;font-family:inherit}
.esp-toolbar button:hover{color:var(--text);border-color:var(--dim2)}
.esp-canvas-area{flex:1;position:relative;overflow:hidden}
#esp-3d{position:absolute;inset:0}
#esp-3d canvas{width:100%!important;height:100%!important;display:block}
#esp-overlay{position:absolute;inset:0;width:100%;height:100%;pointer-events:none;z-index:1}
.esp-bottom{display:flex;align-items:center;justify-content:space-between;padding:6px 14px;font-size:9px;color:var(--dim);background:rgba(11,12,16,.6);border-top:1px solid var(--border);flex-shrink:0}
.esp-bottom-info{display:flex;gap:14px}
.esp-bottom-info b{color:var(--text);font-weight:600}
.esp-zoom{display:flex;align-items:center;gap:6px}
.esp-zoom button{background:none;border:1px solid var(--border);color:var(--dim);width:20px;height:20px;border-radius:4px;font-size:11px;cursor:pointer;display:flex;align-items:center;justify-content:center}
.esp-zoom button:hover{color:var(--text)}
.esp-panel{width:340px;background:var(--bg2);border-left:1px solid var(--border);overflow-y:auto;flex-shrink:0;padding:16px}
.esp-panel::-webkit-scrollbar{width:3px}
.esp-panel::-webkit-scrollbar-thumb{background:var(--dim2);border-radius:3px}
.esp-section{margin-bottom:16px}
.esp-section-header{display:flex;align-items:center;gap:8px;margin-bottom:10px}
.esp-section-icon{width:28px;height:28px;border-radius:6px;background:var(--bg4);display:flex;align-items:center;justify-content:center;flex-shrink:0}
.esp-section-icon svg{width:14px;height:14px;color:var(--dim)}
.esp-section-info{flex:1;display:flex;flex-direction:column}
.esp-section-name{font-size:13px;font-weight:700;color:var(--white)}
.esp-section-desc{font-size:10px;color:var(--dim)}
.esp-row{display:flex;align-items:center;justify-content:space-between;padding:5px 0;min-height:28px}
.esp-lbl{font-size:12px;color:var(--text);font-weight:500}
.esp-select{background:var(--bg3);border:1px solid var(--border);color:var(--text);padding:5px 10px;border-radius:5px;font-size:11px;font-family:inherit;outline:none;min-width:100px;cursor:pointer}
.esp-color-row{display:flex;align-items:center;gap:8px;padding:5px 0}
.esp-color-swatch{width:28px;height:28px;border-radius:6px;border:1px solid var(--border);cursor:pointer;position:relative;overflow:hidden;flex-shrink:0}
.esp-color-swatch input[type=color]{position:absolute;inset:-4px;width:36px;height:36px;border:none;padding:0;cursor:pointer;opacity:0}
.esp-color-hex{background:var(--bg3);border:1px solid var(--border);color:var(--text);padding:5px 8px;border-radius:5px;font-size:11px;font-family:Consolas,monospace;width:72px;outline:none}
</style>
</head>
<body>

<!-- CONNECT SCREEN -->
<div class="connect-overlay" id="connect-screen">
<canvas class="connect-canvas" id="liquid-bg"></canvas>
<div class="connect-box">
<div class="connect-logo"><img src="data:image/png;base64,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)WEBAPP2";
			page += R"WEBAPP3(qh3Uvv6r7KErxL6JOx7sP5IEQRfaCRpqjnqneApSQN5+X0go2R+G2w8UQYL4g/Z6gX9hd8uEcUkUkM/mcbaYHX54FDQhXoIzL0FQbm3yKfju+O32/fzzy9DCb7YAL6flsJUTiyskse5mRrJxsYYaVHfl/iWiODzdRCUWwsheY2w9mgxzGBGYCbEdfXn8AnjLVwSndW44ZCetMfRb2nmWRgXKAfMl2NqE6fd4LM3SJ5+CjeOvHnBWxB9astfaXOPaPBtBdMZqqFxATL4ywYh+B4tJjaeLeVjC3qikU3so6ttMEWQBNNDpP3DhWv+FlYEBf377BDpjpn8uL756iyYwo6G5ZHb8+OvXFN7xe4BT1k6TOVEw9Rg6e35XPVhZmzaWGofSE5x7f/kbt4VM0+aDNPY0UQ8ONEwDQXfYECW4WcMeNj/x/88OFG2dezrTefEwHRuLExnRcHsMMXD+C3uFx3f/RIQvzVrriwNPDkxMI2hJn3NECaDlygOq2FcVx+WyBUr3vFiy2WzOZHN85WZ5Ib7WJzYmGJuYXhF74QZ4VthKicOPELCH8/nRp5fp0xaQO3jB8FigVo6lR0N4/zE4MHZBKy8RhJSYGvages0fm1V02P9xLBcGhtA7eN1kZWV+OtVIvXHfjFb5Kmpqb9d6Rc8dokwautyUXTEGmWqV86lS+9S21BZKwr/kJ28bZwgLn0MMzZxLDsufYxNku3i+G5bjsrs+tt1fUHytjERGXuHfRZZEhX1O3Falq1tFEr6GGk6rr/nr5oenzblvb2aq5qyJnKziBmV8mfh2kW/WSaO2bpUFBntI4pelb7v2Misen5dYLxsDkd9azI7Bsb6CklMDUc7tAHR7cfnNBjna232B1NRTG1PQ/M3ExSTNteDHQnjg2Qwej0H8C0K5KF2jLrre+Dj6GyYGqL4RpSWRQLDNDSvnfkc5cnJrGgY4y+CtfsKiFfM1ffDqmwNjPETgzdPvY3ahobmtZGUfeS9WdzIB2MC5bAm+yp54ycrvx0mc+NhaqCwZ7hCBRqa18oy2SbppFAl+GTmAlY++2zOgUlhStgQmbyOui4NzWuntrb2Z9MCxHXLdmsg5PwtGB+KL5dUGqjr0dB8byxjSuYEHNLDfKyV8xd+JUzc/Eql8jQ0rw3flH1Z0yWpsEQcm0T9j4bme0cbFfWTFdKYk4qsrF9T/6OhoaGhoaGhoaGhoaGhoaGhoaGhoaGhoaGhoaGhoaGhoaGhoaGhoaGhoaGhoaH57+P/ASUb18ZMQyJtAAAAAElFTkSuQmCC" alt="Gozio"></div>
<div class="connect-title">Gozio Web Panel</div>
<div class="connect-desc">Enter the connection code shown in the Gozio overlay menu<br>Settings > Web Map Radar Server</div>
<input class="connect-input" id="connect-code" type="text" maxlength="6" placeholder="000000" autocomplete="off" autofocus>
<button class="connect-btn" id="connect-btn" onclick="doConnect()">Connect</button>
<div class="connect-error" id="connect-error">Invalid code. Check the code in the overlay menu.</div>
<div class="connect-hint">The code is generated when the web server starts.<br>You can find it in Settings inside the Gozio menu.</div>
</div>
</div>
<script>
(function(){
var c=document.getElementById('liquid-bg'),ctx=c.getContext('2d');
function resize(){c.width=window.innerWidth;c.height=window.innerHeight}
resize();window.addEventListener('resize',resize);
var blobs=[
{x:.2,y:.3,vx:.0003,vy:.0002,r:300,color:[79,110,247]},
{x:.7,y:.6,vx:-.0002,vy:.0003,r:250,color:[59,91,219]},
{x:.5,y:.8,vx:.0004,vy:-.0002,r:280,color:[40,70,180]},
{x:.3,y:.5,vx:-.0003,vy:-.0001,r:220,color:[100,130,255]},
{x:.8,y:.2,vx:.0002,vy:.0004,r:260,color:[60,90,200]}
];
function draw(t){
ctx.fillStyle='#0b0c10';
ctx.fillRect(0,0,c.width,c.height);
blobs.forEach(function(b){
b.x+=b.vx+Math.sin(t*0.0004+b.r)*0.0002;
b.y+=b.vy+Math.cos(t*0.0003+b.r)*0.0002;
if(b.x<-0.2)b.vx=Math.abs(b.vx);if(b.x>1.2)b.vx=-Math.abs(b.vx);
if(b.y<-0.2)b.vy=Math.abs(b.vy);if(b.y>1.2)b.vy=-Math.abs(b.vy);
var px=b.x*c.width,py=b.y*c.height;
var pulse=b.r+Math.sin(t*0.001+b.r)*40;
var g=ctx.createRadialGradient(px,py,0,px,py,pulse);
g.addColorStop(0,'rgba('+b.color[0]+','+b.color[1]+','+b.color[2]+',0.3)');
g.addColorStop(0.5,'rgba('+b.color[0]+','+b.color[1]+','+b.color[2]+',0.08)');
g.addColorStop(1,'rgba('+b.color[0]+','+b.color[1]+','+b.color[2]+',0)');
ctx.fillStyle=g;ctx.fillRect(0,0,c.width,c.height);
});
if(document.getElementById('connect-screen').style.display!=='none')requestAnimationFrame(draw);
}
requestAnimationFrame(draw);
})();
</script>

<!-- SIDEBAR -->
<nav class="sidebar">
<div class="sidebar-brand">
<div class="brand-icon"><img src="data:image/png;base64,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" alt="Gozio"></div>
<div class="brand-text"><span class="brand-name">Gozio</span><span class="brand-sub">Web Panel</span></div>
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
<button class="nav-item" onclick="go('esp_preview')">ESP Preview</button>
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
<div class="user-avatar">GZ</div>
<div class="user-info"><span class="user-name" id="user-name">Gozio</span><span class="user-status"><span class="user-status-dot"></span> Online</span></div>
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
<div class="page" id="pg-esp_preview">
<div class="esp-wrap">
<div class="esp-viewport">
<div class="esp-toolbar"><span>Drag to rotate &middot; Scroll to zoom &middot; right-click items for options</span><div class="esp-toolbar-right"><label><input type="checkbox" id="esp-pv-box" checked onchange="espState.box=this.checked;espDirty=true"> Box</label><button onclick="espReset()">Reset positions</button></div></div>
<div class="esp-canvas-area"><div id="esp-3d"></div><canvas id="esp-overlay"></canvas></div>
<div class="esp-bottom"><div class="esp-bottom-info"><span>HP <b id="esp-hp-lbl">Bottom</b></span><span>AR <b id="esp-ar-lbl">Bottom</b></span><span>Text <b id="esp-txt-lbl">Bottom center</b></span></div><div class="esp-zoom"><button onclick="espDoZoom(-1)">&#8722;</button><button onclick="espDoZoom(0)">&#8634;</button><span id="esp-zoom-val">100%</span><button onclick="espDoZoom(1)">+</button></div></div>
</div>
<div class="esp-panel" id="esp-panel-content"></div>
</div>
</div>
<div class="page" id="pg-players"><div class="cfg-page active" id="cfg-players"></div></div>
<div class="page" id="pg-vehicles"><div class="cfg-page active" id="cfg-vehicles"></div></div>
<div class="page" id="pg-exploits"><div class="cfg-page active" id="cfg-exploits"></div></div>
<div class="page" id="pg-settings"><div class="cfg-page active" id="cfg-settings"></div></div>

</div>
</div>

<script src="https://cdnjs.cloudflare.com/ajax/libs/leaflet/1.9.4/leaflet.min.js"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/three.js/r128/three.min.js"></script>
<script>
var cfg={},blipData={},gtaMap=null,tileLayers={},currentStyle='atlas';
var layerGroups={},selfMarker=null,wpMarker=null,wpData=null;
var showNames=true,curPage='radar';
var authCode=localStorage.getItem('gozio_auth')||'';
var authenticated=false;

function doConnect(){
var code=document.getElementById('connect-code').value.trim();
if(code.length!==6){document.getElementById('connect-error').style.display='block';document.getElementById('connect-error').textContent='Enter a 6-digit code.';return}
var btn=document.getElementById('connect-btn');btn.disabled=true;btn.textContent='Connecting...';
fetch('/api/auth?code='+code).then(function(r){return r.json()}).then(function(d){
if(d.authenticated){
authCode=code;authenticated=true;localStorage.setItem('gozio_auth',code);
document.getElementById('connect-screen').style.display='none';
initMap();poll();setInterval(poll,350);
}else{
document.getElementById('connect-error').style.display='block';
document.getElementById('connect-error').textContent='Invalid code. Check the code in the overlay menu.';
btn.disabled=false;btn.textContent='Connect';
}
}).catch(function(){
document.getElementById('connect-error').style.display='block';
document.getElementById('connect-error').textContent='Cannot reach Gozio server. Make sure the cheat is running.';
btn.disabled=false;btn.textContent='Connect';
});
}
document.getElementById('connect-code').addEventListener('keydown',function(e){if(e.key==='Enter')doConnect()});

function api(path){return path+(path.indexOf('?')>=0?'&':'?')+'auth='+authCode}


var PAGE_INFO={
radar:['Radar','Live map of the session'],
aimbot:['Aimbot','Aimbot configuration'],
)WEBAPP3";
			page += R"WEBAPP4(silentaim:['SilentAim','Silent aim configuration'],
triggerbot:['TriggerBot','TriggerBot configuration'],
autopeek:['Auto Peek','Auto peek configuration'],
esp_players:['ESP Players','Player ESP settings'],
esp_vehicles:['ESP Vehicles','Vehicle ESP settings'],
misc_esp:['Misc ESP','Miscellaneous ESP and local player options'],
esp_preview:['ESP Preview','Preview ESP overlay settings'],
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
var map={'Aimbot':'aimbot','SilentAim':'silentaim','TriggerBot':'triggerbot','Auto Peek':'autopeek','ESP Players':'esp_players','ESP Vehicles':'esp_vehicles','Misc ESP':'misc_esp','ESP Preview':'esp_preview','Radar':'radar','Players':'players','Vehicles':'vehicles','Exploits':'exploits','Settings':'settings'};
if(map[t]===id){b.classList.add('active');found=true}
});
var info=PAGE_INFO[id]||[id,''];
document.getElementById('topbar-title').textContent=info[0];
document.getElementById('topbar-desc').textContent=info[1];
if(id==='radar'&&gtaMap)setTimeout(function(){gtaMap.invalidateSize()},50);
if(id==='esp_preview')setTimeout(initESPPreview,50);
else if(id!=='radar')renderCfgPage(id);
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
var ext=s==='grid'?'png':'jpg';
tileLayers[s]=L.tileLayer(base+'/mapStyles/'+folder+'/{z}/{x}/{y}.'+ext,{minZoom:1,maxZoom:5,tileSize:256,noWrap:true,errorTileUrl:base+'/mapStyles/'+folder+'/empty.'+ext});
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
)WEBAPP4";
			page += R"WEBAPP5(if(!selfMarker)selfMarker=L.marker([lp.y,lp.x],{icon:L.divIcon({className:'gta-self',iconSize:[14,14],iconAnchor:[7,7]})}).addTo(gtaMap);
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




// ESP PREVIEW
var espState={box:true,boxStyle:'full',boxThick:1.5,boxColor:'#ffffff',skeleton:true,skelColor:'#ffffff',skelOpacity:75,skelThick:1.5,hpPos:'bottom',arPos:'bottom',textPos:'bottom_center',textSize:14,hpColor:'#00ff63',arColor:'#87cffa',textColor:'#ffffff',hpOpacity:75,arOpacity:75,textOpacity:75};
var espZoomLvl=100,espInited=false,espDirty=true;
var espScene,espCamera,espRenderer,espPed,espOverlay,espOvCtx,espAnimId=0;
var espRotY=0,espRotX=0,espMouseDown=false,espMX=0,espMY=0;
var espJoints={
ht:[0,0.78,0],hd:[0,0.69,0],nk:[0,0.59,0],
ls:[-0.18,0.56,0],rs:[0.18,0.56,0],
le:[-0.3,0.31,0],re:[0.3,0.31,0],
lw:[-0.26,0.04,0],rw:[0.26,0.04,0],
sp:[0,0.31,0],pv:[0,0.03,0],
lh:[-0.09,0.01,0],rh:[0.09,0.01,0],
lk:[-0.1,-0.34,0],rk:[0.1,-0.34,0],
lf:[-0.1,-0.74,0],rf:[0.1,-0.74,0]
};

function hexRgba(hex,a){var r=parseInt(hex.substr(1,2),16),g=parseInt(hex.substr(3,2),16),b=parseInt(hex.substr(5,2),16);return 'rgba('+r+','+g+','+b+','+a+')'}

function espLimb(g,s,e,rt,rb,mat,seg){
var sv=new THREE.Vector3(s[0],s[1],s[2]),ev=new THREE.Vector3(e[0],e[1],e[2]);
var d=new THREE.Vector3().subVectors(ev,sv),ln=d.length();
var geo=new THREE.CylinderGeometry(rt,rb,ln,seg||16);
var m=new THREE.Mesh(geo,mat);
m.position.copy(sv).add(ev).multiplyScalar(0.5);
m.quaternion.setFromUnitVectors(new THREE.Vector3(0,1,0),d.clone().normalize());
g.add(m);return m;
}
function espSph(g,x,y,z,r,mat,sc){
var geo=new THREE.SphereGeometry(r,20,16);
var m=new THREE.Mesh(geo,mat);m.position.set(x,y,z);
if(sc)m.scale.set(sc[0],sc[1],sc[2]);g.add(m);return m;
}
function espBox(g,w,h,d,x,y,z,mat,rx,ry,rz){
var geo=new THREE.BoxGeometry(w,h,d);
var m=new THREE.Mesh(geo,mat);m.position.set(x,y,z);
if(rx)m.rotation.x=rx;if(ry)m.rotation.y=ry;if(rz)m.rotation.z=rz;
g.add(m);return m;
}
function espLathe(g,pts,x,y,z,mat,seg,sx,sy,sz){
var vs=pts.map(function(p){return new THREE.Vector2(p[0],p[1])});
var geo=new THREE.LatheGeometry(vs,seg||24);
var m=new THREE.Mesh(geo,mat);m.position.set(x,y,z);
if(sx)m.scale.set(sx,sy||1,sz||1);g.add(m);return m;
}

function buildPed3D(){
var g=new THREE.Group();
var skin=new THREE.MeshStandardMaterial({color:0xd4a574,roughness:0.55,metalness:0.01});
var skinD=new THREE.MeshStandardMaterial({color:0xc49464,roughness:0.6,metalness:0.01});
var shirt=new THREE.MeshStandardMaterial({color:0x2a2a2a,roughness:0.8});
var pants=new THREE.MeshStandardMaterial({color:0x1e3050,roughness:0.85});
var pantsD=new THREE.MeshStandardMaterial({color:0x162440,roughness:0.85});
var hairM=new THREE.MeshStandardMaterial({color:0x1a1008,roughness:0.95});
var shoeM=new THREE.MeshStandardMaterial({color:0x1a1a1a,roughness:0.9});
var soleM=new THREE.MeshStandardMaterial({color:0x333333,roughness:0.95});
var eyeW=new THREE.MeshStandardMaterial({color:0xf8f8f8,roughness:0.3,metalness:0.0});
var eyeI=new THREE.MeshStandardMaterial({color:0x3a2510,roughness:0.4,metalness:0.1});
var lipM=new THREE.MeshStandardMaterial({color:0xb87060,roughness:0.6});

// --- HEAD ---
espLathe(g,[[0,-0.055],[0.075,-0.05],[0.085,-0.03],[0.088,0],[0.09,0.03],[0.085,0.06],[0.075,0.085],[0.055,0.1],[0.03,0.105],[0,0.108]],0,0.69,0,skin,28,1,1,0.92);
// ears
espSph(g,-0.088,0.695,0,0.018,skin,[0.5,0.9,0.7]);
espSph(g,0.088,0.695,0,0.018,skin,[0.5,0.9,0.7]);
// nose
espLathe(g,[[0,-0.012],[0.012,-0.008],[0.015,0],[0.014,0.01],[0.008,0.018],[0,0.02]],0,0.672,0.08,skin,12,1,1,1);
// eyes
espSph(g,-0.032,0.705,0.074,0.013,eyeW,[1,0.75,0.5]);
espSph(g,0.032,0.705,0.074,0.013,eyeW,[1,0.75,0.5]);
espSph(g,-0.032,0.705,0.08,0.007,eyeI,[1,1,0.5]);
espSph(g,0.032,0.705,0.08,0.007,eyeI,[1,1,0.5]);
// eyebrows
espBox(g,0.032,0.006,0.012,-0.032,0.722,0.078,hairM,0.15);
espBox(g,0.032,0.006,0.012,0.032,0.722,0.078,hairM,0.15);
// mouth
espBox(g,0.03,0.005,0.01,0,0.66,0.08,lipM);
// hair (short buzz cut style)
espLathe(g,[[0,0.108],[0.035,0.107],[0.06,0.1],[0.08,0.085],[0.09,0.06],[0.092,0.03],[0.09,0.005]],0,0.69,0,hairM,28,1.03,1,0.95);

// --- NECK ---
espLimb(g,[0,0.635,0],[0,0.59,0],0.034,0.04,skin);

// --- TORSO (shirt) - using lathe for V-shape ---
espLathe(g,[[0,-0.14],[0.1,-0.135],[0.13,-0.1],[0.14,-0.05],[0.14,0],[0.135,0.05],[0.115,0.1],[0.085,0.12],[0.06,0.125],[0,0.128]],0,0.465,0,shirt,24,1,1,0.55);
// collar detail
espLimb(g,[0,0.588,0],[0,0.575,0],0.055,0.065,skin);
// shirt hem
espLimb(g,[0,0.34,0],[0,0.32,0],0.12,0.118,shirt);

// --- BELT ---
espLimb(g,[0,0.055,0],[0,0.035,0],0.115,0.113,new THREE.MeshStandardMaterial({color:0x2a2015,roughness:0.7}));
// belt buckle
espBox(g,0.025,0.018,0.01,0,0.045,0.11,new THREE.MeshStandardMaterial({color:0x888070,roughness:0.3,metalness:0.6}));

// --- HIPS/PELVIS ---
espLathe(g,[[0,-0.04],[0.1,-0.035],[0.115,-0.01],[0.115,0.01],[0.1,0.035],[0,0.04]],0,0.045,0,pants,24,1,1,0.55);

// --- SHOULDERS ---
espSph(g,-0.175,0.56,0,0.048,shirt,[1,0.85,0.9]);
espSph(g,0.175,0.56,0,0.048,shirt,[1,0.85,0.9]);

// --- UPPER ARMS ---
espLimb(g,[-0.19,0.54,0],[-0.28,0.33,0],0.04,0.035,shirt);
espLimb(g,[0.19,0.54,0],[0.28,0.33,0],0.04,0.035,shirt);
// shirt sleeve hem
espLimb(g,[-0.245,0.41,0],[-0.255,0.39,0],0.04,0.038,shirt);
espLimb(g,[0.245,0.41,0],[0.255,0.39,0],0.04,0.038,shirt);

// --- FOREARMS (skin) ---
espLimb(g,[-0.28,0.33,0],[-0.27,0.31,0],0.033,0.034,skin);
espSph(g,-0.29,0.31,0,0.034,skin);espSph(g,0.29,0.31,0,0.034,skin);
espLimb(g,[-0.29,0.31,0],[-0.26,0.08,0],0.032,0.024,skin);
espLimb(g,[0.28,0.33,0],[0.27,0.31,0],0.033,0.034,skin);
espLimb(g,[0.29,0.31,0],[0.26,0.08,0],0.032,0.024,skin);

// --- WRISTS ---
espSph(g,-0.26,0.06,0,0.024,skin);espSph(g,0.26,0.06,0,0.024,skin);

// --- HANDS ---
espBox(g,0.04,0.055,0.02,-0.255,0.03,0,skin);
espBox(g,0.04,0.055,0.02,0.255,0.03,0,skin);
// fingers (4 + thumb each hand)
var fi=[-0.015,-0.005,0.005,0.015];
for(var i=0;i<4;i++){
espBox(g,0.008,0.028,0.012,-0.255+fi[i],0.0,0,skinD);
espBox(g,0.008,0.028,0.012,0.255-fi[i],0.0,0,skinD);
}
// thumbs
espBox(g,0.012,0.025,0.014,-0.278,0.035,0.005,skinD,0,0,0.4);
espBox(g,0.012,0.025,0.014,0.278,0.035,0.005,skinD,0,0,-0.4);

// --- UPPER LEGS ---
espSph(g,-0.065,0.02,0,0.058,pants,[1,0.7,0.85]);
espSph(g,0.065,0.02,0,0.058,pants,[1,0.7,0.85]);
espLimb(g,[-0.08,0.0,0],[-0.09,-0.34,0],0.058,0.046,pants);
espLimb(g,[0.08,0.0,0],[0.09,-0.34,0],0.058,0.046,pants);

// --- KNEES ---
espSph(g,-0.09,-0.34,0,0.047,pants);espSph(g,0.09,-0.34,0,0.047,pants);

// --- LOWER LEGS (jeans continue) ---
espLimb(g,[-0.09,-0.34,0],[-0.09,-0.65,0],0.044,0.035,pants);
espLimb(g,[0.09,-0.34,0],[0.09,-0.65,0],0.044,0.035,pants);
// jeans cuff
espLimb(g,[-0.09,-0.64,0],[-0.09,-0.67,0],0.037,0.036,pantsD);
espLimb(g,[0.09,-0.64,0],[0.09,-0.67,0],0.037,0.036,pantsD);

// --- SHOES (sneaker style) ---
// ankle
espSph(g,-0.09,-0.68,0,0.035,shoeM);espSph(g,0.09,-0.68,0,0.035,shoeM);
// shoe body
espBox(g,0.065,0.04,0.13,-0.09,-0.72,0.015,shoeM);
espBox(g,0.065,0.04,0.13,0.09,-0.72,0.015,shoeM);
// sole
espBox(g,0.068,0.012,0.138,-0.09,-0.742,0.018,soleM);
espBox(g,0.068,0.012,0.138,0.09,-0.742,0.018,soleM);
// toe cap
espSph(g,-0.09,-0.73,0.075,0.033,shoeM,[1,0.5,0.6]);
espSph(g,0.09,-0.73,0.075,0.033,shoeM,[1,0.5,0.6]);

return g;
}

function initESPPreview(){
if(espInited){espResizeCanvas();espDirty=true;cancelAnimationFrame(espAnimId);espAnimId=requestAnimationFrame(espAnimate);return}
var container=document.getElementById('esp-3d');if(!container)return;
espScene=new THREE.Scene();espScene.background=new THREE.Color(0x000000);
espCamera=new THREE.PerspectiveCamera(40,1,0.1,100);
espCamera.position.set(0,0.1,2.8);espCamera.lookAt(0,0,0);
espRenderer=new THREE.WebGLRenderer({antialias:true});
espRenderer.setPixelRatio(Math.min(window.devicePixelRatio,2));
container.appendChild(espRenderer.domElement);
var amb=new THREE.AmbientLight(0x606060,1);espScene.add(amb);
var dir1=new THREE.DirectionalLight(0xffffff,0.7);dir1.position.set(2,3,4);espScene.add(dir1);
var dir2=new THREE.DirectionalLight(0x8888ff,0.3);dir2.position.set(-2,1,-2);espScene.add(dir2);
espPed=buildPed3D();espScene.add(espPed);
espOverlay=document.getElementById('esp-overlay');
espOvCtx=espOverlay.getContext('2d');
var cv=espRenderer.domElement;
cv.addEventListener('mousedown',function(e){espMouseDown=true;espMX=e.clientX;espMY=e.clientY});
cv.addEventListener('contextmenu',function(e){e.preventDefault()});
window.addEventListener('mouseup',function(){espMouseDown=false});
window.addEventListener('mousemove',function(e){
if(!espMouseDown)return;
espRotY+=(e.clientX-espMX)*0.008;espRotX+=(e.clientY-espMY)*0.005;
espRotX=Math.max(-0.6,Math.min(0.6,espRotX));
espMX=e.clientX;espMY=e.clientY;espDirty=true;
});
cv.addEventListener('wheel',function(e){
e.preventDefault();
espZoomLvl=Math.max(50,Math.min(200,espZoomLvl-(e.deltaY>0?-10:10)));
document.getElementById('esp-zoom-val').textContent=espZoomLvl+'%';
espDirty=true;
},{passive:false});
window.addEventListener('resize',function(){if(curPage==='esp_preview'){espResizeCanvas();espDirty=true}});
buildEspPanel();
espInited=true;espResizeCanvas();espDirty=true;
espAnimId=requestAnimationFrame(espAnimate);
}

function espResizeCanvas(){
var c=document.getElementById('esp-3d');if(!c)return;
var w=c.clientWidth,h=c.clientHeight;
espRenderer.setSize(w,h);
espCamera.aspect=w/h;espCamera.updateProjectionMatrix();
espOverlay.width=w;espOverlay.height=h;
espDirty=true;
}

function espAnimate(){
espAnimId=requestAnimationFrame(espAnimate);
if(curPage!=='esp_preview')return;
if(!espDirty)return;espDirty=false;
var dist=2.8*(100/espZoomLvl);
espCamera.position.set(Math.sin(espRotY)*dist,espRotX+0.1,Math.cos(espRotY)*dist);
espCamera.lookAt(0,0,0);
espRenderer.render(espScene,espCamera);
drawEspOverlay3D();
}

function espProject(key){
var j=espJoints[key];var v=new THREE.Vector3(j[0],j[1],j[2]);
v.project(espCamera);
return{x:(v.x+1)/2*espOverlay.width,y:(-v.y+1)/2*espOverlay.height};
}

function drawEspOverlay3D(){
var c=espOvCtx,w=espOverlay.width,h=espOverlay.height;
c.clearRect(0,0,w,h);
var J={};Object.keys(espJoints).forEach(function(k){J[k]=espProject(k)});
var minX=Infinity,minY=Infinity,maxX=-Infinity,maxY=-Infinity;
Object.keys(J).forEach(function(k){var p=J[k];if(p.x<minX)minX=p.x;if(p.y<minY)minY=p.y;if(p.x>maxX)maxX=p.x;if(p.y>maxY)maxY=p.y});
var pad=12;var bx1=minX-pad,by1=minY-pad,bx2=maxX+pad,by2=maxY+pad;
if(espState.box){
c.strokeStyle=espState.boxColor;c.lineWidth=espState.boxThick;
if(espState.boxStyle==='full'){c.strokeRect(bx1,by1,bx2-bx1,by2-by1)}
else{var cl=18;c.beginPath();
c.moveTo(bx1,by1+cl);c.lineTo(bx1,by1);c.lineTo(bx1+cl,by1);
c.moveTo(bx2-cl,by1);c.lineTo(bx2,by1);c.lineTo(bx2,by1+cl);
c.moveTo(bx1,by2-cl);c.lineTo(bx1,by2);c.lineTo(bx1+cl,by2);
c.moveTo(bx2-cl,by2);c.lineTo(bx2,by2);c.lineTo(bx2,by2-cl);
c.stroke();}
}
if(espState.skeleton){
var sa=espState.skelOpacity/100;c.strokeStyle=hexRgba(espState.skelColor,sa);c.lineWidth=espState.skelThick;c.lineCap='round';
var bones=[['ht','hd'],['hd','nk'],['nk','ls'],['nk','rs'],['ls','le'],['rs','re'],['le','lw'],['re','rw'],['nk','sp'],['sp','pv'],['pv','lh'],['pv','rh'],['lh','lk'],['rh','rk'],['lk','lf'],['rk','rf']];
)WEBAPP5";
			page += R"WEBAPP6(bones.forEach(function(b){var a=J[b[0]],z=J[b[1]];c.beginPath();c.moveTo(a.x,a.y);c.lineTo(z.x,z.y);c.stroke()});
c.fillStyle=hexRgba(espState.skelColor,sa);
Object.keys(J).forEach(function(k){c.beginPath();c.arc(J[k].x,J[k].y,3,0,Math.PI*2);c.fill()});
}
var bw=bx2-bx1,bh=by2-by1;
drawEspBar3D(c,espState.hpPos,bx1,by1,bw,bh,espState.hpColor,espState.hpOpacity,0.82,false);
drawEspBar3D(c,espState.arPos,bx1,by1,bw,bh,espState.arColor,espState.arOpacity,0.65,true);
var cx=(bx1+bx2)/2,ty=by2+18;
c.textAlign='center';c.textBaseline='top';
c.font='bold '+espState.textSize+'px -apple-system,sans-serif';
c.fillStyle=hexRgba(espState.textColor,espState.textOpacity/100);
c.fillText('Player_Name',cx,ty);
c.font=(espState.textSize-2)+'px -apple-system,sans-serif';
c.fillStyle=hexRgba(espState.textColor,espState.textOpacity/100*0.7);
c.fillText('Combat Pistol',cx,ty+espState.textSize*1.2);
c.fillText('12 m',cx,ty+espState.textSize*2.4);
try{document.getElementById('esp-hp-lbl').textContent=espState.hpPos.charAt(0).toUpperCase()+espState.hpPos.slice(1);
document.getElementById('esp-ar-lbl').textContent=espState.arPos.charAt(0).toUpperCase()+espState.arPos.slice(1);
document.getElementById('esp-txt-lbl').textContent=espState.textPos.replace('_',' ').replace(/\b\w/,function(c){return c.toUpperCase()});}catch(e){}
}

function drawEspBar3D(c,pos,bx,by,bw,bh,color,opacity,fill,second){
var a=opacity/100,t=4,off=second?6:0;
c.fillStyle='rgba(0,0,0,'+(a*0.5)+')';
if(pos==='bottom'){c.fillRect(bx,by+bh+3+off,bw,t);c.fillStyle=hexRgba(color,a);c.fillRect(bx,by+bh+3+off,bw*fill,t)}
else if(pos==='top'){c.fillRect(bx,by-t-3-off,bw,t);c.fillStyle=hexRgba(color,a);c.fillRect(bx,by-t-3-off,bw*fill,t)}
else if(pos==='left'){var x=bx-t-3-off;c.fillRect(x,by,t,bh);c.fillStyle=hexRgba(color,a);var fh=bh*fill;c.fillRect(x,by+bh-fh,t,fh)}
else if(pos==='right'){var x=bx+bw+3+off;c.fillRect(x,by,t,bh);c.fillStyle=hexRgba(color,a);var fh=bh*fill;c.fillRect(x,by+bh-fh,t,fh)}
}

function espDoZoom(d){if(d===0)espZoomLvl=100;else espZoomLvl=Math.max(50,Math.min(200,espZoomLvl+d*25));document.getElementById('esp-zoom-val').textContent=espZoomLvl+'%';espDirty=true}

function espReset(){
espState={box:true,boxStyle:'full',boxThick:1.5,boxColor:'#ffffff',skeleton:true,skelColor:'#ffffff',skelOpacity:75,skelThick:1.5,hpPos:'bottom',arPos:'bottom',textPos:'bottom_center',textSize:14,hpColor:'#00ff63',arColor:'#87cffa',textColor:'#ffffff',hpOpacity:75,arOpacity:75,textOpacity:75};
espZoomLvl=100;espRotY=0;espRotX=0;
document.getElementById('esp-zoom-val').textContent='100%';
buildEspPanel();espDirty=true;
}

function espRedraw(){espDirty=true}

function espCPick(label,key,def){
return '<div class="esp-color-row"><div class="esp-color-swatch" style="background:'+def+'" id="esw-'+key+'"><input type="color" value="'+def+'" onchange="espState.'+key+'=this.value;this.parentElement.style.background=this.value;document.getElementById(\'ehx-'+key+'\').value=this.value;espDirty=true"></div><span class="esp-lbl" style="flex:1;margin-left:4px">'+label+'</span><input class="esp-color-hex" id="ehx-'+key+'" value="'+def+'" onchange="espState.'+key+'=this.value;document.getElementById(\'esw-'+key+'\').style.background=this.value;espDirty=true"></div>';
}
function espOSlider(label,key,def){
return '<div style="padding:2px 0"><div class="rng-h"><span>'+label+'</span><b id="esv-'+key+'">'+def+'%</b></div><input type="range" min="0" max="100" step="5" value="'+def+'" oninput="espState.'+key+'=parseInt(this.value);document.getElementById(\'esv-'+key+'\').textContent=this.value+\'%\';espDirty=true"></div>';
}
function espSlider(label,key,min,max,step,def,suffix){
return '<div class="rng"><div class="rng-h"><span>'+label+'</span><b id="esv-'+key+'">'+def+(suffix||'')+'</b></div><input type="range" min="'+min+'" max="'+max+'" step="'+step+'" value="'+def+'" oninput="espState.'+key+'=parseFloat(this.value);document.getElementById(\'esv-'+key+'\').textContent=this.value+\''+(suffix||'')+'\';espDirty=true"></div>';
}

function buildEspPanel(){
var p=document.getElementById('esp-panel-content');if(!p)return;
var h='';
h+='<div class="esp-section"><div class="esp-section-header"><div class="esp-section-icon"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><rect x="3" y="3" width="18" height="18" rx="2"/></svg></div><div class="esp-section-info"><div class="esp-section-name">Box</div><div class="esp-section-desc">Style, thickness and color</div></div><label class="switch" style="margin-left:auto"><input type="checkbox" '+(espState.box?'checked':'')+' onchange="espState.box=this.checked;document.getElementById(\'esp-pv-box\').checked=this.checked;espDirty=true"><span class="sw-s"></span></label></div>';
h+='<div class="esp-row"><span class="esp-lbl">Style</span><select class="esp-select" onchange="espState.boxStyle=this.value;espDirty=true"><option value="full"'+(espState.boxStyle==='full'?' selected':'')+'>Full</option><option value="corner"'+(espState.boxStyle==='corner'?' selected':'')+'>Corner</option></select></div>';
h+=espSlider('Thickness','boxThick',0.5,5,0.5,espState.boxThick);
h+=espCPick('Color','boxColor',espState.boxColor);
h+='</div>';
h+='<div class="esp-section"><div class="esp-section-header"><div class="esp-section-icon"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="5" r="2"/><line x1="12" y1="7" x2="12" y2="15"/><line x1="8" y1="10" x2="16" y2="10"/><line x1="12" y1="15" x2="8" y2="21"/><line x1="12" y1="15" x2="16" y2="21"/></svg></div><div class="esp-section-info"><div class="esp-section-name">Skeleton</div><div class="esp-section-desc">Bone lines and joint dots</div></div><label class="switch" style="margin-left:auto"><input type="checkbox" '+(espState.skeleton?'checked':'')+' onchange="espState.skeleton=this.checked;espDirty=true"><span class="sw-s"></span></label></div>';
h+=espSlider('Thickness','skelThick',0.5,4,0.5,espState.skelThick);
h+=espCPick('Color','skelColor',espState.skelColor);
h+=espOSlider('Opacity','skelOpacity',espState.skelOpacity);
h+='</div>';
h+='<div class="esp-section"><div class="esp-section-header"><div class="esp-section-icon"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M12 2L2 7l10 5 10-5-10-5z"/><path d="M2 17l10 5 10-5"/><path d="M2 12l10 5 10-5"/></svg></div><div class="esp-section-info"><div class="esp-section-name">Positions</div><div class="esp-section-desc">Anchors around the player box</div></div></div>';
var posOpts='<option value="bottom">Bottom</option><option value="top">Top</option><option value="left">Left</option><option value="right">Right</option>';
h+='<div class="esp-row"><span class="esp-lbl">Health bar</span><select class="esp-select" id="esp-sel-hp" onchange="espState.hpPos=this.value;espDirty=true">'+posOpts+'</select></div>';
h+='<div class="esp-row"><span class="esp-lbl">Armor bar</span><select class="esp-select" id="esp-sel-ar" onchange="espState.arPos=this.value;espDirty=true">'+posOpts+'</select></div>';
h+='<div class="esp-row"><span class="esp-lbl">Text stack</span><select class="esp-select" onchange="espState.textPos=this.value;espDirty=true"><option value="bottom_center">Bottom center</option><option value="top_center">Top center</option></select></div>';
h+=espSlider('Text size','textSize',8,24,0.5,espState.textSize.toFixed(1));
h+='</div>';
h+='<div class="esp-section"><div class="esp-section-header"><div class="esp-section-icon"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="10"/><circle cx="12" cy="12" r="4"/></svg></div><div class="esp-section-info"><div class="esp-section-name">Colors</div><div class="esp-section-desc">Same pickers as the item menus</div></div></div>';
h+=espCPick('HP bar','hpColor',espState.hpColor);h+=espOSlider('','hpOpacity',espState.hpOpacity);
h+=espCPick('AR bar','arColor',espState.arColor);h+=espOSlider('','arOpacity',espState.arOpacity);
h+=espCPick('Text','textColor',espState.textColor);h+=espOSlider('','textOpacity',espState.textOpacity);
h+='</div>';
p.innerHTML=h;
if(document.getElementById('esp-sel-hp'))document.getElementById('esp-sel-hp').value=espState.hpPos;
if(document.getElementById('esp-sel-ar'))document.getElementById('esp-sel-ar').value=espState.arPos;
}

// Try auto-reconnect with saved code
if(authCode){
fetch('/api/auth?code='+authCode).then(function(r){return r.json()}).then(function(d){
if(d.authenticated){
authenticated=true;
document.getElementById('connect-screen').style.display='none';
initMap();poll();setInterval(poll,350);
}else{localStorage.removeItem('gozio_auth');authCode=''}
}).catch(function(){});
}
</script>
</body>
</html>
)WEBAPP6";
			return page;
		}
	}
}
