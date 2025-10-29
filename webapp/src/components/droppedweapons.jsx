import { useRef } from "react";
import { getRadarPosition, teamEnum, colorSchemePallette, weaponList } from "../utilities/utilities";
import MaskedIcon from "./maskedicon";

const DroppedWeapon = ({ droppedWeaponData, mapData, settings, averageLatency, radarImage }) => {

  const radarPosition = getRadarPosition(mapData, { x: droppedWeaponData.m_x, y: droppedWeaponData.m_y });
  
  const wepRef = useRef();
  const wepBounding = (wepRef.current &&
    wepRef.current.getBoundingClientRect()) || { width: 0, height: 0 };
  const radarImageBounding = (radarImage !== undefined &&
    radarImage.getBoundingClientRect()) || { width: 0, height: 0 };
  const radarImageTranslation = {
    x: radarImageBounding.width * radarPosition.x - wepBounding.width * 0.5,
    y: radarImageBounding.width * radarPosition.y - wepBounding.height * 0.5,
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