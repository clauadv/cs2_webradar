import ReactDOM from "react-dom/client";
import { useEffect, useState } from "react";
import "./App.css";
import PlayerCard from "./components/PlayerCard";
import Radar from "./components/Radar";
import { getLatency, Latency } from "./components/latency";
import MaskedIcon from "./components/maskedicon";

/* if you want to share the radar, set this to '0', otherwise let it be '1' */
const USE_LOCALHOST = 1;

const PUBLIC_IP = "your ip goes here";
const PORT = 22006;

const App = () => {
  const [averageLatency, setAverageLatency] = useState(0);
  const [playerArray, setPlayerArray] = useState([]);
  const [mapData, setMapData] = useState();
  const [localTeam, setLocalTeam] = useState();
  const [bombData, setBombData] = useState();

  useEffect(() => {
    const fetchData = async () => {
      let webSocket = null;
      let webSocketURL = null;

      if (!webSocket) {
        if (USE_LOCALHOST) {
          webSocketURL = `ws://localhost:${PORT}/cs2_webradar`;
        } else {
          webSocketURL = `ws://${PUBLIC_IP}:${PORT}/cs2_webradar`;
        }

        if (!webSocketURL) return;
        webSocket = new WebSocket(webSocketURL);
      }

      webSocket.onopen = async () => {
        console.info("connected to the web socket");
      };

      webSocket.onclose = async () => {
        console.error("disconnected from the web socket");
      };

      webSocket.onerror = async (error) => {
        document.getElementsByClassName(
          "radar_message"
        )[0].textContent = `WebSocket connection to '${webSocketURL}' failed`;
        console.error(error);
      };

      webSocket.onmessage = async (event) => {
        setAverageLatency(getLatency());

        const parsedData = JSON.parse(await event.data.text());
        setPlayerArray(parsedData.m_players);
        setLocalTeam(parsedData.m_local_team);
        setBombData(parsedData.m_bomb);

        const map = parsedData.m_map;
        if (map !== "invalid") {
          setMapData({
            ...(await (await fetch(`data/${map}/data.json`)).json()),
            name: map,
          });
          document.body.style.backgroundImage = `url(./data/${map}/background.png)`;
        }
      };
    };

    fetchData();
  }, []);

  return (
    <div
      className={`w-screen h-screen flex flex-col justify-center backdrop-blur-[7.5px]`}
      style={{
        background: `radial-gradient(50% 50% at 50% 50%, rgba(20, 40, 55, 0.95) 0%, rgba(7, 20, 30, 0.95) 100%)`,
        backdropFilter: `blur(7.5px)`,
      }}
    >
      {bombData && bombData.m_blow_time > 0 && !bombData.m_is_defused && (
        <div className={`flex flex-col items-center gap-1`}>
          <div className={`flex justify-center items-center gap-1`}>
            <MaskedIcon
              path={`./assets/icons/c4_sml.png`}
              height={32}
              color={
                (bombData.m_is_defusing &&
                  bombData.m_blow_time - bombData.m_defuse_time > 0 &&
                  `bg-radar-green`) ||
                (bombData.m_blow_time - bombData.m_defuse_time < 0 &&
                  `bg-radar-red`) ||
                `bg-radar-secondary`
              }
            />
            <span>{`${bombData.m_blow_time.toFixed(1)}s ${
              (bombData.m_is_defusing &&
                `(${bombData.m_defuse_time.toFixed(1)}s)`) ||
              ""
            }`}</span>
          </div>
        </div>
      )}

      <div className={`flex items-center justify-evenly`}>
        <Latency value={averageLatency} />

        <ul id="terrorist" className="lg:flex hidden flex-col gap-7 m-0 p-0">
          {playerArray
            .filter((player) => player.m_team == 2)
            .map((player) => (
              <PlayerCard
                right={false}
                key={player.m_idx}
                playerData={player}
              />
            ))}
        </ul>

        {(playerArray.length > 0 && mapData && (
          <Radar
            playerArray={playerArray}
            radarImage={`./data/${mapData.name}/radar.png`}
            mapData={mapData}
            localTeam={localTeam}
            averageLatency={averageLatency}
            bombData={bombData}
          />
        )) || (
          <div id="radar" className={`relative overflow-hidden origin-center`}>
            <h1 className="radar_message">
              Connected! Waiting for data from usermode
            </h1>
          </div>
        )}

        <ul
          id="counterTerrorist"
          className="lg:flex hidden flex-col gap-7 m-0 p-0"
        >
          {playerArray
            .filter((player) => player.m_team == 3)
            .map((player) => (
              <PlayerCard right={true} key={player.m_idx} playerData={player} />
            ))}
        </ul>
      </div>
    </div>
  );
};

export default App;
