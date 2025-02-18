import { useRef } from "react";
import { getRadarPosition, teamEnum } from "../utilities/utilities";

const Bomb = ({ bombData, mapData, radarImage, localTeam, averageLatency, settings }) => {
  const radarPosition = getRadarPosition(mapData, bombData);

  const bombRef = useRef();
  const bombBounding = (bombRef.current &&
    bombRef.current.getBoundingClientRect()) || { width: 0, height: 0 };

  const radarImageBounding = (radarImage !== undefined &&
    radarImage.getBoundingClientRect()) || { width: 0, height: 0 };
  const radarImageTranslation = {
    x: radarImageBounding.width * radarPosition.x - bombBounding.width * 0.5,
    y: radarImageBounding.height * radarPosition.y - bombBounding.height * 0.5,
  };

  // Calculate bomb size based on settings
  const baseSize = 1.5; // Base size in vw
  const scaledSize = baseSize * settings.bombSize;

  return (
    <div
      className={`absolute origin-center rounded-[100%] left-0 top-0`}
      ref={bombRef}
      style={{
        width: `${scaledSize}vw`,
        height: `${scaledSize}vw`,
        transform: `translate(${radarImageTranslation.x}px, ${radarImageTranslation.y}px)`,
        transition: `transform ${averageLatency}ms linear`,
        backgroundColor: `${
          (bombData.m_is_defused && `#50904c`) ||
          (localTeam == teamEnum.counterTerrorist && `#6492b4`) ||
          `#c90b0b`
        }`,
        WebkitMask: `url('./assets/icons/c4_sml.png') no-repeat center / contain`,
        opacity: `1`,
        zIndex: `1`,
      }}
    />
  );
};

export default Bomb;