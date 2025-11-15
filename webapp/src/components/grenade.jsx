import { useRef } from "react";
import GrenadeEffects from "./grenadeeffects.jsx";
import { getRadarPosition, teamEnum, calculatePositionWithScale } from "../utilities/utilities";
import MaskedIcon from "./maskedicon.jsx";

const Grenade = ({ grenadeData, mapData, settings, averageLatency, radarImage, type }) => {

  const firePositions = grenadeData.m_firePositions || {};

  const radarPosition = getRadarPosition(mapData, { x: grenadeData.m_x, y: grenadeData.m_y });
  
  const grenRef = useRef();
  const grenBounding = (grenRef.current &&
    grenRef.current.getBoundingClientRect()) || { width: 0, height: 0 };
  const scaledPos = calculatePositionWithScale(radarImage, radarPosition);
  const radarImageTranslation = {
    x: (scaledPos[0] - grenBounding.width * 0.5),
    y: (scaledPos[1] - grenBounding.height * 0.5),
  };

  if(type == "landed") {
  if (grenadeData.m_type == "smoke") {
    return (
      <GrenadeEffects
        grenadeData={grenadeData}
        type="smoke"
        mapData={mapData}
        settings={settings}
        averageLatency={averageLatency}
        radarImage={radarImage}
      />
    );
  } else if (grenadeData.m_type == "molo") {

    return ( 
      <div>

        <div
        className={`absolute rounded-[100%] left-0 top-0`}
        style={{
          opacity: `0.4`,
        }}
        >

        {firePositions[0]!=null && firePositions.map((firePosition, index) => (
          <GrenadeEffects
            key={index}
            grenadeData={{m_x: firePosition[0], m_y: firePosition[1]}}
            type="molo"
            mapData={mapData}
            settings={settings}
            averageLatency={averageLatency}
            radarImage={radarImage}
          />
        ))}

        </div>

        <div
        className={`absolute rounded-[100%] left-0 top-0`}
        style={{
          transform: `translate(${radarImageTranslation.x}px, ${radarImageTranslation.y}px)`,
        }}
        >
        
        <label
        className={`absolute w-full text-center text-white text-xs font-bold`}
        >
        
          {Math.round(grenadeData.m_timeleft)}s
      
        </label>
      
        </div>

      </div>
    );
    
  }

  } else {
    return (
      <div
      ref={grenRef}
      className={`absolute left-0 top-0`}
      style={{
        transform: `translate(${radarImageTranslation.x}px, ${radarImageTranslation.y}px)`,
        transition: `transform ${averageLatency}ms linear`,
      }}
      >

        <MaskedIcon
          path={`./assets/icons/${grenadeData.m_type}.svg`}
          height={`${settings.thrownGrenadeSize}vw`}
          color={`${settings.thrownGrenadeColor}`}
        />
      
      </div>
        
    )
  } 
};

export default Grenade;