export const getRadarPosition = (mapData, entityCoords) => {
  const divide = 1024;

  if (!entityCoords.x || !entityCoords.y) {
    return { x: 0, y: 0 };
  }

  if (!mapData.x || !mapData.y) {
    return { x: 0, y: 0 };
  }

  const position = {
    x: (entityCoords.x - mapData.x) / mapData.scale / divide,
    y: (((entityCoords.y - mapData.y) / mapData.scale) * -1.0) / divide,
  };

  return position;
};

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

export const weaponList = {
  "pistols": [
    "cs75a",
    "deagle",
    "elite", //dual berettas
    "fiveseven",
    "glock",
    "hkp2000", //p2000
    "p250",
    "revolver",
    "tec9",
    "usp-silencer",
    "usp-silencer-off"
  ],
  "rifles": [
    "ak47",
    "aug",
    "awp",
    "famas",
    "g3sg1",
    "galilar",
    "m4a1",
    "m4a1-silencer",
    "m4a1-silencer-off",
    "scar20",
    "sg556",
    "ssg08"
  ],
  "smgs": [
    "mac10",
    "mp5sd",
    "mp7",
    "mp9",
    "bizon",
    "p90",
    "ump45"
  ],
  "heavys": [
    "mag7",
    "nova",
    "sawedoff",
    "xm1014",
    "m249",
    "negev"
  ],
  "grenades": [
    "smokegrenade",
    "molotov",
    "incgrenade",
    "hegrenade",
    "flashbang",
    "decoy",
  ]
}

export const teamEnum = {
  none: 0,
  spectator: 1,
  terrorist: 2,
  counterTerrorist: 3,
};

export const colorSchemePallette = {
  undefined: [
    "#b1d0e7",
    "#6492b4"
  ],
  "default": [
    "#b1d0e7",
    "#6492b4"
  ],
  "white": [
    "#FFFFFF",
    "#a1a1a1ff"
  ],
  "light_blue": [
    "#32c8ffff",
    "#0183aaff"
  ],
  "dark_blue": [
    "#1685c0ff",
    "#005383ff"
  ],
  "purple": [
    "#cca0ffff",
    "#9933FF"
  ],
  "red": [
    "#ff6f6fff",
    "#CC3333"
  ],
  "orange": [
    "#ffbd60ff",
    "#FF8C00"
  ],

  "yellow": [
    "#ffe47aff",
    "#ffe600ff"
  ],

  "green": [
    "#71d492ff",
    "#00B050"
  ],
  "light_green": [
    "#a9ffa9ff",
    "#41ff41ff"
  ],
  "pink": [
    "#ff8fa0ff",
    "#ff3d9eff"
  ]
}