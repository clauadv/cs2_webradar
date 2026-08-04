import Player from "./player";
import Bomb from "./bomb";
import { useT } from "../i18n/i18n";

// Radar canvas surface component
const Radar = ({ playerArray, radarImage, mapData, localTeam, bombData, settings }) => {
  const { t } = useT();

  return (
    <div
      id="radar"
      className="panel-sunken relative aspect-square w-full overflow-hidden rounded-2xl"
    >
      <img
        className="pointer-events-none block h-full w-full select-none object-contain"
        src={radarImage}
        alt={t("app.radarAlt", { map: mapData.name })}
        draggable={false}
      />

      {playerArray.map((player) => (
        <Player
          key={player.m_idx}
          playerData={player}
          mapData={mapData}
          localTeam={localTeam}
          settings={settings}
        />
      ))}

      {bombData && (
        <Bomb
          bombData={bombData}
          mapData={mapData}
          localTeam={localTeam}
          settings={settings}
        />
      )}
    </div>
  );
};

export default Radar;
