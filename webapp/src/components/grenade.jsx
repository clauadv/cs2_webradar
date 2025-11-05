import { getRadarPosition } from "../utilities/utilities";

const R = { smoke: 144, he: 350, molotov: 240, flashbang: 0 };
const C = { smoke: "#9ca3af", he: "#ef4444", molotov: "#f97316", flashbang: "#fbbf24" };

const Grenade = ({ grenadeData, mapData, radarImage }) => {
  if (!radarImage || !grenadeData || !grenadeData.m_type) return null;
  const c = C[grenadeData.m_type];
  if (!c) return null;
  const pos = getRadarPosition(mapData, { x: grenadeData.m_pos_x, y: grenadeData.m_pos_y });
  const rPos = { x: pos.x * radarImage.width, y: pos.y * radarImage.height };
  const r = R[grenadeData.m_type] || 0;
  const rPx = (r / mapData.scale) * (radarImage.width / 1024);
  return (
    <>
      {r > 0 && <div className="absolute rounded-full pointer-events-none" style={{ left: `${rPos.x}px`, top: `${rPos.y}px`, width: `${rPx * 2}px`, height: `${rPx * 2}px`, transform: "translate(-50%, -50%)", backgroundColor: c, opacity: 0.2, border: `2px solid ${c}`, borderOpacity: 0.5 }} />}
      <div className="absolute w-2.5 h-2.5 rounded-full" style={{ left: `${rPos.x}px`, top: `${rPos.y}px`, transform: "translate(-50%, -50%)", backgroundColor: c, boxShadow: `0 0 8px ${c}` }} />
    </>
  );
};

export default Grenade;
