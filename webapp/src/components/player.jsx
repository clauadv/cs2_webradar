import { memo, useEffect, useRef, useState } from "react";
import { getRadarPosition, playerColors } from "../utilities/utilities";

// Unwrap angle, shortest rotation
const playerRotations = new Map();

const calculatePlayerRotation = (playerData) => {
  const target = 270 - playerData.m_eye_angle;
  const idx = playerData.m_idx;

  let current = (playerRotations.get(idx) || 0) % 360;
  current += ((target - current + 540) % 360) - 180;
  playerRotations.set(idx, current);

  return current;
};

const Player = ({ playerData, mapData, localTeam, settings }) => {
  const radarPosition = getRadarPosition(mapData, playerData.m_position);
  const invalidPosition = radarPosition.x <= 0 && radarPosition.y <= 0;

  // Death marker stays put
  const deathPositionRef = useRef(null);
  const [deathPosition, setDeathPosition] = useState(null);

  useEffect(() => {
    if (playerData.m_is_dead) {
      if (!deathPositionRef.current) {
        deathPositionRef.current = radarPosition;
        setDeathPosition(radarPosition);
      }
    } else if (deathPositionRef.current) {
      deathPositionRef.current = null;
      setDeathPosition(null);
    }
    // Snapshot on death
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [playerData.m_is_dead]);

  if (playerData.m_is_dead && !settings.showDeadOnRadar) return null;

  const position = playerData.m_is_dead
    ? deathPosition || radarPosition
    : radarPosition;

  const size = 1.4 * settings.dotSize; // % of the radar's width
  const rotation = playerData.m_is_dead ? 0 : calculatePlayerRotation(playerData);
  const isLocalTeam = playerData.m_team === localTeam;
  const color = (isLocalTeam && playerColors[playerData.m_color]) || "#e0453b";

  const opacity = (playerData.m_is_dead && 0.65) || (invalidPosition && 0) || 1;

  return (
    <div
      // Percentages, no layout reads
      className="radar-entity absolute origin-center"
      style={{
        left: `${position.x * 100}%`,
        top: `${position.y * 100}%`,
        width: `${size}%`,
        aspectRatio: "1 / 1",
        transform: "translate(-50%, -50%)",
        zIndex: playerData.m_is_dead ? 1 : 2,
        opacity,
        WebkitMask: playerData.m_is_dead
          ? `url('./assets/icons/icon-enemy-death_png.png') no-repeat center / contain`
          : "none",
        mask: playerData.m_is_dead
          ? `url('./assets/icons/icon-enemy-death_png.png') no-repeat center / contain`
          : "none",
        backgroundColor: playerData.m_is_dead ? color : "transparent",
      }}
      title={playerData.m_name}
    >
      {!playerData.m_is_dead && (
        <div
          className="radar-entity-rotor h-full w-full"
          style={{ transform: `rotate(${rotation}deg)` }}
        >
          {/* Teardrop direction marker */}
          <div
            className="h-full w-full rotate-[315deg] rounded-[50%_50%_50%_0%]"
            style={{
              backgroundColor: color,
              boxShadow: `0 0 0 1px rgba(3, 6, 11, 0.85), 0 0 8px -2px ${color}`,
            }}
          />
        </div>
      )}

      {/* Floating name & HP label */}
      {settings.showRadarLabels && !playerData.m_is_dead && !invalidPosition && (
        <span
          className="pointer-events-none absolute left-1/2 top-full flex -translate-x-1/2 translate-y-1 items-center gap-1 whitespace-nowrap rounded bg-base/90 px-1 py-px text-[9px] font-semibold leading-none text-content-primary"
        >
          <span className="max-w-[80px] overflow-hidden text-ellipsis">{playerData.m_name}</span>
          <span className="tnum" style={{ color }}>
            {playerData.m_health}
          </span>
        </span>
      )}
    </div>
  );
};

export default memo(Player);
