import ReactDOM from "react-dom/client";
import { useEffect, useState } from "react";
import "./App.css";
import PlayerCard from "./components/playercard";
import Radar from "./components/radar";
import { getLatency, Latency } from "./components/latency";
import MaskedIcon from "./components/maskedicon";
import { colorSchemePallette } from "./utilities/utilities";

const CONNECTION_TIMEOUT = 5000;

/* change this to '1' if you want to use offline (your own pc only) */
const USE_LOCALHOST = 0;

/* you can get your public ip from https://ipinfo.io/ip */
const PUBLIC_IP = "PUBLIC_IP".trim();
const PORT = 22006;












let tempPlayer_ = null;

const EFFECTIVE_IP = USE_LOCALHOST ? "localhost" : PUBLIC_IP.match(/[a-zA-Z]/) ? window.location.hostname : PUBLIC_IP;

const DEFAULT_SETTINGS = {
  dotSize: 1,
  bombSize: 0.5,
  showAllNames: false,
  showEnemyNames: true,
  showViewCones: false,
  showOnlyEnemies: false,
  showDroppedWeapons: true,
  droppedWeaponSize: 0.5,
  droppedWeaponGlow: true,
  droppedWeaponIgnoreNade: false,
  showGrenades: true,
  thrownGrenadeSize: 0.5,
  thrownGrenadeColor: "#FF0000",
  whichPlayerAreYou: "0",
  colorScheme: "default",
  settings_version: "1.0"
};

const loadSettings = () => {
  const savedSettings = localStorage.getItem("radarSettings");
  let parsedSettings = JSON.parse(savedSettings);
  return (savedSettings && parsedSettings.settings_version && parsedSettings.settings_version == DEFAULT_SETTINGS.settings_version) ? parsedSettings : DEFAULT_SETTINGS;
};

const PlayerSelectionModal = ({ players, onSelect, localTeam }) => {
    players = players
    return (
        <div className="fixed inset-0 bg-black bg-opacity-50 backdrop-blur-md flex justify-center items-center z-[100]">
            <div className="bg-gray-800 p-8 rounded-lg shadow-2xl w-96 max-w-[90vw] border border-gray-700">
                <h2 className="text-xl font-bold text-white mb-4">Select Yourself</h2>
                <p className="text-sm text-gray-400 mb-6">This is used to show you different map levels. <br></br> Please choose <b>YOURSELF</b>!</p>
                <ul className="max-h-60 overflow-y-auto pr-2 space-y-2">
                    {players
                    .filter(player => player.m_steam_id !== "0")
                    .filter(player => player.m_team === localTeam)
                    .map((player) => (
                        <li 
                            key={player.m_steam_id} 
                            onClick={() => onSelect(player.m_steam_id)}
                            className="p-3 bg-gray-700 hover:bg-radar-blue text-white rounded-md cursor-pointer transition-all duration-200 flex justify-between items-center"
                        >
                            <span className="font-medium truncate">{player.m_name}</span>
                            <span className={`text-xs px-2 py-0.5 rounded-full`}>
                                {player.m_steam_id}
                            </span>
                        </li>
                    ))}
                </ul>
            </div>
        </div>
    );
};

const App = () => {
  const [averageLatency, setAverageLatency] = useState(0);
  const [playerArray, setPlayerArray] = useState([]);
  const [grenadeData, setGrenadeData] = useState([]);
  const [droppedWeaponsData, setDroppedWeaponsData] = useState([]);
  const [mapData, setMapData] = useState();
  const [localTeam, setLocalTeam] = useState();
  const [bombData, setBombData] = useState();
  const [settings, setSettings] = useState(loadSettings());
  const [showPlayerPrompt, setShowPlayerPrompt] = useState(false);

  useEffect(() => {
    localStorage.setItem("radarSettings", JSON.stringify(settings));
  }, [settings]);

  useEffect(() => {
      if (settings.whichPlayerAreYou === "0" && playerArray.length > 0 || settings.whichPlayerAreYou === undefined && playerArray.length > 0 ) {
          setShowPlayerPrompt(true);
      } else if (settings.whichPlayerAreYou !== "0" && settings.whichPlayerAreYou !== undefined) {
          setShowPlayerPrompt(false);
      }
    }, [playerArray, settings.whichPlayerAreYou]);
    const handlePlayerSelect = (playerIdx) => {
    setSettings((prevSettings) => ({
        ...prevSettings,
        whichPlayerAreYou: playerIdx,
    }));
    setShowPlayerPrompt(false);
  };

  useEffect(() => {
    const fetchData = async () => {
      let webSocket = null;
      let webSocketURL = null;
      let connectionTimeout = null;

      if (PUBLIC_IP.startsWith("192.168")) {
        document.getElementsByClassName(
          "radar_message"
        )[0].textContent = `A public IP address is required! Currently detected IP (${PUBLIC_IP}) is a private/local IP`;
        return;
      }

      if (!webSocket) {
        try {
          if (USE_LOCALHOST) {
            webSocketURL = `ws://localhost:${PORT}/cs2_webradar`;
          } else {
            webSocketURL = `ws://${EFFECTIVE_IP}:${PORT}/cs2_webradar`;
          }

          if (!webSocketURL) return;
          webSocket = new WebSocket(webSocketURL);
        } catch (error) {
          document.getElementsByClassName(
            "radar_message"
          )[0].textContent = `${error}`;
        }
      }

      connectionTimeout = setTimeout(() => {
        webSocket.close();
      }, CONNECTION_TIMEOUT);

      webSocket.onopen = async () => {
        clearTimeout(connectionTimeout);
        console.info("connected to the web socket");
      };

      webSocket.onclose = async () => {
        clearTimeout(connectionTimeout);
        console.error("disconnected from the web socket");
      };

      webSocket.onerror = async (error) => {
        clearTimeout(connectionTimeout);
        document.getElementsByClassName(
          "radar_message"
        )[0].textContent = `WebSocket connection to '${webSocketURL}' failed. Please check the IP address and try again`;
        console.error(error);
      };

      webSocket.onmessage = async (event) => {
        setAverageLatency(getLatency());

        const parsedData = JSON.parse(await event.data.text());
        setLocalTeam(parsedData.m_local_team);
        setBombData(parsedData.m_bomb);
        setGrenadeData(parsedData.m_grenades);
        setDroppedWeaponsData(parsedData.m_dropped_weapons);

        const map = parsedData.m_map;
        if (map !== "invalid") {
          setPlayerArray(parsedData.m_players);
          setMapData({
            ...(await (await fetch(`data/${map}/data.json`)).json()),
            name: map,
          });
          document.body.style.backgroundImage = `url(./data/${map}/background.png)`;
        } else {
          setMapData({ name: "invalid" });
          setPlayerArray([]);
          document.body.style.backgroundImage = `url(./data/de_mirage/background.png)`;
        }
      };
    };

    fetchData();
  }, []);
  
  if (playerArray && playerArray.length > 0) {
    tempPlayer_ = playerArray.find((player) => player.m_steam_id === settings.whichPlayerAreYou);
  }

  return (
    <div className="w-screen h-screen flex flex-col"
      style={{
        background: `radial-gradient(50% 50% at 50% 50%, rgba(20, 40, 55, 0.95) 0%, rgba(7, 20, 30, 0.95) 100%)`,
        backdropFilter: `blur(7.5px)`,
      }}
    >
        {showPlayerPrompt && playerArray.length > 0 && (
            <PlayerSelectionModal 
                players={playerArray} 
                onSelect={handlePlayerSelect} 
                localTeam={localTeam}
            />
        )}

      <div className={`w-full h-full flex flex-col justify-center overflow-hidden relative`} style={{transform: "rotate(0deg)"}}>
        {bombData && bombData.m_blow_time > 0 && !bombData.m_is_defused && (
          <div className={`absolute left-1/2 top-2 flex-col items-center gap-1 z-50`}>
            <div className={`flex justify-center items-center gap-1`}>
              <MaskedIcon
                path={`./assets/icons/c4_sml.png`}
                height={32}
                color={
                  (bombData.m_is_defusing &&
                    bombData.m_blow_time - bombData.m_defuse_time > 0 &&
                    `#00FF00`) ||
                  (bombData.m_blow_time - bombData.m_defuse_time < 0 &&
                    `#FF0000`) ||
                  `${colorSchemePallette[settings.colorScheme][1]}`
                }
              />
              <span>{`${bombData.m_blow_time.toFixed(1)}s ${(bombData.m_is_defusing &&
                `(${bombData.m_defuse_time.toFixed(1)}s)`) ||
                ""
                }`}</span>
            </div>
          </div>
        )}

        <div className={`flex items-center justify-evenly`}>
          <Latency
            value={averageLatency}
            settings={settings}
            setSettings={setSettings}
          />

          <ul id="terrorist" className="lg:flex hidden flex-col gap-7 m-0 p-0">
            {(playerArray && playerArray.length > 0 && playerArray
              .filter((player) => player.m_team == 2)
              .map((player) => (
                <PlayerCard
                  right={false}
                  key={player.m_idx}
                  playerData={player}
                  settings={settings}
                />
              )))}
          </ul>

          {(playerArray && playerArray.length > 0 && mapData && mapData.name !== "invalid" && settings.whichPlayerAreYou && (
            <div style={{transform: "scale(1)"}}>
              <Radar
                playerArray={playerArray}
                radarImage={(tempPlayer_ && (mapData.leveling && tempPlayer_.m_position.z < mapData.level_change) ? `./data/${mapData.name}/radar_lower.png` : `./data/${mapData.name}/radar.png`)}
                mapData={mapData}
                localTeam={localTeam}
                averageLatency={averageLatency}
                bombData={bombData}
                settings={settings}
                grenadeData={grenadeData}
                droppedWeaponsData={droppedWeaponsData}
              />
            </div>
          )) || (
              <div id="radar" className={`relative overflow-hidden origin-center`}>
                <h1 className="radar_message">
                  Connected! Please wait for the host to join the match.
                </h1>
              </div>
            )}

          <ul
            id="counterTerrorist"
            className="lg:flex hidden flex-col gap-7 m-0 p-0"
          >
            {(playerArray && playerArray.length > 0 && playerArray
              .filter((player) => player.m_team == 3)
              .map((player) => (
                <PlayerCard
                  right={true}
                  key={player.m_idx}
                  playerData={player}
                  settings={settings}
                />
              )))}
          </ul>
        </div>
      </div>
    </div>
  );
};

export default App;