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

export const calculatePositionWithScale = (radarImage, radarPosition) => {
  if (!radarImage) return [0, 0];

  const width = radarImage.offsetWidth;
  const height = radarImage.offsetHeight;
  const style = radarImage.style;
  const transformString = style.transform || "";

  let transformX = 0;
  let transformY = 0;
  const transMatch = transformString.match(/translate\(\s*([+-]?\d+(\.\d+)?)px,\s*([+-]?\d+(\.\d+)?)px\)/);
  if (transMatch) {
    transformX = Number(transMatch[1]);
    transformY = Number(transMatch[3]);
  }

  let radarScale = 1;
  if (style.scale) {
    const s = Number(style.scale);
    if (!isNaN(s)) radarScale = s;
  }

  let angleRad = 0;
  let rotStr = style.rotate;

  if (!rotStr && transformString.includes("rotate")) {
    const rotMatch = transformString.match(/rotate\((.*?)\)/);
    if (rotMatch) rotStr = rotMatch[1];
  }

  if (rotStr) {
    const s = rotStr.trim();
    angleRad = Number(s.slice(0, -3)) * (Math.PI / 180);
    if (!isFinite(angleRad)) angleRad = 0;
  }

  const rotateIndex = transformString.indexOf("rotate");
  const translateIndex = transformString.indexOf("translate");
  const scaleIndex = transformString.indexOf("scale");

  const isRotateAfterTranslate = style.rotate || (rotateIndex !== -1 && translateIndex !== -1 && rotateIndex < translateIndex);

  const isScaleAfterTranslate = style.scale || (scaleIndex !== -1 && translateIndex !== -1 && scaleIndex > translateIndex);

  const centerX = width / 2;
  const centerY = height / 2;

  const localX = (width * radarPosition.x) - centerX;
  const localY = (height * radarPosition.y) - centerY;

  const scaledLocalX = localX * radarScale;
  const scaledLocalY = localY * radarScale;

  const cos = Math.cos(angleRad);
  const sin = Math.sin(angleRad);

  const rotatedPointX = (scaledLocalX * cos) - (scaledLocalY * sin);
  const rotatedPointY = (scaledLocalX * sin) + (scaledLocalY * cos);

  let finalTransformX = transformX;
  let finalTransformY = transformY;

  if (isScaleAfterTranslate) {
    finalTransformX *= radarScale;
    finalTransformY *= radarScale;
  }

  if (isRotateAfterTranslate) {
    const tx = finalTransformX;
    const ty = finalTransformY;
    finalTransformX = (tx * cos) - (ty * sin);
    finalTransformY = (tx * sin) + (ty * cos);
  }

  const finalX = centerX + rotatedPointX + finalTransformX;
  const finalY = centerY + rotatedPointY + finalTransformY;

  return [finalX, finalY];
};

export const calculateMapOffsetForCentering = (playerPosition, radarImage, mapData) => {
  if (!playerPosition || !radarImage) {
    return { x: 0, y: 0 };
  }

  const radarPosition = getRadarPosition(mapData, playerPosition);
  if (!radarPosition) {
    return { x: 0, y: 0 };
  }

  const width = radarImage.offsetWidth;
  const height = radarImage.offsetHeight;
  const style = radarImage.style;
  const transformString = style.transform || "";

  let angleRad = 0;
  let rotStr = style.rotate;

  if (!rotStr && transformString.includes("rotate")) {
    const rotMatch = transformString.match(/rotate\((.*?)\)/);
    if (rotMatch) rotStr = rotMatch[1];
  }

  if (rotStr) {
    const s = rotStr.trim();
    angleRad = Number(s.slice(0, -3)) * (Math.PI / 180);
    if (!isFinite(angleRad)) angleRad = 0;
  }

  const rotateIndex = transformString.indexOf("rotate");
  const translateIndex = transformString.indexOf("translate");
  const isRotateFirst = style.rotate || (rotateIndex !== -1 && translateIndex !== -1 && rotateIndex < translateIndex);

  const mapCenterX = width / 2;
  const mapCenterY = height / 2;

  const playerPixelX = width * radarPosition.x;
  const playerPixelY = height * radarPosition.y;

  const vecX = playerPixelX - mapCenterX;
  const vecY = playerPixelY - mapCenterY;
  
  if (isRotateFirst) {
    return { x: -vecX, y: -vecY };
  } else {
    const cos = Math.cos(angleRad);
    const sin = Math.sin(angleRad);

    const rotatedVecX = (vecX * cos) - (vecY * sin);
    const rotatedVecY = (vecX * sin) + (vecY * cos);

    return { x: -rotatedVecX, y: -rotatedVecY };
  }
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