import { useRef, useState, useEffect } from "react";
import { getRadarPosition, playerColors, calculatePositionWithScale, calculateMapOffsetForCentering } from "../utilities/utilities";

let playerRotations = [];
const calculatePlayerRotation = (playerData) => {
  const playerViewAngle = 270 - playerData.m_eye_angle;
  const idx = playerData.m_idx;

  playerRotations[idx] = (playerRotations[idx] || 0) % 360;
  playerRotations[idx] +=
    ((playerViewAngle - playerRotations[idx] + 540) % 360) - 180;

  return playerRotations[idx];
};

const Player = ({ playerData, mapData, radarImage, localTeam, averageLatency, settings }) => {
  const [lastKnownPosition, setLastKnownPosition] = useState(null);
  const radarPosition = getRadarPosition(mapData, playerData.m_position) || { x: 0, y: 0 };
  const invalidPosition = radarPosition.x <= 0 && radarPosition.y <= 0;
  const playerRef = useRef();
  const playerBounding = (playerRef.current &&
    playerRef.current.getBoundingClientRect()) || { width: 0, height: 0 };
  const playerRotation = calculatePlayerRotation(playerData);
  const [scaledSize, setScaledSize] = useState(0.7 * settings.dotSize);

  useEffect(() => {
    if (window.innerHeight<=500) setScaledSize(0.7 * settings.dotSize+1.5); else setScaledSize(0.7 * settings.dotSize);
  }, [window.innerHeight])

  useEffect(() => {
    if (settings.showOnlyEnemies && playerData.m_team === localTeam) { 
      if (!settings.followYourself) {
        setScaledSize(0.0);
      } else if (settings.followYourself && playerData.m_steam_id!=settings.whichPlayerAreYou) {
        setScaledSize(0.0);
      } else {
        setScaledSize(0.7 * settings.dotSize);
      }
    if (settings.showAllNames) {settings.showAllNames = false;} } else { setScaledSize(0.7 * settings.dotSize) }
  }, [settings.showOnlyEnemies, settings.followYourself])
  
  useEffect(() => {
    if (playerData.m_is_dead) {
      if (!lastKnownPosition) {
        setLastKnownPosition(radarPosition);
      }
    } else {
      setLastKnownPosition(null);
    }
  }, [playerData.m_is_dead, radarPosition]);

  const effectivePosition = playerData.m_is_dead ? lastKnownPosition || { x: 0, y: 0 } : radarPosition;

  const scaledPos = calculatePositionWithScale(radarImage, effectivePosition);
  const radarImageTranslation = {
    x: (scaledPos[0] - playerBounding.width * 0.5),
    y: (scaledPos[1] - playerBounding.height * 0.5),
  };

  if (playerData.m_steam_id==settings.whichPlayerAreYou) {
    if (radarImage && mapData && playerData.m_position && settings.followYourself) {
      const radarOffset = calculateMapOffsetForCentering(playerData.m_position, radarImage, mapData);
      if (radarImage.getAttribute("isbeingdragged")=="false") {
        radarImage.setAttribute("moveoverride", "true")
        radarImage.setAttribute("newtransx", `${Math.floor(radarOffset.x)}`)
        radarImage.setAttribute("newtransy", `${Math.floor(radarOffset.y)}`)
      }
    } else if (radarImage) {
      radarImage.setAttribute("moveoverride", "false")
    }
  }

  return (
    <div
      className={`absolute origin-center rounded-[100%] left-0 top-0`}
      ref={playerRef}
      style={{
        width: `${scaledSize}vw`,
        height: `${scaledSize}vw`,
        transform: `translate(${radarImageTranslation.x}px, ${radarImageTranslation.y}px)`,
        transition: `transform ${averageLatency}ms linear`,
        zIndex: `${(playerData.m_is_dead && `0`) || `1`}`,
        WebkitMask: `${(playerData.m_is_dead && `url('./assets/icons/icon-enemy-death_png.png') no-repeat center / contain`) || `none`}`,
      }}
    >
      {/* Name above the dot - outside rotation container */}
      {(settings.showAllNames && playerData.m_team === localTeam) ||
        (settings.showEnemyNames && playerData.m_team !== localTeam) ? (
        <div className="absolute bottom-full left-1/2 -translate-x-1/2 -translate-y-1 text-center">
          <span className="text-xs text-white whitespace-nowrap max-w-[80px] inline-block overflow-hidden text-ellipsis">
            {playerData.m_name}
          </span>
        </div>
      ) : null}



      {/* Rotating container for player elements */}
      <div
        style={{
          transform: `rotate(${(playerData.m_is_dead && `0`) || playerRotation}deg)`,
          width: `${scaledSize}vw`,
          height: `${scaledSize}vw`,
          transition: `transform ${averageLatency}ms linear`,
          opacity: `${(playerData.m_is_dead && `0.8`) || (invalidPosition && `0`) || `1`}`,
        }}
      >
        {/* Player dot */}
        <div
          className={`w-full h-full rounded-[50%_50%_50%_0%] rotate-[315deg]`}
          style={{

            backgroundColor: `${(playerData.m_team == localTeam && playerColors[playerData.m_color]) || (settings.showOnlyEnemies && playerData.m_team != localTeam && playerColors[playerData.m_color]) || `red`}`,
            opacity: `${(playerData.m_is_dead && `0.8`) || (invalidPosition && `0`) || `1`}`,
            border: `${(playerData.m_team != localTeam && `1.5px solid white`) || `none`}`,
          }}
        />

        {/* View cone (kept exactly as it was) */}
        {(settings.showOnlyEnemies && playerData.m_team === localTeam && settings.showViewCones && !playerData.m_is_dead) || (settings.showViewCones && !playerData.m_is_dead) && (
          <div
            className="absolute left-1/2 top-1/2 w-[1.5vw] h-[3vw] bg-white opacity-30"
            style={{
              transform: `translate(-50%, 5%) rotate(0deg)`,
              clipPath: "polygon(50% 0%, 0% 100%, 100% 100%)",
            }}
          />
        )}
      </div>
      {(mapData.leveling && !playerData.m_is_dead) && (
        <img id="level_indicator"
          src={playerData.m_position.z < mapData.level_change ? `./assets/icons/down.png` : `./assets/icons/up.png`}
        />
      )}
    </div>
  );
};

export default Player;