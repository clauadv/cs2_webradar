import ReactDOM from 'react-dom/client'
import { useEffect, useState } from 'react'
import './App.css'
import { PlayerCard } from "./PlayerCard";
import { Player, Radar } from "./Radar";
import { GetLatency, LatencyContainer } from './LatencyContainer';


const App = () => {
	const [players, setPlayers] = useState([]);
	const [mapData, setMapData] = useState();
	const [localTeam, setLocalTeam] = useState();
	const [averageLatency, setAverageLatency] = useState(0);

	let web_socket = null;

	useEffect(() => {
		const fetchData = async () => {
			if (!web_socket) {
				web_socket = new WebSocket("ws://188.24.175.93:22006/cs2_webradar");
			}

			web_socket.onopen = async () => {
				console.info("connected to the web socket");
			}

			web_socket.onclose = async () => {
				console.error("disconnected from the web socket");
			}

			web_socket.onerror = async (error) => {
				console.error(error);
			}

			web_socket.onmessage = async (event) => {
				const parsed_data = JSON.parse(await event.data.text());
				setPlayers(parsed_data.players);
				setLocalTeam(parsed_data.m_local_team);
				setAverageLatency(GetLatency());

				const map = parsed_data.m_map;
				if (map !== "invalid") {
					setMapData({ ...(await (await fetch(`data/${map}/data.json`)).json()), name: map });
					document.body.style.backgroundImage = `url(./data/${map}/background.png)`;
				}
			};
		};

		fetchData();
	}, []);

	return (
		<div className={`w-screen h-screen flex items-center justify-evenly backdrop-blur-[7.5px]`} style={{ background: `radial-gradient(50% 50% at 50% 50%, rgba(20, 40, 55, 0.95) 0%, rgba(7, 20, 30, 0.95) 100%)`, backdropFilter: `blur(7.5px)` }}>
			<LatencyContainer value={averageLatency} />

			<ul id="t" className="flex flex-col gap-7 m-0 p-0">
				{
					players.filter((player) => player.data.m_team == 2).map((player) =>
						<PlayerCard right={false} key={player.data.m_idx} playerData={player.data} />
					)
				}
			</ul>

			{
				players.length > 0 && mapData && (
					<Radar players={players} image={`./data/${mapData.name}/radar.png`} map_data={mapData} local_team={localTeam} averageLatency={averageLatency} />
				) || (
					<div id="radar" className={`relative overflow-hidden origin-center`}>
						<h1>Waiting for data</h1>
					</div>
				)
			}

			<ul id="ct" className="flex flex-col gap-7 m-0 p-0">
				{
					players.filter((player) => player.data.m_team == 3).map((player) =>
						<PlayerCard right={true} key={player.data.m_idx} playerData={player.data} />
					)
				}
			</ul>
		</div>
	)
}

export default App;