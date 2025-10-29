import { useRef } from "react";
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

  return (
    <div id="radar" className={`relative overflow-hidden origin-center`}>
      <img ref={radarImageRef} className={`w-full h-auto`} src={radarImage} />

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