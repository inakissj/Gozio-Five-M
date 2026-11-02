// ============================================================================
// p4m — San Andreas Cartography & Telemetry Console (Frontend Application)
// Authentic Rockstar GTA V Multi-Zoom Tile Map + Live C++ / Render Cloud Sync
// ============================================================================

let activeTab = 'map';
let gtaMap = null;
let currentMapStyle = 'atlas';
let tileLayers = {};
let layerGroups = {};

let localPlayer = null;
let playersList = [];
let vehiclesList = [];
let mapBlipsList = [];
let watchlist = [];
let isConnectedToGame = false;

let followLocal = false;
let selectedWaypoint = null;
let waypointMarker = null;
let localPlayerMarker = null;

const layersState = {
  pois: true,
  admins: true,
  players: true,
  vehicles: true,
  blips: true,
};

// Exact Rockstar GTA V World-to-Leaflet CRS Transformation Constants
const GTA_CRS_CONFIG = {
  centerX: 117.3,
  centerY: 172.8,
  scaleX: 0.02072,
  scaleY: 0.0205,
  ln2: Math.LN2,
};

const STYLE_OCEAN_COLORS = {
  atlas: '#384950',
  satellite: '#143d6b',
  grid: '#1a3a4a',
};

// 24 Verified Authentic Grand Theft Auto V In-Game Locations (Exact Game Coordinates)
const VERIFIED_GTA_LOCATIONS = [
  { id: 'legion', name: 'Legion Square', region: 'Downtown Los Santos', x: 215.4, y: -810.2, z: 30.7 },
  { id: 'lsia', name: 'Los Santos International Airport', region: 'South Los Santos', x: -1037.0, y: -2737.0, z: 20.1 },
  { id: 'mazebank', name: 'Maze Bank Tower Helipad', region: 'Pillbox Hill', x: -75.0, y: -818.9, z: 326.2 },
  { id: 'casino', name: 'The Diamond Casino & Resort', region: 'East Vinewood', x: 925.3, y: 46.2, z: 80.9 },
  { id: 'vinewood', name: 'Vinewood Sign Overlook', region: 'Vinewood Hills', x: 711.4, y: 1198.1, z: 348.5 },
  { id: 'observatory', name: 'Galileo Observatory', region: 'Vinewood Hills', x: -425.0, y: 1123.5, z: 325.9 },
  { id: 'delperro', name: 'Del Perro Pier', region: 'Del Perro Beach', x: -1655.2, y: -1032.1, z: 13.1 },
  { id: 'kortz', name: 'Kortz Center', region: 'Pacific Bluffs', x: -2243.8, y: 264.0, z: 174.6 },
  { id: 'vespucci', name: 'Vespucci Canals', region: 'Vespucci', x: -1112.4, y: -1062.8, z: 2.1 },
  { id: 'eclipse', name: 'Eclipse Towers', region: 'Rockford Hills', x: -773.4, y: 312.8, z: 85.7 },
  { id: 'grovest', name: 'Grove Street Cul-de-Sac', region: 'Strawberry / Davis', x: 105.9, y: -1940.2, z: 20.8 },
  { id: 'mirrorpark', name: 'Mirror Park Boulevard', region: 'East Los Santos', x: 1070.2, y: -710.5, z: 58.5 },
  { id: 'docks', name: 'Port of South Los Santos', region: 'Elysian Island', x: 101.3, y: -2970.0, z: 6.0 },
  { id: 'chumash', name: 'Chumash Plaza', region: 'Great Ocean Highway', x: -3192.6, y: 1100.1, z: 20.8 },
  { id: 'zancudo', name: 'Fort Zancudo Airbase', region: 'Lago Zancudo', x: -2047.4, y: 3132.1, z: 32.8 },
  { id: 'sandy', name: 'Sandy Shores Airfield', region: 'Grand Senora Desert', x: 1747.0, y: 3273.7, z: 41.1 },
  { id: 'sandy_town', name: 'Sandy Shores Medical Center', region: 'Algonquin Blvd', x: 1853.2, y: 3686.5, z: 34.2 },
  { id: 'humanelabs', name: 'Humane Labs and Research', region: 'San Chianski Range', x: 3619.7, y: 3743.6, z: 28.6 },
  { id: 'prison', name: 'Bolingbroke Penitentiary', region: 'Route 68 / Senora', x: 1845.5, y: 2585.9, z: 45.7 },
  { id: 'harmony', name: 'Harmony Truck Stop', region: 'Route 68', x: 548.2, y: 2669.5, z: 42.2 },
  { id: 'grapeseed', name: 'Grapeseed Main Street', region: 'Alamo Sea East', x: 1692.5, y: 4785.2, z: 42.0 },
  { id: 'chiliad', name: 'Mount Chiliad Summit', region: 'Chiliad Wilderness', x: 501.8, y: 5604.5, z: 797.9 },
  { id: 'paleto', name: 'Paleto Bay Sheriff Office', region: 'Paleto Blvd', x: -446.1, y: 6013.8, z: 31.7 },
  { id: 'gordo', name: 'Mount Gordo Lighthouse', region: 'Northeast Coast', x: 3426.6, y: 5174.6, z: 7.4 },
];

// Region lookup by GTA V coordinates
function resolveGtaRegion(wx, wy) {
  let closest = null;
  let minDist = Infinity;
  for (const loc of VERIFIED_GTA_LOCATIONS) {
    const d = Math.hypot(wx - loc.x, wy - loc.y);
    if (d < minDist) {
      minDist = d;
      closest = loc;
    }
  }
  if (closest && minDist <= 850) {
    return closest.region;
  }
  if (wy > 4500) return 'Paleto & Northern Coast';
  if (wy > 1600) return 'Blaine County';
  if (wy > 200) return 'Vinewood & Northern Hills';
  return 'Los Santos Metropolitan';
}

// ============================================================================
// Authentic GTA V Leaflet Map Initialization
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
    infinite: true,
  });
}

function initGtaMap() {
  const container = document.getElementById('gta-leaflet-map');
  if (!container || gtaMap) return;

  const crs = createGtaCRS();

  tileLayers = {
    atlas: L.tileLayer('/mapStyles/styleAtlas/{z}/{x}/{y}.jpg', {
      minZoom: 0,
      maxZoom: 5,
      noWrap: true,
      attribution: 'Grand Theft Auto V Cartography · San Andreas',
      errorTileUrl: '/mapStyles/styleAtlas/empty.jpg',
    }),
    satellite: L.tileLayer('/mapStyles/styleSatelite/{z}/{x}/{y}.jpg', {
      minZoom: 0,
      maxZoom: 5,
      noWrap: true,
      attribution: 'Grand Theft Auto V Satellite Imagery · San Andreas',
      errorTileUrl: '/mapStyles/styleSatelite/empty.jpg',
    }),
    grid: L.tileLayer('/mapStyles/styleGrid/{z}/{x}/{y}.png', {
      minZoom: 0,
      maxZoom: 5,
      noWrap: true,
      attribution: 'Grand Theft Auto V Topographic Grid · San Andreas',
      errorTileUrl: '/mapStyles/styleAtlas/empty.jpg',
    }),
  };

  gtaMap = L.map(container, {
    crs,
    minZoom: 1,
    maxZoom: 5,
    zoom: 3,
    center: [200, 0], // [Y, X] in GTA V world coordinates
    layers: [tileLayers.atlas],
    zoomControl: false,
    preferCanvas: true,
    maxBounds: L.latLngBounds(L.latLng(-4600, -6000), L.latLng(8600, 6800)),
    maxBoundsViscosity: 0.9,
  });

  updateMapBackground('atlas');

  layerGroups = {
    pois: L.layerGroup().addTo(gtaMap),
    blips: L.layerGroup().addTo(gtaMap),
    vehicles: L.layerGroup().addTo(gtaMap),
    players: L.layerGroup().addTo(gtaMap),
    admins: L.layerGroup().addTo(gtaMap),
  };

  populateVerifiedLandmarksLayer();

  // Live cursor coordinate tracking in GTA V world units (X = lng, Y = lat)
  gtaMap.on('mousemove', e => {
    const wx = e.latlng.lng;
    const wy = e.latlng.lat;
    const el = document.getElementById('cursor-coords');
    if (el) {
      el.textContent = `X: ${wx.toFixed(1)} · Y: ${wy.toFixed(1)}`;
    }
  });

  // Disable follow-lock when user manually drags the map
  gtaMap.on('dragstart', () => {
    if (followLocal) {
      followLocal = false;
      updateFollowButtonUI();
    }
  });

  // Click anywhere on the GTA V map to place a precision waypoint
  gtaMap.on('click', e => {
    const wx = e.latlng.lng;
    const wy = e.latlng.lat;
    const region = resolveGtaRegion(wx, wy);
    setSelectedWaypoint(wx, wy, 35.0, `Waypoint (${region})`);
  });

  requestAnimationFrame(() => {
    gtaMap.invalidateSize();
  });
}

function updateMapBackground(style) {
  const color = STYLE_OCEAN_COLORS[style] || '#384950';
  const stage = document.getElementById('map-viewport');
  const mapEl = document.getElementById('gta-leaflet-map');
  if (stage) stage.style.backgroundColor = color;
  if (mapEl) mapEl.style.backgroundColor = color;
}

function setMapStyle(style) {
  if (!gtaMap || !tileLayers[style] || style === currentMapStyle) return;

  gtaMap.removeLayer(tileLayers[currentMapStyle]);
  tileLayers[style].addTo(gtaMap);
  currentMapStyle = style;
  updateMapBackground(style);

  ['atlas', 'satellite', 'grid'].forEach(s => {
    const btn = document.getElementById(`style-btn-${s}`);
    if (btn) btn.classList.toggle('active', s === style);
  });
}

function populateVerifiedLandmarksLayer() {
  if (!layerGroups.pois) return;
  layerGroups.pois.clearLayers();

  const poiIcon = L.divIcon({
    className: 'gta-pin-wrapper',
    html: '<div class="gta-marker-poi"></div>',
    iconSize: [10, 10],
    iconAnchor: [5, 5],
  });

  VERIFIED_GTA_LOCATIONS.forEach(loc => {
    const marker = L.marker([loc.y, loc.x], { icon: poiIcon });
    const popupHtml = `
      <div>
        <div class="popup-title">${escapeHtml(loc.name)}</div>
        <div class="popup-coords">${escapeHtml(loc.region)}<br/>X: ${loc.x.toFixed(1)}, Y: ${loc.y.toFixed(1)}, Z: ${loc.z.toFixed(1)}</div>
        <button type="button" class="btn-primary btn-xs" onclick="teleportCoords(${loc.x}, ${loc.y}, ${loc.z}, '${escapeAttr(loc.name)}')">
          Dispatch Teleport
        </button>
      </div>
    `;
    marker.bindPopup(popupHtml);
    marker.addTo(layerGroups.pois);
  });
}

function renderLandmarkDirectory() {
  const container = document.getElementById('landmark-directory');
  if (!container) return;

  const query = (document.getElementById('poi-search')?.value || '').trim().toLowerCase();
  const filtered = VERIFIED_GTA_LOCATIONS.filter(loc =>
    !query ||
    loc.name.toLowerCase().includes(query) ||
    loc.region.toLowerCase().includes(query)
  );

  const countEl = document.getElementById('poi-count-label');
  if (countEl) countEl.textContent = `${filtered.length} verified`;

  if (filtered.length === 0) {
    container.innerHTML = '<div class="standby-notice">No GTA V locations match your filter.</div>';
    return;
  }

  container.innerHTML = filtered.map(loc => `
    <div class="landmark-row" role="listitem">
      <div class="landmark-info" onclick="focusMapOnCoords(${loc.x}, ${loc.y}, 4, '${escapeAttr(loc.name)}', ${loc.z})" title="Pan map to ${escapeAttr(loc.name)}">
        <div class="landmark-name">${escapeHtml(loc.name)}</div>
        <div class="landmark-sub">${escapeHtml(loc.region)} · ${loc.x.toFixed(0)}, ${loc.y.toFixed(0)}</div>
      </div>
      <div class="landmark-btns">
        <button type="button" class="btn-secondary btn-xs" onclick="focusMapOnCoords(${loc.x}, ${loc.y}, 4, '${escapeAttr(loc.name)}', ${loc.z})">View</button>
        <button type="button" class="btn-primary btn-xs" onclick="teleportCoords(${loc.x}, ${loc.y}, ${loc.z}, '${escapeAttr(loc.name)}')">TP</button>
      </div>
    </div>
  `).join('');
}

function focusMapOnCoords(x, y, zoom = 4, label = null, z = 35.0) {
  if (!gtaMap) return;
  followLocal = false;
  updateFollowButtonUI();
  gtaMap.flyTo([y, x], zoom, { duration: 0.45 });
  if (label) {
    setSelectedWaypoint(x, y, z, label);
  }
}

function setSelectedWaypoint(x, y, z = 35.0, label = 'Selected Waypoint') {
  selectedWaypoint = { x, y, z, label };

  if (gtaMap) {
    const wpIcon = L.divIcon({
      className: 'gta-pin-wrapper',
      html: '<div class="gta-marker-waypoint"></div>',
      iconSize: [16, 16],
      iconAnchor: [8, 8],
    });
    if (waypointMarker) {
      waypointMarker.setLatLng([y, x]);
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

function copyWaypointCoords() {
  if (!selectedWaypoint) return;
  const text = `${selectedWaypoint.x.toFixed(2)}, ${selectedWaypoint.y.toFixed(2)}, ${selectedWaypoint.z.toFixed(2)}`;
  if (navigator.clipboard) {
    navigator.clipboard.writeText(text);
  }
}

function teleportToSelectedWaypoint() {
  if (!selectedWaypoint) return;
  teleportCoords(selectedWaypoint.x, selectedWaypoint.y, selectedWaypoint.z, selectedWaypoint.label);
}

function teleportCoords(x, y, z, label = 'Coordinates') {
  setSelectedWaypoint(x, y, z, label);
  fetch(`/api/teleport?x=${x.toFixed(2)}&y=${y.toFixed(2)}&z=${z.toFixed(2)}`)
    .then(r => r.json())
    .then(() => {
      const lbl = document.getElementById('wp-label');
      if (lbl) lbl.textContent = `DISPATCHED → ${label.toUpperCase()}`;
      pollTelemetry();
    })
    .catch(() => {});
}

// ============================================================================
// Camera & Layer Controls
// ============================================================================
function zoomMapIn() {
  if (gtaMap) gtaMap.zoomIn();
}

function zoomMapOut() {
  if (gtaMap) gtaMap.zoomOut();
}

function recenterOnLocal() {
  if (!gtaMap) return;
  if (localPlayer && typeof localPlayer.x === 'number') {
    gtaMap.flyTo([localPlayer.y, localPlayer.x], Math.max(gtaMap.getZoom(), 4), { duration: 0.35 });
  } else if (selectedWaypoint) {
    gtaMap.flyTo([selectedWaypoint.y, selectedWaypoint.x], 4, { duration: 0.35 });
  } else {
    // Center on Downtown Los Santos (Legion Square)
    gtaMap.flyTo([-810.2, 215.4], 4, { duration: 0.35 });
  }
}

function resetFullView() {
  if (!gtaMap) return;
  followLocal = false;
  updateFollowButtonUI();
  gtaMap.flyTo([800, 0], 2, { duration: 0.4 });
}

function toggleFollowLocal() {
  followLocal = !followLocal;
  updateFollowButtonUI();
  if (followLocal && localPlayer) {
    recenterOnLocal();
  }
}

function updateFollowButtonUI() {
  const lbl = document.getElementById('lbl-follow');
  const chip = document.getElementById('chip-follow');
  if (lbl) lbl.textContent = followLocal ? 'ON' : 'OFF';
  if (chip) chip.classList.toggle('active', followLocal);
}

function toggleLayer(name) {
  if (!(name in layersState)) return;
  layersState[name] = !layersState[name];

  const chip = document.getElementById(`chip-${name}`);
  if (chip) chip.classList.toggle('active', layersState[name]);

  if (gtaMap && layerGroups[name]) {
    if (layersState[name]) {
      layerGroups[name].addTo(gtaMap);
    } else {
      gtaMap.removeLayer(layerGroups[name]);
    }
  }
}

function toggleInspectorSidebar() {
  const sb = document.getElementById('inspector-sidebar');
  if (!sb) return;
  sb.classList.toggle('collapsed');
  if (gtaMap) {
    setTimeout(() => gtaMap.invalidateSize(), 150);
  }
}

// ============================================================================
// Navigation & View Switching
// ============================================================================
function switchTab(tabId, btnEl) {
  activeTab = tabId;
  document.querySelectorAll('.nav-tab').forEach(b => {
    const isCurrent = b.getAttribute('data-tab') === tabId;
    b.classList.toggle('active', isCurrent);
    if (isCurrent) b.setAttribute('aria-current', 'page');
    else b.removeAttribute('aria-current');
  });

  document.querySelectorAll('.view-panel').forEach(p => {
    p.classList.toggle('active', p.id === `tab-${tabId}`);
  });

  if (tabId === 'map' && gtaMap) {
    requestAnimationFrame(() => gtaMap.invalidateSize());
  }
}

// Keyboard shortcuts (1-4 for views, Escape to clear waypoint)
window.addEventListener('keydown', e => {
  const tag = (e.target?.tagName || '').toLowerCase();
  if (tag === 'input' || tag === 'textarea' || tag === 'select') return;

  if (e.key === '1') switchTab('map');
  else if (e.key === '2') switchTab('config');
  else if (e.key === '3') switchTab('admins');
  else if (e.key === '4') switchTab('entities');
  else if (e.key === 'Escape') clearWaypoint();
});

// ============================================================================
// Runtime Configuration & Watchlist API Sync
// ============================================================================
function setConfigValue(key, val) {
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
          if (el.type === 'checkbox') el.checked = Boolean(v);
          else el.value = v;
        }
        const qEl = document.getElementById(`quick-${k}`);
        if (qEl && qEl.type === 'checkbox') qEl.checked = Boolean(v);
      }
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
        <span class="watch-chip">
          <span>${escapeHtml(name)}</span>
          <button type="button" onclick="removeAdminWatch('${escapeAttr(name)}')" title="Remove identifier" aria-label="Remove ${escapeAttr(name)}">✕</button>
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
    })
    .catch(() => {});
}

function removeAdminWatch(name) {
  fetch(`/api/admin/remove?name=${encodeURIComponent(name)}`)
    .then(() => fetchWatchlist())
    .catch(() => {});
}

// ============================================================================
// Live Telemetry Polling & Map Marker Synchronization
// ============================================================================
function syncTelemetryMarkers() {
  if (!gtaMap) return;

  // 1. Local Player Marker
  if (localPlayer && typeof localPlayer.x === 'number' && typeof localPlayer.y === 'number') {
    const selfIcon = L.divIcon({
      className: 'gta-pin-wrapper',
      html: '<div class="gta-marker-self" title="Local Player"></div>',
      iconSize: [18, 18],
      iconAnchor: [9, 9],
    });
    if (localPlayerMarker) {
      localPlayerMarker.setLatLng([localPlayer.y, localPlayer.x]);
    } else {
      localPlayerMarker = L.marker([localPlayer.y, localPlayer.x], { icon: selfIcon, zIndexOffset: 1000 }).addTo(gtaMap);
      localPlayerMarker.bindPopup('<div class="popup-title">Local Player</div>');
    }
  } else if (localPlayerMarker) {
    gtaMap.removeLayer(localPlayerMarker);
    localPlayerMarker = null;
  }

  // 2. Session Players & Flagged Admins
  layerGroups.players.clearLayers();
  layerGroups.admins.clearLayers();

  const adminIcon = L.divIcon({
    className: 'gta-pin-wrapper',
    html: '<div class="gta-marker-admin"></div>',
    iconSize: [16, 16],
    iconAnchor: [8, 8],
  });

  const playerIcon = L.divIcon({
    className: 'gta-pin-wrapper',
    html: '<div class="gta-marker-player"></div>',
    iconSize: [13, 13],
    iconAnchor: [6, 6],
  });

  playersList.forEach(p => {
    const isAdm = Boolean(p.is_admin);
    const marker = L.marker([p.y, p.x], {
      icon: isAdm ? adminIcon : playerIcon,
      zIndexOffset: isAdm ? 900 : 500,
    });
    const popupHtml = `
      <div>
        <div class="popup-title">${isAdm ? '[WATCHLIST] ' : ''}${escapeHtml(p.name || 'Player')}</div>
        <div class="popup-coords">
          ID #${p.id ?? 0} · ${Math.round(p.dist || 0)}m<br/>
          HP: ${Math.round(p.health || 0)} · ARM: ${Math.round(p.armor || 0)}<br/>
          X: ${Number(p.x).toFixed(1)}, Y: ${Number(p.y).toFixed(1)}, Z: ${Number(p.z).toFixed(1)}
        </div>
        <button type="button" class="btn-primary btn-xs" onclick="teleportCoords(${Number(p.x)}, ${Number(p.y)}, ${Number(p.z)}, '${escapeAttr(p.name || 'Player')}')">
          Teleport To Entity
        </button>
      </div>
    `;
    marker.bindPopup(popupHtml);
    marker.addTo(isAdm ? layerGroups.admins : layerGroups.players);
  });

  // 3. Session Vehicles
  layerGroups.vehicles.clearLayers();
  const vehIcon = L.divIcon({
    className: 'gta-pin-wrapper',
    html: '<div class="gta-marker-vehicle"></div>',
    iconSize: [11, 11],
    iconAnchor: [5, 5],
  });

  vehiclesList.forEach(v => {
    const marker = L.marker([v.y, v.x], { icon: vehIcon });
    marker.bindPopup(`
      <div>
        <div class="popup-title">${escapeHtml(v.name || 'Vehicle')}</div>
        <div class="popup-coords">${Math.round(v.dist || 0)}m · X: ${Number(v.x).toFixed(0)}, Y: ${Number(v.y).toFixed(0)}</div>
        <button type="button" class="btn-primary btn-xs" onclick="teleportCoords(${Number(v.x)}, ${Number(v.y)}, ${Number(v.z || 35)}, '${escapeAttr(v.name || 'Vehicle')}')">
          Teleport To Vehicle
        </button>
      </div>
    `);
    marker.addTo(layerGroups.vehicles);
  });

  // 4. Native GTA Map Blips (m_BlipList)
  layerGroups.blips.clearLayers();
  const blipIcon = L.divIcon({
    className: 'gta-pin-wrapper',
    html: '<div class="gta-marker-poi" style="background:#10b981;"></div>',
    iconSize: [10, 10],
    iconAnchor: [5, 5],
  });

  mapBlipsList.forEach(b => {
    const marker = L.marker([b.y, b.x], { icon: blipIcon });
    marker.bindPopup(`
      <div>
        <div class="popup-title">${b.sprite === 8 ? 'In-Game Waypoint' : 'Blip Sprite #' + b.sprite}</div>
        <div class="popup-coords">X: ${Number(b.x).toFixed(1)}, Y: ${Number(b.y).toFixed(1)}, Z: ${Number(b.z).toFixed(1)}</div>
        <button type="button" class="btn-primary btn-xs" onclick="teleportCoords(${Number(b.x)}, ${Number(b.y)}, ${Number(b.z || 35)}, 'Game Blip')">
          Teleport Here
        </button>
      </div>
    `);
    marker.addTo(layerGroups.blips);
  });
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

      // Update Connection Status Pill
      const dot = document.getElementById('conn-dot');
      const txt = document.getElementById('conn-text');
      if (isConnectedToGame) {
        dot.classList.add('live');
        txt.textContent = data.source === 'cpp_push'
          ? 'Connected · Render Cloud Sync'
          : 'Connected · 127.0.0.1:8080';
      } else {
        dot.classList.remove('live');
        txt.textContent = 'Standby · Awaiting p4m.exe';
      }

      const admins = playersList.filter(p => p.is_admin);
      document.getElementById('cnt-admins').textContent = admins.length;
      document.getElementById('cnt-players').textContent = playersList.length;
      document.getElementById('cnt-vehicles').textContent = vehiclesList.length;
      document.getElementById('cnt-blips').textContent = mapBlipsList.length;
      document.getElementById('nav-admin-count').textContent = admins.length;
      document.getElementById('nav-entity-count').textContent = playersList.length;
      document.getElementById('hud-admins').textContent = `${admins.length} flagged`;

      // Priority Admin Banner
      const alertBar = document.getElementById('admin-alert-bar');
      if (admins.length > 0) {
        alertBar.style.display = 'block';
        document.getElementById('admin-alert-names').textContent =
          admins.map(a => `${a.name} (${Math.round(a.dist || 0)}m)`).join(' · ');
      } else {
        alertBar.style.display = 'none';
      }

      // Local Player Telemetry Readout
      if (localPlayer && typeof localPlayer.x === 'number') {
        document.getElementById('hud-x').textContent = localPlayer.x.toFixed(1);
        document.getElementById('hud-y').textContent = localPlayer.y.toFixed(1);
        document.getElementById('hud-z').textContent = localPlayer.z.toFixed(1);
        document.getElementById('hud-hp').textContent = Math.round(localPlayer.health || 0);
        document.getElementById('hud-arm').textContent = Math.round(localPlayer.armor || 0);
        document.getElementById('bar-hp').style.width = `${Math.min(100, Math.max(0, ((localPlayer.health || 0) / 200) * 100))}%`;
        document.getElementById('bar-arm').style.width = `${Math.min(100, Math.max(0, localPlayer.armor || 0))}%`;
        document.getElementById('hud-zone').textContent = resolveGtaRegion(localPlayer.x, localPlayer.y);

        if (followLocal && gtaMap) {
          gtaMap.panTo([localPlayer.y, localPlayer.x], { animate: true });
        }
      } else {
        document.getElementById('hud-x').textContent = '—';
        document.getElementById('hud-y').textContent = '—';
        document.getElementById('hud-z').textContent = '—';
        document.getElementById('hud-hp').textContent = '—';
        document.getElementById('hud-arm').textContent = '—';
        document.getElementById('bar-hp').style.width = '0%';
        document.getElementById('bar-arm').style.width = '0%';
        document.getElementById('hud-zone').textContent = selectedWaypoint
          ? resolveGtaRegion(selectedWaypoint.x, selectedWaypoint.y)
          : 'San Andreas';
      }

      syncTelemetryMarkers();
      renderProximityFeed();
      renderTables(admins);
    })
    .catch(() => {});
}

function renderProximityFeed() {
  const container = document.getElementById('proximity-list');
  if (!container) return;

  if (!isConnectedToGame) {
    container.innerHTML = `
      <div class="standby-notice">
        <strong>Telemetry Link in Standby</strong>
        No active FiveM process is currently streaming entities. Click any location on the GTA V map or select a landmark above to queue a teleport waypoint.
      </div>
    `;
    return;
  }

  if (playersList.length === 0) {
    container.innerHTML = '<div class="standby-notice">Connected to FiveM process — 0 remote players within radar cull radius.</div>';
    return;
  }

  const sorted = [...playersList].sort((a, b) => (Number(b.is_admin) - Number(a.is_admin)) || ((a.dist || 0) - (b.dist || 0)));
  container.innerHTML = sorted.map(p => `
    <div class="prox-item ${p.is_admin ? 'is-admin' : ''}">
      <div>
        <div style="font-weight:600;color:var(--text-primary);">
          ${p.is_admin ? '<span class="role-badge admin" style="margin-right:6px;">ADMIN</span>' : ''}${escapeHtml(p.name || 'Player')}
        </div>
        <div class="prox-meta">#${p.id ?? 0} · ${Math.round(p.dist || 0)}m · ${escapeHtml(p.weapon || 'Unarmed')}</div>
      </div>
      <button type="button" class="btn-secondary btn-xs" onclick="teleportCoords(${Number(p.x)}, ${Number(p.y)}, ${Number(p.z)}, '${escapeAttr(p.name || 'Player')}')">TP</button>
    </div>
  `).join('');
}

function renderTables(admins) {
  const adminBody = document.getElementById('admins-table-body');
  if (adminBody) {
    if (!admins.length) {
      adminBody.innerHTML = '<tr><td colspan="8" class="empty-cell">No flagged watchlist entities detected in the current session.</td></tr>';
    } else {
      adminBody.innerHTML = admins.map(a => `
        <tr>
          <td><span class="role-badge admin">WATCHLIST</span></td>
          <td><strong>${escapeHtml(a.name || 'Unknown')}</strong></td>
          <td class="mono">#${a.id ?? 0}</td>
          <td class="mono">${Number(a.x).toFixed(1)}, ${Number(a.y).toFixed(1)}, ${Number(a.z).toFixed(1)}</td>
          <td class="mono">${Math.round(a.dist || 0)}m</td>
          <td class="mono">${Math.round(a.health || 0)} HP / ${Math.round(a.armor || 0)} ARM</td>
          <td class="mono">${escapeHtml(a.weapon || 'Unarmed')}</td>
          <td class="text-right">
            <button type="button" class="btn-primary btn-xs" onclick="teleportCoords(${Number(a.x)}, ${Number(a.y)}, ${Number(a.z)}, '${escapeAttr(a.name || 'Admin')}')">Teleport</button>
          </td>
        </tr>
      `).join('');
    }
  }

  const entBody = document.getElementById('entities-table-body');
  if (entBody) {
    if (!isConnectedToGame) {
      entBody.innerHTML = '<tr><td colspan="9" class="empty-cell">Telemetry standby — attach p4m.exe locally (127.0.0.1:8080) or enable Render Cloud Sync (/api/sync) to view live session players.</td></tr>';
    } else if (!playersList.length) {
      entBody.innerHTML = '<tr><td colspan="9" class="empty-cell">Connected to process — no remote players in range.</td></tr>';
    } else {
      entBody.innerHTML = playersList.map(p => `
        <tr>
          <td>${p.is_admin ? '<span class="role-badge admin">WATCHLIST</span>' : '<span class="role-badge player">PLAYER</span>'}</td>
          <td><strong>${escapeHtml(p.name || 'Player')}</strong></td>
          <td class="mono">#${p.id ?? 0}</td>
          <td class="mono">${Number(p.x).toFixed(1)}, ${Number(p.y).toFixed(1)}, ${Number(p.z).toFixed(1)}</td>
          <td class="mono">${Math.round(p.dist || 0)}m</td>
          <td class="mono">${Math.round(p.health || 0)}</td>
          <td class="mono">${Math.round(p.armor || 0)}</td>
          <td class="mono">${escapeHtml(p.weapon || 'Unarmed')}</td>
          <td class="text-right">
            <button type="button" class="btn-primary btn-xs" onclick="teleportCoords(${Number(p.x)}, ${Number(p.y)}, ${Number(p.z)}, '${escapeAttr(p.name || 'Player')}')">Teleport</button>
          </td>
        </tr>
      `).join('');
    }
  }
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
// Application Bootstrap
// ============================================================================
window.addEventListener('DOMContentLoaded', () => {
  initGtaMap();
  renderLandmarkDirectory();
  fetchConfig();
  fetchWatchlist();
  pollTelemetry();
  setInterval(pollTelemetry, 350);
});
