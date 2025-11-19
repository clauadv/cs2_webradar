import { useRef, useState } from "react";
import Draggable from "./Draggable"
import Player from "./player";
import Bomb from "./bomb";
import Grenade from "./grenade";
import DroppedWeapon from "./droppedweapons";

const Radar = ({
  playerArray,
  radarImage,
  mapData,
  localTeam,
  averageLatency,
  bombData,
  settings,
  grenadeData,
  droppedWeaponsData,
}) => {
  const radarImageRef = useRef();
  const [radarScale, setRadarScale] = useState(1);

  const onScroll = (e) => {
    const delta = e.deltaY * -0.001;
    const newScale = radarScale + delta;
    if (newScale>0.3&&newScale<4) setRadarScale(newScale)
  };



  return (
    <div id="radar" className={`relative overflow-hidden origin-center`}>

      <Draggable
      imgref={radarImageRef.current}
      avrPing={averageLatency}
      >
        <img onWheelCapture={onScroll} ref={radarImageRef} className={`w-full h-auto`} src={radarImage} draggable={false} style={{scale: `${radarScale}`, transition: "scale 150ms linear"}}/>
      </Draggable>

      {playerArray.map((player) => (
        <Player
          key={player.m_idx}
          playerData={player}
          mapData={mapData}
          radarImage={radarImageRef.current}
          radarScale={radarScale}
          localTeam={localTeam}
          averageLatency={averageLatency}
          settings={settings}
        />
      ))}

      {bombData && (
        <Bomb
          bombData={bombData}
          mapData={mapData}
          radarImage={radarImageRef.current}
          localTeam={localTeam}
          averageLatency={averageLatency}
          settings={settings}
        />
      )}

      {grenadeData && grenadeData["landed"]!={} && settings.showGrenades && grenadeData["landed"].map((grenade) => (
        <Grenade
          key={grenade.m_idx}
          grenadeData={grenade}
          mapData={mapData}
          settings={settings}
          averageLatency={averageLatency}
          radarImage={radarImageRef.current}
          type={"landed"}
        />
      ))}

      {grenadeData && grenadeData["thrown"]!={} && settings.showGrenades && grenadeData["thrown"].map((grenade) => (
        <Grenade
          key={grenade.m_idx}
          grenadeData={grenade}
          mapData={mapData}
          settings={settings}
          averageLatency={averageLatency}
          radarImage={radarImageRef.current}
          type={"thrown"}
        />
      ))}

      {settings.showDroppedWeapons && droppedWeaponsData && droppedWeaponsData!={} && droppedWeaponsData.map((droppedWeapon) => (
        <DroppedWeapon
          key={droppedWeapon.m_idx}
          droppedWeaponData={droppedWeapon}
          mapData={mapData}
          settings={settings}
          averageLatency={averageLatency}
          radarImage={radarImageRef.current}
        />
      ))}

    </div>
  );
};

export default Radar;