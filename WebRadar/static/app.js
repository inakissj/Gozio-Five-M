// ============================================================================
// GOZIO WEB PANEL & LIVE GTA V RADAR — Exact Replica of Video (80s-162s)
// ============================================================================

let currentPage = 'radar';
let gtaMap = null;
let currentMapStyle = 'atlas';
let showMapNames = true;
let tileLayers = {};
let layerGroups = {};

let localPlayer = null;
let playersList = [];
let vehiclesList = [];
let mapBlipsList = [];
let watchlist = [];
let isConnectedToGame = false;

let followedTarget = 'local'; // 'local' or player ID or null
let selectedWaypoint = null;
let waypointMarker = null;
let localPlayerMarker = null;

// Exact Rockstar GTA V World-to-Leaflet CRS Transformation Constants
const GTA_CRS_CONFIG = {
  centerX: 117.3,
  centerY: 172.8,
  scaleX: 0.02072,
  scaleY: 0.0205,
  ln2: Math.LN2,
};

const PAGE_METADATA = {
  dashboard: ['Dashboard', 'Session overview & quick access'],
  legitbot: ['Legitbot', 'Aim, silent and trigger'],
  esp_players: ['ESP Players', 'Player overlay'],
  esp_vehicles: ['ESP Vehicles', 'Vehicle overlay'],
  misc_esp: ['Misc ESP', 'HUD, radar and crosshair'],
  esp_preview: ['ESP Preview', 'Live overlay preview'],
  radar: ['Radar', 'Live map of the session'],
  players: ['Players', 'Target & interact'],
  vehicles: ['Vehicles', 'Live vehicles list'],
  resources: ['Resources', 'Server resources'],
  exploits: ['Exploits', 'Movement and utility'],
  settings: ['Settings', 'Panel, safety and session'],
  configs: ['Configs', 'Shared by the community'],
};

// 24 Verified Authentic Grand Theft Auto V In-Game Locations (Exact Game Coordinates)
const VERIFIED_GTA_LOCATIONS = [
  { id: 'legion', name: 'Legion Square', region: 'Downtown LS', x: 215.4, y: -810.2, z: 30.7 },
  { id: 'lsia', name: 'LS International Airport', region: 'South LS', x: -1037.0, y: -2737.0, z: 20.1 },
  { id: 'mazebank', name: 'Maze Bank Tower', region: 'Pillbox Hill', x: -75.0, y: -818.9, z: 326.2 },
  { id: 'casino', name: 'Diamond Casino & Resort', region: 'East Vinewood', x: 925.3, y: 46.2, z: 80.9 },
  { id: 'vinewood', name: 'Vinewood Sign', region: 'Vinewood Hills', x: 711.4, y: 1198.1, z: 348.5 },
  { id: 'observatory', name: 'Galileo Observatory', region: 'Vinewood Hills', x: -425.0, y: 1123.5, z: 325.9 },
  { id: 'delperro', name: 'Del Perro Pier', region: 'Del Perro Beach', x: -1655.2, y: -1032.1, z: 13.1 },
  { id: 'kortz', name: 'Kortz Center', region: 'Pacific Bluffs', x: -2243.8, y: 264.0, z: 174.6 },
  { id: 'vespucci', name: 'Vespucci Canals', region: 'Vespucci', x: -1112.4, y: -1062.8, z: 2.1 },
  { id: 'eclipse', name: 'Eclipse Towers', region: 'Rockford Hills', x: -773.4, y: 312.8, z: 85.7 },
  { id: 'grovest', name: 'Grove Street', region: 'Strawberry', x: 105.9, y: -1940.2, z: 20.8 },
  { id: 'mirrorpark', name: 'Mirror Park Blvd', region: 'East LS', x: 1070.2, y: -710.5, z: 58.5 },
  { id: 'docks', name: 'Port of South LS', region: 'Elysian Island', x: 101.3, y: -2970.0, z: 6.0 },
  { id: 'chumash', name: 'Chumash Plaza', region: 'Great Ocean Hwy', x: -3192.6, y: 1100.1, z: 20.8 },
  { id: 'zancudo', name: 'Fort Zancudo Airbase', region: 'Lago Zancudo', x: -2047.4, y: 3132.1, z: 32.8 },
  { id: 'sandy', name: 'Sandy Shores Airfield', region: 'Senora Desert', x: 1747.0, y: 3273.7, z: 41.1 },
  { id: 'sandy_town', name: 'Sandy Medical Center', region: 'Algonquin Blvd', x: 1853.2, y: 3686.5, z: 34.2 },
  { id: 'humanelabs', name: 'Humane Labs & Research', region: 'San Chianski', x: 3619.7, y: 3743.6, z: 28.6 },
  { id: 'prison', name: 'Bolingbroke Penitentiary', region: 'Route 68', x: 1845.5, y: 2585.9, z: 45.7 },
  { id: 'harmony', name: 'Harmony Truck Stop', region: 'Route 68', x: 548.2, y: 2669.5, z: 42.2 },
  { id: 'grapeseed', name: 'Grapeseed Main St', region: 'Alamo Sea', x: 1692.5, y: 4785.2, z: 42.0 },
  { id: 'chiliad', name: 'Mount Chiliad Summit', region: 'Chiliad', x: 501.8, y: 5604.5, z: 797.9 },
  { id: 'paleto', name: 'Paleto Bay Sheriff', region: 'Paleto Blvd', x: -446.1, y: 6013.8, z: 31.7 },
  { id: 'gordo', name: 'Mount Gordo Lighthouse', region: 'NE Coast', x: 3426.6, y: 5174.6, z: 7.4 },
];

// Default Server Resources (Exact Frame 148s)
const SERVER_RESOURCES = [
  { name: 'video3d', status: 'STARTED' },
  { name: 'vehsAddon', status: 'STARTED' },
  { name: 'pedText', status: 'STARTED' },
  { name: 'pvp-ui', status: 'STARTED' },
  { name: 'estrelaroupas', status: 'STARTED' },
  { name: 'mods', status: 'STARTED' },
  { name: 'roupas', status: 'STARTED' },
  { name: 'w_ar_carbineriflemk2_pb', status: 'STARTED' },
  { name: 'skyline', status: 'STARTED' },
  { name: 'vehicles2', status: 'STARTED' },
  { name: 'tebex2', status: 'STARTED' },
  { name: 'vehicles3', status: 'STARTED' },
];

// ============================================================================
// Sidebar & Page Switching
// ============================================================================
function switchPage(pageId) {
  currentPage = pageId;
  document.getElementById('app-sidebar')?.classList.remove('mobile-open');

  document.querySelectorAll('[data-page]').forEach(btn => {
    btn.classList.toggle('active', btn.getAttribute('data-page') === pageId);
  });

  document.querySelectorAll('.page-view').forEach(view => {
    view.classList.toggle('active', view.id === `page-${pageId}`);
  });

  const meta = PAGE_METADATA[pageId] || [pageId, ''];
  const titleEl = document.getElementById('topbar-title');
  const subEl = document.getElementById('topbar-sub');
  if (titleEl) titleEl.textContent = meta[0];
  if (subEl) subEl.textContent = meta[1];

  if (pageId === 'radar' && gtaMap) {
    requestAnimationFrame(() => {
      gtaMap.invalidateSize();
      setTimeout(() => gtaMap.invalidateSize(), 60);
    });
  } else if (pageId === 'esp_preview') {
    setTimeout(() => initEspPreview3D(), 50);
  } else if (pageId === 'players') {
    renderPlayersPage();
  } else if (pageId === 'vehicles') {
    renderVehiclesPage();
  } else if (pageId === 'resources') {
    renderResourcesList();
  }
}

function toggleSidebarCollapse() {
  const sb = document.getElementById('app-sidebar');
  if (!sb) return;
  if (window.innerWidth <= 780) {
    sb.classList.toggle('mobile-open');
  } else {
    sb.classList.toggle('collapsed');
  }
  if (gtaMap && currentPage === 'radar') {
    setTimeout(() => gtaMap.invalidateSize(), 200);
  }
}

function toggleMenuGroup() {
  const grp = document.getElementById('nav-group-menu');
  if (grp) grp.classList.toggle('collapsed');
}

// ============================================================================
// Monochrome White Slider Fill & UI Helpers (Exact Frame 86s / 95s)
// ============================================================================
function syncRangeVisual(inputEl, valSpanId, suffix = '') {
  if (!inputEl) return;
  const min = parseFloat(inputEl.min || '0');
  const max = parseFloat(inputEl.max || '100');
  const val = parseFloat(inputEl.value || '0');
  const pct = max > min ? ((val - min) / (max - min)) * 100 : 0;

  inputEl.style.background = `linear-gradient(90deg, #ffffff 0%, #ffffff ${pct.toFixed(1)}%, #1f1f22 ${pct.toFixed(1)}%, #1f1f22 100%)`;

  if (valSpanId) {
    const span = document.getElementById(valSpanId);
    if (span) {
      const step = parseFloat(inputEl.step || '1');
      const displayVal = step < 1 ? val.toFixed(1) : Math.round(val);
      span.textContent = `${displayVal}${suffix}`;
    }
  }
}

function initAllRangeSliders() {
  document.querySelectorAll('.wp-range').forEach(el => {
    const min = parseFloat(el.min || '0');
    const max = parseFloat(el.max || '100');
    const val = parseFloat(el.value || '0');
    const pct = max > min ? ((val - min) / (max - min)) * 100 : 0;
    el.style.background = `linear-gradient(90deg, #ffffff 0%, #ffffff ${pct.toFixed(1)}%, #1f1f22 ${pct.toFixed(1)}%, #1f1f22 100%)`;
  });
}

let activeKeybindBtn = null;

function bindKeyPrompt(btnEl, keyId) {
  if (!btnEl) return;
  if (activeKeybindBtn) activeKeybindBtn.classList.remove('listening');
  activeKeybindBtn = btnEl;
  btnEl.classList.add('listening');
  btnEl.textContent = 'Press any key...';

  const onKey = e => {
    e.preventDefault();
    e.stopPropagation();
    const keyName = e.key === 'Escape' ? 'Not set' : e.key.toUpperCase();
    btnEl.classList.remove('listening');
    btnEl.textContent = keyName;
    activeKeybindBtn = null;
    window.removeEventListener('keydown', onKey, true);
    flashSavingIndicator();
  };
  window.addEventListener('keydown', onKey, true);
}

function flashSavingIndicator() {
  const el = document.getElementById('saving-pill');
  if (!el) return;
  el.classList.add('show');
  clearTimeout(el._timer);
  el._timer = setTimeout(() => el.classList.remove('show'), 900);
}

function showPanelToast(title, sub) {
  const toast = document.getElementById('panel-toast');
  const tEl = document.getElementById('toast-title');
  const sEl = document.getElementById('toast-sub');
  if (!toast) return;
  if (tEl) tEl.textContent = title;
  if (sEl) sEl.textContent = sub;
  toast.classList.remove('hidden');
  clearTimeout(toast._timer);
  toast._timer = setTimeout(() => toast.classList.add('hidden'), 3200);
}

function handleGlobalSearch(query) {
  const q = (query || '').trim().toLowerCase();
  if (currentPage === 'radar') {
    const rSearch = document.getElementById('radar-player-search');
    if (rSearch) {
      rSearch.value = query;
      renderRadarPlayersList();
      renderLandmarkDirectory(q);
    }
    return;
  }
  document.querySelectorAll('.panel-card[data-searchable]').forEach(card => {
    if (!q) {
      card.classList.remove('filtered-out');
      return;
    }
    const hay = `${card.getAttribute('data-searchable')} ${card.textContent}`.toLowerCase();
    card.classList.toggle('filtered-out', !hay.includes(q));
  });
}

// Ctrl+K shortcut to focus search
window.addEventListener('keydown', e => {
  if ((e.ctrlKey || e.metaKey) && e.key.toLowerCase() === 'k') {
    e.preventDefault();
    document.getElementById('global-search-input')?.focus();
  }
});

// ============================================================================
// Authentic Rockstar GTA V Multi-Zoom Map (Exact Frame 80s, 130s, 136s, 139s)
// ============================================================================
function createGtaCRS() {
  const { centerX, centerY, scaleX, scaleY, ln2 } = GTA_CRS_CONFIG;
  return Object.assign({}, L.CRS.Simple, {
    projection: L.Projection.LonLat,
    scale(zoom) {
      return Math.pow(2, zoom);
    },
    zoom(sc) {
      return Math.log(sc) / ln2;
    },
    distance(pos1, pos2) {
      return Math.hypot(pos2.lng - pos1.lng, pos2.lat - pos1.lat);
    },
    transformation: new L.Transformation(scaleX, centerX, -scaleY, centerY),
    infinite: false,
  });
}

const MAP_STYLE_BG = {
  atlas: '#0fa0cd',
  satellite: '#153e6a',
  grid: '#8f8f8f',
};

function applyMapOceanBackground(style) {
  const bg = MAP_STYLE_BG[style] || '#0fa0cd';
  const mapEl = document.getElementById('gta-leaflet-map');
  const vpEl = document.getElementById('map-viewport');
  if (mapEl) mapEl.style.background = bg;
  if (vpEl) vpEl.style.background = bg;
}

function initGtaMap() {
  const container = document.getElementById('gta-leaflet-map');
  if (!container || gtaMap) return;

  const crs = createGtaCRS();

  // Exact full 0..256 Rockstar GTA V tile pyramid bounds (no clipped edges or white/black void)
  const southWest = crs.unproject(L.point(0, 256), 0);
  const northEast = crs.unproject(L.point(256, 0), 0);
  const gtaTileBounds = L.latLngBounds(southWest, northEast);

  const commonTileOpts = {
    minZoom: 0,
    maxZoom: 7,
    minNativeZoom: 0,
    maxNativeZoom: 4,
    tileSize: 256,
    noWrap: true,
    bounds: gtaTileBounds,
    keepBuffer: 6,
    updateWhenIdle: false,
    updateWhenZooming: true,
  };

  tileLayers = {
    atlas: L.tileLayer('/mapStyles/styleAtlas/{z}/{x}/{y}.jpg', {
      ...commonTileOpts,
      errorTileUrl: '/mapStyles/styleAtlas/empty.jpg?v=gozio_v1',
    }),
    satellite: L.tileLayer('/mapStyles/styleSatelite/{z}/{x}/{y}.jpg', {
      ...commonTileOpts,
      errorTileUrl: '/mapStyles/styleSatelite/empty.jpg?v=gozio_v1',
    }),
    grid: L.tileLayer('/mapStyles/styleGrid/{z}/{x}/{y}.png', {
      ...commonTileOpts,
      errorTileUrl: '/mapStyles/styleGrid/empty.png?v=gozio_v1',
    }),
  };

  applyMapOceanBackground(currentMapStyle);

  gtaMap = L.map(container, {
    crs,
    minZoom: 1,
    maxZoom: 7,
    zoom: 3,
    zoomSnap: 0.25,
    zoomDelta: 0.5,
    wheelPxPerZoomLevel: 80,
    center: [1200, 150],
    layers: [tileLayers.atlas],
    zoomControl: false,
    attributionControl: false,
    preferCanvas: true,
    maxBounds: gtaTileBounds,
    maxBoundsViscosity: 1.0,
    bounceAtZoomLimits: false,
  });

  layerGroups = {
    vehicles: L.layerGroup().addTo(gtaMap),
    players: L.layerGroup().addTo(gtaMap),
    admins: L.layerGroup().addTo(gtaMap),
  };

  // Live cursor coordinate tracking in GTA V world units (X = lng, Y = lat) — Exact Frame 80s format
  gtaMap.on('mousemove', e => {
    const wx = Math.round(e.latlng.lng);
    const wy = Math.round(e.latlng.lat);
    const el = document.getElementById('cursor-coords');
    if (el) {
      el.textContent = `X ${wx} · Y ${wy}`;
    }
  });

  // Click anywhere on the GTA V map to place a waypoint
  gtaMap.on('click', e => {
    const wx = e.latlng.lng;
    const wy = e.latlng.lat;
    setSelectedWaypoint(wx, wy, 35.0, 'Map Waypoint');
  });

  const refitMapToViewport = () => {
    if (!gtaMap) return;
    gtaMap.invalidateSize();
    gtaMap.fitBounds(gtaTileBounds, { padding: [0, 0], animate: false });
  };

  requestAnimationFrame(refitMapToViewport);
  setTimeout(refitMapToViewport, 300);

  // Keep the map fitted on viewport resize
  let resizeTimer = null;
  window.addEventListener('resize', () => {
    if (resizeTimer) clearTimeout(resizeTimer);
    resizeTimer = setTimeout(refitMapToViewport, 100);
  });

  // Also refit when the Radar page becomes visible after being hidden
  if (window.ResizeObserver) {
    try {
      const ro = new ResizeObserver(() => {
        if (resizeTimer) clearTimeout(resizeTimer);
        resizeTimer = setTimeout(refitMapToViewport, 100);
      });
      ro.observe(container);
    } catch (e) {}
  }
}

function setMapStyle(style) {
  if (!gtaMap || !tileLayers[style] || style === currentMapStyle) return;
  applyMapOceanBackground(style);
  gtaMap.removeLayer(tileLayers[currentMapStyle]);
  tileLayers[style].addTo(gtaMap);
  currentMapStyle = style;

  ['atlas', 'satellite', 'grid'].forEach(s => {
    const btn = document.getElementById(`style-btn-${s}`);
    if (btn) btn.classList.toggle('active', s === style);
  });
}

function toggleMapNames() {
  showMapNames = !showMapNames;
  const btn = document.getElementById('pill-toggle-names');
  if (btn) btn.classList.toggle('active', showMapNames);
  syncTelemetryMarkers();
}

function zoomMapIn() {
  if (gtaMap) gtaMap.zoomIn(0.75);
}

function zoomMapOut() {
  if (gtaMap) gtaMap.zoomOut(0.75);
}

function recenterOnLocal() {
  if (!gtaMap) return;
  const lpX = (localPlayer && typeof localPlayer.x === 'number') ? localPlayer.x : 215.4;
  const lpY = (localPlayer && typeof localPlayer.y === 'number') ? localPlayer.y : -810.2;
  gtaMap.flyTo([lpY, lpX], Math.max(gtaMap.getZoom(), 4), { duration: 0.4 });
}

function setSelectedWaypoint(x, y, z = 35.0, label = 'Selected Waypoint') {
  selectedWaypoint = { x, y, z, label };

  if (gtaMap) {
    const wpIcon = L.divIcon({
      className: '',
      html: `<div class="gozio-pin-wrap"><div class="gozio-dot-wp"></div>${showMapNames ? `<div class="gozio-nametag">${escapeHtml(label)}</div>` : ''}</div>`,
      iconSize: [0, 0],
    });
    if (waypointMarker) {
      waypointMarker.setLatLng([y, x]);
      waypointMarker.setIcon(wpIcon);
    } else {
      waypointMarker = L.marker([y, x], { icon: wpIcon }).addTo(gtaMap);
    }
  }

  const card = document.getElementById('waypoint-card');
  const lbl = document.getElementById('wp-label');
  const coords = document.getElementById('wp-coords');
  if (card) card.style.display = 'flex';
  if (lbl) lbl.textContent = label.toUpperCase();
  if (coords) coords.textContent = `X: ${x.toFixed(1)}, Y: ${y.toFixed(1)}, Z: ${z.toFixed(1)}`;
}

function clearWaypoint() {
  selectedWaypoint = null;
  if (waypointMarker && gtaMap) {
    gtaMap.removeLayer(waypointMarker);
    waypointMarker = null;
  }
  const card = document.getElementById('waypoint-card');
  if (card) card.style.display = 'none';
}

function teleportToSelectedWaypoint() {
  if (!selectedWaypoint) {
    showPanelToast('No waypoint set', 'Click anywhere on the GTA V radar map first');
    return;
  }
  teleportCoords(selectedWaypoint.x, selectedWaypoint.y, selectedWaypoint.z, selectedWaypoint.label);
}

function teleportCoords(x, y, z = 35.0, label = 'Location') {
  setSelectedWaypoint(x, y, z, label);
  fetch(`/api/teleport?x=${x.toFixed(2)}&y=${y.toFixed(2)}&z=${z.toFixed(2)}`)
    .then(r => r.json())
    .then(() => {
      showPanelToast('Teleport dispatched', `${label} (${x.toFixed(0)}, ${y.toFixed(0)})`);
      pollTelemetry();
    })
    .catch(() => {});
}

function renderLandmarkDirectory(filterOverride = null) {
  const container = document.getElementById('landmark-directory');
  if (!container) return;

  const q = (filterOverride !== null
    ? filterOverride
    : (document.getElementById('radar-player-search')?.value || '')
  ).trim().toLowerCase();

  const filtered = VERIFIED_GTA_LOCATIONS.filter(loc =>
    !q || loc.name.toLowerCase().includes(q) || loc.region.toLowerCase().includes(q)
  );

  const cnt = document.getElementById('poi-count-label');
  if (cnt) cnt.textContent = filtered.length;

  container.innerHTML = filtered.map(loc => `
    <div class="rpc-lm-row">
      <span class="rpc-lm-name" onclick="focusMapOnLandmark(${loc.x}, ${loc.y}, ${loc.z}, '${escapeAttr(loc.name)}')" title="Focus map on ${escapeAttr(loc.name)}">
        ${escapeHtml(loc.name)}
      </span>
      <button type="button" class="rpc-lm-tp" onclick="teleportCoords(${loc.x}, ${loc.y}, ${loc.z}, '${escapeAttr(loc.name)}')">TP</button>
    </div>
  `).join('');
}

function focusMapOnLandmark(x, y, z, name) {
  if (!gtaMap) return;
  followedTarget = null;
  gtaMap.flyTo([y, x], 4, { duration: 0.45 });
  setSelectedWaypoint(x, y, z, name);
}

// ============================================================================
// Radar Players Card & Map Marker Synchronization (Exact Frame 80s / 136s / 139s)
// ============================================================================
function syncTelemetryMarkers() {
  if (!gtaMap) return;

  // 1. Local Player Marker (Green circle + white border + dark nametag pill underneath)
  const lpX = (localPlayer && typeof localPlayer.x === 'number') ? localPlayer.x : 215.4;
  const lpY = (localPlayer && typeof localPlayer.y === 'number') ? localPlayer.y : -810.2;
  const lpName = (localPlayer && localPlayer.name) ? localPlayer.name : 'punkasswhiteboy';

  const selfIcon = L.divIcon({
    className: '',
    html: `
      <div class="gozio-pin-wrap">
        <div class="gozio-dot-self"></div>
        ${showMapNames ? `<div class="gozio-nametag">${escapeHtml(lpName)}</div>` : ''}
      </div>
    `,
    iconSize: [0, 0],
  });

  if (localPlayerMarker) {
    localPlayerMarker.setLatLng([lpY, lpX]);
    localPlayerMarker.setIcon(selfIcon);
  } else {
    localPlayerMarker = L.marker([lpY, lpX], { icon: selfIcon, zIndexOffset: 1000 }).addTo(gtaMap);
  }

  // 2. Remote Players & Flagged Admins
  layerGroups.players.clearLayers();
  layerGroups.admins.clearLayers();

  playersList.forEach(p => {
    const isAdm = Boolean(p.is_admin);
    const dotClass = isAdm ? 'gozio-dot-admin' : 'gozio-dot-player';
    const icon = L.divIcon({
      className: '',
      html: `
        <div class="gozio-pin-wrap">
          <div class="${dotClass}"></div>
          ${showMapNames ? `<div class="gozio-nametag">${escapeHtml(p.name || 'Player')}</div>` : ''}
        </div>
      `,
      iconSize: [0, 0],
    });
    const m = L.marker([p.y, p.x], { icon, zIndexOffset: isAdm ? 900 : 500 });
    m.on('click', () => {
      teleportCoords(Number(p.x), Number(p.y), Number(p.z || 35), p.name || 'Player');
    });
    m.addTo(isAdm ? layerGroups.admins : layerGroups.players);
  });

  // 3. Session Vehicles
  layerGroups.vehicles.clearLayers();
  vehiclesList.forEach(v => {
    const vIcon = L.divIcon({
      className: '',
      html: `
        <div class="gozio-pin-wrap">
          <div class="gozio-dot-veh"></div>
          ${showMapNames ? `<div class="gozio-nametag">${escapeHtml(v.name || 'Vehicle')}</div>` : ''}
        </div>
      `,
      iconSize: [0, 0],
    });
    L.marker([v.y, v.x], { icon: vIcon }).addTo(layerGroups.vehicles);
  });
}

function renderRadarPlayersList() {
  const container = document.getElementById('radar-players-list');
  if (!container) return;

  const query = (document.getElementById('radar-player-search')?.value || '').trim().toLowerCase();
  const lpX = (localPlayer && typeof localPlayer.x === 'number') ? localPlayer.x : 215.4;
  const lpY = (localPlayer && typeof localPlayer.y === 'number') ? localPlayer.y : -810.2;
  const lpName = (localPlayer && localPlayer.name) ? localPlayer.name : 'punkasswhiteboy';

  const allEntries = [
    {
      key: 'local',
      name: lpName,
      role: 'Local player',
      tag: 'You',
      dotClass: '',
      x: lpX,
      y: lpY,
      z: (localPlayer && localPlayer.z) || 30.7,
    },
    ...playersList.map(p => ({
      key: `p_${p.id}`,
      name: p.name || 'Player',
      role: p.is_admin ? 'Flagged Admin' : `${Math.round(p.dist || 0)}m · ${p.weapon || 'Unarmed'}`,
      tag: p.is_admin ? 'ADMIN' : `#${p.id ?? 0}`,
      dotClass: p.is_admin ? 'admin' : 'remote',
      x: Number(p.x),
      y: Number(p.y),
      z: Number(p.z || 35),
    })),
  ];

  const countEl = document.getElementById('radar-player-count');
  if (countEl) countEl.textContent = allEntries.length;

  const filtered = allEntries.filter(e => !query || e.name.toLowerCase().includes(query));

  container.innerHTML = filtered.map(e => `
    <div class="rpc-player-row ${followedTarget === e.key ? 'active' : ''}" onclick="followRadarPlayer('${escapeAttr(e.key)}', ${e.x}, ${e.y})">
      <div class="rpc-p-left">
        <span class="rpc-p-dot ${e.dotClass}"></span>
        <div>
          <div class="rpc-p-name">${escapeHtml(e.name)}</div>
          <div class="rpc-p-role">${escapeHtml(e.role)}</div>
        </div>
      </div>
      <span class="rpc-p-tag">${escapeHtml(e.tag)}</span>
    </div>
  `).join('');
}

function followRadarPlayer(key, x, y) {
  followedTarget = key;
  renderRadarPlayersList();
  if (gtaMap && typeof x === 'number' && typeof y === 'number') {
    gtaMap.flyTo([y, x], Math.max(gtaMap.getZoom(), 4), { duration: 0.4 });
  }
}

// ============================================================================
// Players, Vehicles & Resources Full Pages (Exact Frames 142s, 144s, 148s)
// ============================================================================
function renderPlayersPage() {
  const container = document.getElementById('players-page-rows');
  const banner = document.getElementById('players-online-banner');
  if (!container) return;

  const lpName = (localPlayer && localPlayer.name) ? localPlayer.name : 'punkasswhiteboy';
  const lpHp = (localPlayer && typeof localPlayer.health === 'number') ? Math.round(localPlayer.health / 2) : 100;
  const lpX = (localPlayer && typeof localPlayer.x === 'number') ? localPlayer.x : 215.4;
  const lpY = (localPlayer && typeof localPlayer.y === 'number') ? localPlayer.y : -810.2;
  const lpZ = (localPlayer && typeof localPlayer.z === 'number') ? localPlayer.z : 30.7;

  const rows = [
    { name: lpName, hpPct: Math.min(100, Math.max(0, lpHp)), weapon: 'Unarmed', dist: 0, x: lpX, y: lpY, z: lpZ, isAdmin: false },
    ...playersList.map(p => ({
      name: p.name || 'Player',
      hpPct: Math.min(100, Math.max(0, Math.round((p.health || 200) / 2))),
      weapon: (p.weapon || 'Unarmed').replace('WEAPON_', ''),
      dist: Math.round(p.dist || 0),
      x: Number(p.x),
      y: Number(p.y),
      z: Number(p.z || 35),
      isAdmin: Boolean(p.is_admin),
    })),
  ];

  if (banner) {
    banner.textContent = `Cheat online — ${rows.length} player${rows.length === 1 ? '' : 's'}`;
  }

  const q = (document.getElementById('players-page-filter')?.value || '').trim().toLowerCase();
  const filtered = rows.filter(r => !q || r.name.toLowerCase().includes(q));

  container.innerHTML = filtered.map(r => `
    <div class="entity-row">
      <div style="display:flex;align-items:center;gap:10px;">
        <div class="st-badge" style="border-radius:50%;width:24px;height:24px;font-style:normal;">☆</div>
        <span style="font-weight:700;color:#fff;">${escapeHtml(r.name)}</span>
      </div>
      <div style="display:flex;align-items:center;gap:12px;">
        <div class="entity-hp-bar"><div class="entity-hp-fill" style="width:${r.hpPct}%;"></div></div>
        <span style="font-family:var(--font-mono);font-size:11px;color:var(--text-secondary);">${r.hpPct}%</span>
        <span style="font-size:11.5px;color:var(--text-muted);">${escapeHtml(r.weapon)}</span>
      </div>
      <div style="display:flex;align-items:center;gap:10px;">
        <span style="font-family:var(--font-mono);font-size:11px;color:var(--text-secondary);">${r.dist} m</span>
        <button type="button" class="rpc-lm-tp" onclick="teleportCoords(${r.x}, ${r.y}, ${r.z}, '${escapeAttr(r.name)}')">TP</button>
      </div>
    </div>
  `).join('');
}

function renderVehiclesPage() {
  const container = document.getElementById('vehicles-page-rows');
  const banner = document.getElementById('vehicles-online-banner');
  if (!container) return;

  if (banner) {
    banner.textContent = `Cheat online — ${vehiclesList.length} vehicle${vehiclesList.length === 1 ? '' : 's'}`;
  }

  const q = (document.getElementById('vehicles-page-filter')?.value || '').trim().toLowerCase();
  const filtered = vehiclesList.filter(v => !q || (v.name || '').toLowerCase().includes(q));

  if (filtered.length === 0) {
    container.innerHTML = `
      <div class="panel-card" style="padding:28px;text-align:center;color:var(--text-muted);">
        No vehicles match the current filters.
      </div>
    `;
    return;
  }

  container.innerHTML = filtered.map(v => `
    <div class="entity-row">
      <span style="font-weight:700;color:#fff;">${escapeHtml(v.name || 'Vehicle')}</span>
      <span style="font-family:var(--font-mono);font-size:11px;color:var(--text-secondary);">${Math.round(v.dist || 0)} m</span>
      <button type="button" class="rpc-lm-tp" onclick="teleportCoords(${Number(v.x)}, ${Number(v.y)}, ${Number(v.z || 35)}, '${escapeAttr(v.name || 'Vehicle')}')">TP</button>
    </div>
  `).join('');
}

function renderResourcesList() {
  const container = document.getElementById('resources-list-container');
  if (!container) return;
  const q = (document.getElementById('resources-filter')?.value || '').trim().toLowerCase();
  const filtered = SERVER_RESOURCES.filter(r => !q || r.name.toLowerCase().includes(q));

  container.innerHTML = filtered.map(r => `
    <div class="res-row">
      <div class="res-left">
        <span class="user-status-dot"></span>
        <span>${escapeHtml(r.name)}</span>
      </div>
      <div class="res-right">
        <span class="res-started-tag">${r.status}</span>
        <button type="button" class="res-stop-btn" onclick="this.previousElementSibling.textContent='STOPPED';this.previousElementSibling.style.color='#ef4444';showPanelToast('Resource stopped', '${escapeAttr(r.name)}')">□ Stop</button>
      </div>
    </div>
  `).join('');
}

// ============================================================================
// Runtime Configuration & Watchlist API Sync
// ============================================================================
function setConfigValue(key, val) {
  flashSavingIndicator();
  fetch(`/api/set?${encodeURIComponent(key)}=${encodeURIComponent(val)}`)
    .then(() => fetchConfig())
    .catch(() => {});
}

function fetchConfig() {
  fetch('/api/config')
    .then(r => r.json())
    .then(cfg => {
      for (const [k, v] of Object.entries(cfg)) {
        const el = document.getElementById(`cfg-${k}`);
        if (el) {
          if (el.type === 'checkbox') {
            el.checked = Boolean(v);
          } else {
            el.value = v;
            if (el.classList.contains('wp-range')) {
              const suffix = (k.includes('distance') || k.includes('dist'))
                ? 'm'
                : k.includes('miss')
                  ? '%'
                  : k.includes('delay')
                    ? 'ms'
                    : '';
              syncRangeVisual(el, `val-${k}`, suffix);
            }
          }
        }
      }
    })
    .catch(() => {});
}

function exportWebConfig() {
  fetch('/api/config')
    .then(r => r.json())
    .then(cfg => {
      if (navigator.clipboard) {
        navigator.clipboard.writeText(JSON.stringify(cfg, null, 2));
      }
      showPanelToast('Config copied', 'Preset JSON copied to clipboard');
    })
    .catch(() => {});
}

function fetchWatchlist() {
  fetch('/api/admin/watchlist')
    .then(r => r.json())
    .then(data => {
      watchlist = data.watchlist || [];
      const container = document.getElementById('watchlist-chips');
      if (!container) return;
      container.innerHTML = watchlist.map(name => `
        <span style="display:inline-flex;align-items:center;gap:6px;padding:3px 9px;border-radius:6px;background:#141414;border:1px solid #262626;font-size:11px;color:#fff;">
          <span>${escapeHtml(name)}</span>
          <button type="button" onclick="removeAdminWatch('${escapeAttr(name)}')" style="background:none;border:none;color:#71717a;cursor:pointer;">✕</button>
        </span>
      `).join('');
    })
    .catch(() => {});
}

function addAdminWatch() {
  const input = document.getElementById('admin-input');
  if (!input) return;
  const name = input.value.trim();
  if (!name) return;
  fetch(`/api/admin/add?name=${encodeURIComponent(name)}`)
    .then(() => {
      input.value = '';
      fetchWatchlist();
      pollTelemetry();
    })
    .catch(() => {});
}

function removeAdminWatch(name) {
  fetch(`/api/admin/remove?name=${encodeURIComponent(name)}`)
    .then(() => {
      fetchWatchlist();
      pollTelemetry();
    })
    .catch(() => {});
}

function pollTelemetry() {
  fetch('/api/blips')
    .then(r => r.json())
    .then(data => {
      isConnectedToGame = Boolean(data.connected);
      localPlayer = data.local || null;
      playersList = Array.isArray(data.players) ? data.players : [];
      vehiclesList = Array.isArray(data.vehicles) ? data.vehicles : [];
      mapBlipsList = Array.isArray(data.map_blips) ? data.map_blips : [];

      syncTelemetryMarkers();
      renderRadarPlayersList();
      if (currentPage === 'players') renderPlayersPage();
      if (currentPage === 'vehicles') renderVehiclesPage();
    })
    .catch(() => {});
}

// ============================================================================
// Interactive 3D ESP Preview (Exact Frame 118s / 122s / 126s)
// ============================================================================
const espPreviewState = {
  box: true,
  boxStyle: 'full',
  boxThick: 1.5,
  boxColor: '#ffffff',
  hpPos: 'bottom',
  arPos: 'bottom',
  hpColor: '#00ff63',
  arColor: '#87cffa',
};

let esp3DInitialized = false;
let espPreviewDirty = true;
let espScene, espCamera, espRenderer, espOverlayCanvas, espOverlayCtx;
let espRotY = 0;
let espRotX = 0;
let espZoom = 100;

const ESP_JOINTS = {
  top: [0, 0.82, 0],
  bottom: [0, -0.78, 0],
  left: [-0.28, 0.0, 0],
  right: [0.28, 0.0, 0],
};

function initEspPreview3D() {
  if (esp3DInitialized) {
    resizeEspPreview();
    espPreviewDirty = true;
    return;
  }
  const viewport = document.getElementById('esp-3d-viewport');
  espOverlayCanvas = document.getElementById('esp-2d-overlay');
  if (!viewport || !espOverlayCanvas || typeof THREE === 'undefined') return;

  espScene = new THREE.Scene();
  espCamera = new THREE.PerspectiveCamera(38, 1, 0.1, 100);
  espCamera.position.set(0, 0.05, 2.8);

  espRenderer = new THREE.WebGLRenderer({ antialias: true, alpha: true });
  espRenderer.setPixelRatio(Math.min(window.devicePixelRatio, 2));
  viewport.appendChild(espRenderer.domElement);

  espScene.add(new THREE.AmbientLight(0xffffff, 0.75));
  const dir = new THREE.DirectionalLight(0xffffff, 0.85);
  dir.position.set(2, 4, 3);
  espScene.add(dir);

  // Build stylized 3D Ped Character matching Frame 118s (White Tank Top + Khaki Pants)
  const group = new THREE.Group();
  const skinMat = new THREE.MeshStandardMaterial({ color: 0xd8a47f, roughness: 0.55 });
  const topMat = new THREE.MeshStandardMaterial({ color: 0xf4f4f6, roughness: 0.7 });
  const pantsMat = new THREE.MeshStandardMaterial({ color: 0xb8a386, roughness: 0.8 });
  const hairMat = new THREE.MeshStandardMaterial({ color: 0x22160c, roughness: 0.9 });

  const addPart = (geo, mat, x, y, z, rx = 0, ry = 0, rz = 0) => {
    const m = new THREE.Mesh(geo, mat);
    m.position.set(x, y, z);
    m.rotation.set(rx, ry, rz);
    group.add(m);
  };

  // Head & Hair
  addPart(new THREE.SphereGeometry(0.095, 24, 20), skinMat, 0, 0.68, 0);
  addPart(new THREE.SphereGeometry(0.098, 20, 16), hairMat, 0, 0.71, -0.01);
  // Neck
  addPart(new THREE.CylinderGeometry(0.036, 0.042, 0.08, 16), skinMat, 0, 0.57, 0);
  // Torso (White tank top)
  addPart(new THREE.CylinderGeometry(0.135, 0.11, 0.36, 20), topMat, 0, 0.37, 0);
  // Midriff
  addPart(new THREE.CylinderGeometry(0.108, 0.118, 0.08, 20), skinMat, 0, 0.15, 0);
  // Pelvis & Khaki Pants
  addPart(new THREE.CylinderGeometry(0.122, 0.125, 0.12, 20), pantsMat, 0, 0.05, 0);
  addPart(new THREE.CylinderGeometry(0.062, 0.055, 0.72, 18), pantsMat, -0.07, -0.35, 0);
  addPart(new THREE.CylinderGeometry(0.062, 0.055, 0.72, 18), pantsMat, 0.07, -0.35, 0);
  // Arms
  addPart(new THREE.CylinderGeometry(0.036, 0.028, 0.52, 16), skinMat, -0.22, 0.32, 0, 0, 0, -0.28);
  addPart(new THREE.CylinderGeometry(0.036, 0.028, 0.52, 16), skinMat, 0.22, 0.32, 0, 0, 0, 0.28);

  espScene.add(group);
  espOverlayCtx = espOverlayCanvas.getContext('2d');

  let dragging = false;
  let lastX = 0;
  let lastY = 0;
  espRenderer.domElement.addEventListener('mousedown', e => {
    dragging = true;
    lastX = e.clientX;
    lastY = e.clientY;
  });
  window.addEventListener('mouseup', () => { dragging = false; });
  window.addEventListener('mousemove', e => {
    if (!dragging) return;
    espRotY += (e.clientX - lastX) * 0.01;
    espRotX = Math.max(-0.5, Math.min(0.5, espRotX + (e.clientY - lastY) * 0.006));
    lastX = e.clientX;
    lastY = e.clientY;
    espPreviewDirty = true;
  });

  esp3DInitialized = true;
  resizeEspPreview();
  requestAnimationFrame(animateEspPreview);
}

function resizeEspPreview() {
  const viewport = document.getElementById('esp-3d-viewport');
  if (!viewport || !espRenderer || !espOverlayCanvas) return;
  const w = viewport.clientWidth || 640;
  const h = viewport.clientHeight || 520;
  espRenderer.setSize(w, h);
  espCamera.aspect = w / h;
  espCamera.updateProjectionMatrix();
  espOverlayCanvas.width = w;
  espOverlayCanvas.height = h;
  espPreviewDirty = true;
}

function animateEspPreview() {
  requestAnimationFrame(animateEspPreview);
  if (currentPage !== 'esp_preview' || !espPreviewDirty) return;
  espPreviewDirty = false;

  const dist = 2.8 * (100 / espZoom);
  espCamera.position.set(Math.sin(espRotY) * dist, espRotX + 0.05, Math.cos(espRotY) * dist);
  espCamera.lookAt(0, 0, 0);
  espRenderer.render(espScene, espCamera);
  drawEspOverlay2D();
}

function drawEspOverlay2D() {
  if (!espOverlayCtx || !espOverlayCanvas) return;
  const w = espOverlayCanvas.width;
  const h = espOverlayCanvas.height;
  espOverlayCtx.clearRect(0, 0, w, h);

  const proj = pt => {
    const v = new THREE.Vector3(pt[0], pt[1], pt[2]).project(espCamera);
    return { x: ((v.x + 1) / 2) * w, y: ((-v.y + 1) / 2) * h };
  };

  const pTop = proj(ESP_JOINTS.top);
  const pBot = proj(ESP_JOINTS.bottom);
  const boxH = Math.abs(pBot.y - pTop.y) + 24;
  const boxW = boxH * 0.48;
  const bx = (pTop.x + pBot.x) / 2 - boxW / 2;
  const by = pTop.y - 12;

  if (espPreviewState.box) {
    espOverlayCtx.strokeStyle = espPreviewState.boxColor;
    espOverlayCtx.lineWidth = espPreviewState.boxThick;
    espOverlayCtx.strokeRect(bx, by, boxW, boxH);
  }

  // Health & Armor bars
  espOverlayCtx.fillStyle = espPreviewState.hpColor;
  espOverlayCtx.fillRect(bx, by + boxH + 6, boxW * 0.9, 4);
  espOverlayCtx.fillStyle = espPreviewState.arColor;
  espOverlayCtx.fillRect(bx, by + boxH + 13, boxW * 0.75, 4);
}

function toggleEspPreviewBox() {
  espPreviewState.box = !espPreviewState.box;
  const chk = document.getElementById('esp-pv-box-chk');
  const m = document.getElementById('esp-box-master');
  if (chk) chk.checked = espPreviewState.box;
  if (m) m.checked = espPreviewState.box;
  espPreviewDirty = true;
}

function zoomEspPreview(delta) {
  espZoom = Math.max(55, Math.min(180, espZoom + delta));
  const el = document.getElementById('esp-zoom-readout');
  if (el) el.textContent = `${espZoom}%`;
  espPreviewDirty = true;
}

function resetEspPreview() {
  espRotX = 0;
  espRotY = 0;
  espZoom = 100;
  const el = document.getElementById('esp-zoom-readout');
  if (el) el.textContent = '100%';
  espPreviewDirty = true;
}

function updateEspColor(key, val, swatchId, hexId) {
  espPreviewState[key] = val;
  const sw = document.getElementById(swatchId);
  const hx = document.getElementById(hexId);
  if (sw) sw.style.background = val;
  if (hx) hx.value = val;
  espPreviewDirty = true;
}

function escapeHtml(str) {
  return String(str)
    .replace(/&/g, '&amp;')
    .replace(/</g, '&lt;')
    .replace(/>/g, '&gt;')
    .replace(/"/g, '&quot;');
}

function escapeAttr(str) {
  return String(str).replace(/'/g, "\\'").replace(/"/g, '&quot;');
}

// ============================================================================
// Bootstrap Application
// ============================================================================
window.addEventListener('DOMContentLoaded', () => {
  initAllRangeSliders();
  initGtaMap();
  renderLandmarkDirectory();
  renderRadarPlayersList();
  renderResourcesList();
  fetchConfig();
  fetchWatchlist();
  pollTelemetry();
  setInterval(pollTelemetry, 1000);

  // Auto-hide initial "Panel connected" toast after 4 seconds
  setTimeout(() => {
    document.getElementById('panel-toast')?.classList.add('hidden');
  }, 4000);
});
