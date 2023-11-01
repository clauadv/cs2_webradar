import { useRef } from "react";
import { getRadarPosition, teamEnum } from "../Utils/Utils";

export const Bomb = ({ bombData, mapData, radarImage, localTeam, averageLatency }) => {
    const radarPosition = getRadarPosition(mapData, bombData);
    
    const bombRef = useRef();
    const bombBounding = bombRef.current && bombRef.current.getBoundingClientRect() || { width: 0, height: 0 };

    const radarImageBounding = radarImage !== undefined && radarImage.getBoundingClientRect() || { width: 0, height: 0 };
    const radarImageTranslation =
    {
        x: (radarImageBounding.width * radarPosition.x - bombBounding.width * 0.5),
        y: (radarImageBounding.height * radarPosition.y - bombBounding.height * 0.5)
    };

    return (
        <div className={`absolute origin-center rounded-[100%] left-0 top-0 w-[1.5vw] h-[1.5vw] lg:w-[0.8vw] lg:h-[0.8vw]`} ref={bombRef} style={
            {
                transform: `translate(${radarImageTranslation.x}px, ${radarImageTranslation.y}px)`,
                transition: `transform ${averageLatency}ms linear`,
                backgroundColor: `${bombData.m_is_defused && `#50904c` || (localTeam == teamEnum.counterTerrorist) && `#6492b4` || `#c90b0b`}`,
                WebkitMask: `url('./assets/icons/c4_sml.png') no-repeat center / contain`,
                opacity: `1`,
                zIndex: `1`
            }
        }/>
    )
}