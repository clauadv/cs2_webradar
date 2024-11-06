import ReactDOM from "react-dom/client";
import { useEffect, useState } from "react";
import "./App.css";
import PlayerCard from "./components/PlayerCard";
import Radar from "./components/Radar";
import { getLatency, Latency } from "./components/latency";
import MaskedIcon from "./components/maskedicon";

const CONNECTION_TIMEOUT = 5000;

/* if you want to share the radar, set this to '0', otherwise let it be '1' */
const USE_LOCALHOST = 1;

/* you can get your public ip from https://ipinfo.io/ip */
const PUBLIC_IP = "your ip goes here".trim();
const PORT = 22006;

function isPrivateIP(ip) {
    return ip.startsWith("192.168.") ||
        ip.startsWith("10.") ||
        /^172\.(1[6-9]|2[0-9]|3[0-1])\./.test(ip);
}

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
            let connectionTimeout = null;

            if (isPrivateIP(PUBLIC_IP)) {
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
                        webSocketURL = `ws://${PUBLIC_IP}:${PORT}/cs2_webradar`;
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
                        <span>{`${bombData.m_blow_time.toFixed(1)}s ${(bombData.m_is_defusing &&
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