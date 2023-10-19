import ReactDOM from 'react-dom/client'
import { useEffect, useState } from 'react'
import './App.css'
import { PlayerCard } from "./PlayerCard";
import { Player, Radar } from "./Radar";
import { GetLatency, LatencyContainer } from './LatencyContainer';
import { MaskedIcon } from './MaskedIcon';


const App = () => {
	const [players, setPlayers] = useState([]);
	const [mapData, setMapData] = useState();
	const [localTeam, setLocalTeam] = useState();
	const [averageLatency, setAverageLatency] = useState(0);
	const [bomb, setBomb] = useState();

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
				setPlayers(parsed_data.m_players);
				setLocalTeam(parsed_data.m_local_team);
				setAverageLatency(GetLatency());
				setBomb(parsed_data.m_bomb);

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
		<div className={`w-screen h-screen flex flex-col justify-center backdrop-blur-[7.5px]`} style={{ background: `radial-gradient(50% 50% at 50% 50%, rgba(20, 40, 55, 0.95) 0%, rgba(7, 20, 30, 0.95) 100%)`, backdropFilter: `blur(7.5px)` }}>

			{
				(bomb && bomb.m_blow_time > 0 && !bomb.m_is_defused) && (
					<div className={`flex flex-col items-center gap-1`}>
						<div className={`flex justify-center items-center gap-1`}>
							<MaskedIcon path={`./assets/icons/c4_sml.png`} height={32} color={bomb.m_is_defusing && `bg-radar-green` || `bg-radar-secondary`} />
							<span>{`${bomb.m_blow_time.toFixed(1)}s ${bomb.m_is_defusing && `(${bomb.m_defuse_time.toFixed(1)}s)` || ''}`}</span>
						</div>
					</div>
				)
			}

			<div className={`flex items-center justify-evenly`}>
				<LatencyContainer value={averageLatency} />

				<ul id="t" className="flex flex-col gap-7 m-0 p-0">
					{
						players.filter((player) => player.m_team == 2).map((player) =>
							<PlayerCard right={false} key={player.m_idx} playerData={player} />
						)
					}
				</ul>

				{
					players.length > 0 && mapData && (
						<Radar players={players} image={`./data/${mapData.name}/radar.png`} map_data={mapData} local_team={localTeam} averageLatency={averageLatency} bomb={bomb} />
					) || (
						<div id="radar" className={`relative overflow-hidden origin-center`}>
							<h1>Waiting for data</h1>
						</div>
					)
				}

				<ul id="ct" className="flex flex-col gap-7 m-0 p-0">
					{
						players.filter((player) => player.m_team == 3).map((player) =>
							<PlayerCard right={true} key={player.m_idx} playerData={player} />
						)
					}
				</ul>
			</div>
		</div>
	)
}

export default App;