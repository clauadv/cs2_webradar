// Radar math

// World coords to radar space
// Formula fixed by contract
export const getRadarPosition = (mapData, entityCoords) => {
  if (!entityCoords || !entityCoords.x || !entityCoords.y) {
    return { x: 0, y: 0 };
  }

  if (!mapData || !mapData.x || !mapData.y) {
    return { x: 0, y: 0 };
  }

  return {
    x: (entityCoords.x - mapData.x) / mapData.scale / 1024,
    y: (((entityCoords.y - mapData.y) / mapData.scale) * -1.0) / 1024,
  };
};

// Game enums and palettes

export const playerColors = [
  // blue
  "#84c8ed",

  // green
  "#009a7d",

  // yellow
  "#eadd40",

  // orange
  "#df7d29",

  // purple
  "#b72b92",

  // white
  "#ffffff",
];

export const teamEnum = {
  none: 0,
  spectator: 1,
  terrorist: 2,
  counterTerrorist: 3,
};

// Team accents used across HUD
export const teamColors = {
  [teamEnum.terrorist]: "#de6338",
  [teamEnum.counterTerrorist]: "#4e9bf5",
};

export const getTeamColor = (team) => teamColors[team] || "#7387a0";

// Fuse length scales timer
export const BOMB_FUSE_SECONDS = 40;

// Settings

const SETTINGS_STORAGE_KEY = "radarSettings";

export const DEFAULT_SETTINGS = {
  // interface
  language: "en",

  // connection
  useLocalhost: true,
  targetIp: "",
  port: 22006,

  // radar display
  dotSize: 1,
  bombSize: 1,
  showDeadOnRadar: true,
  showRadarLabels: false,

  // player card display
  keepDeadCards: true,
  showMoney: true,
  showArmor: true,
  showWeapons: true,
  showSteamLink: true,
};

// Changing these reconnects the socket
export const CONNECTION_KEYS = ["useLocalhost", "targetIp", "port"];

// fallbackLanguage seeds a first-run language
export const loadSettings = (fallbackLanguage) => {
  const defaults = {
    ...DEFAULT_SETTINGS,
    ...(fallbackLanguage ? { language: fallbackLanguage } : {}),
  };

  try {
    const saved = localStorage.getItem(SETTINGS_STORAGE_KEY);
    if (!saved) return defaults;

    // Older saves gain defaults
    return { ...defaults, ...JSON.parse(saved) };
  } catch {
    return defaults;
  }
};

export const saveSettings = (settings) => {
  try {
    localStorage.setItem(SETTINGS_STORAGE_KEY, JSON.stringify(settings));
  } catch {
    // Storage disabled, stay in memory
  }
};

// Connection

// Builds ws://<host>:<port>/cs2_webradar
// Errors return translation key
export const resolveWebSocketURL = ({ useLocalhost, targetIp, port }) => {
  const resolvedPort = Number(port) || DEFAULT_SETTINGS.port;

  if (useLocalhost) {
    return {
      url: `ws://localhost:${resolvedPort}/cs2_webradar`,
      errorKey: null,
      errorValues: null,
    };
  }

  const ip = (targetIp || "").trim();

  if (ip.startsWith("192.168") || ip.startsWith("10.") || ip.startsWith("172.16.")) {
    return { url: null, errorKey: "connection.privateIp", errorValues: { ip } };
  }

  // Empty falls back to page host
  const host = !ip || /[a-zA-Z]/.test(ip) ? window.location.hostname : ip;

  if (!host) {
    return { url: null, errorKey: "connection.noHost", errorValues: null };
  }

  return {
    url: `ws://${host}:${resolvedPort}/cs2_webradar`,
    errorKey: null,
    errorValues: null,
  };
};

// Binary or string payload
export const readSocketPayload = async (data) => {
  if (typeof data === "string") return JSON.parse(data);
  if (data instanceof ArrayBuffer) return JSON.parse(new TextDecoder().decode(data));
  if (typeof Blob !== "undefined" && data instanceof Blob) return JSON.parse(await data.text());
  return JSON.parse(String(data));
};

// Helpers

export const clamp = (value, min, max) => Math.min(Math.max(value, min), max);

export const formatMoney = (amount, locale = "en-US") =>
  `$${Number(amount || 0).toLocaleString(locale)}`;

// Filled percent of track
export const trackFill = (value, min, max) => clamp(((value - min) / (max - min)) * 100, 0, 100);
