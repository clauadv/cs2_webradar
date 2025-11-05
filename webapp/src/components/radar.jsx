import { useRef } from "react";
import Player from "./player";
import Bomb from "./bomb";
import Grenade from "./grenade";

const Radar = ({ playerArray, radarImage, mapData, localTeam, averageLatency, bombData, nades, settings }) => {
  const radarImageRef = useRef();

  return (
    <div className="relative">
      <div id="radar" className="relative overflow-hidden origin-center">
        <img ref={radarImageRef} className="w-full h-auto" src={radarImage} />

          {playerArray.map((player) => (
            <Player
              key={player.m_idx}
              playerData={player}
              mapData={mapData}
              radarImage={radarImageRef.current}
              localTeam={localTeam}
              averageLatency={averageLatency}
              settings={settings}
            />
          ))}

          {nades && nades.map((grenade, idx) => (
            <Grenade
              key={`grenade-${idx}`}
              grenadeData={grenade}
              mapData={mapData}
              radarImage={radarImageRef.current}
            />
          ))}

        {bombData && <Bomb bombData={bombData} mapData={mapData} radarImage={radarImageRef.current} localTeam={localTeam} averageLatency={averageLatency} settings={settings} />}
      </div>
    </div>
  );
};

export default Radar;