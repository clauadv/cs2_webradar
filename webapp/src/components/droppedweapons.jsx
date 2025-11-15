import { useRef } from "react";
import { getRadarPosition, teamEnum, colorSchemePallette, weaponList, calculatePositionWithScale } from "../utilities/utilities";
import MaskedIcon from "./maskedicon";

const DroppedWeapon = ({ droppedWeaponData, mapData, settings, averageLatency, radarImage }) => {

  const radarPosition = getRadarPosition(mapData, { x: droppedWeaponData.m_x, y: droppedWeaponData.m_y });
  
  const wepRef = useRef();
  const wepBounding = (wepRef.current &&
    wepRef.current.getBoundingClientRect()) || { width: 0, height: 0 };
  const scaledPos = calculatePositionWithScale(radarImage, radarPosition);
  const radarImageTranslation = {
    x: (scaledPos[0] - wepBounding.width * 0.5),
    y: (scaledPos[1] - wepBounding.height * 0.5),
  };

return (
  <div
    ref={wepRef}
    className={`absolute left-0 top-0`}
    style={{
        transform: `translate(${radarImageTranslation.x}px, ${radarImageTranslation.y}px)`,
        transition: `transform ${averageLatency}ms linear`,
        opacity: `0.7`,
    }}
  >
  
  {(settings.droppedWeaponIgnoreNade && !weaponList["grenades"].includes(droppedWeaponData.m_name) || !settings.droppedWeaponIgnoreNade) &&
  
    <MaskedIcon
      path={`./assets/icons/${droppedWeaponData.m_name}.svg`}
      height={`${settings.droppedWeaponSize}vw`}
      color={`${settings.droppedWeaponGlow ? colorSchemePallette[settings.colorScheme][0] : colorSchemePallette[settings.colorScheme][1]}`}
    />
  }

  </div>
)

};

export default DroppedWeapon;