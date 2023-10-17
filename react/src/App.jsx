import { useEffect, useState } from 'react'
import './App.css'
import { PlayerCard } from "./PlayerCard";
import { Player, Radar } from "./Radar";

const App = () => {
	const [players, setPlayers] = useState([]);
	const [mapData, setMapData] = useState();
	const [localTeam, setLocalTeam] = useState();
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
		<div className="ui-container">
			<ul id="t" className="flex flex-col gap-7">
				{
					players.filter((player) => player.data.m_team == 2).map((player) =>
						<PlayerCard right={false} key={player.data.m_idx} playerData={player.data}></PlayerCard>
					)
				}
			</ul>

			{
				players.length > 0 && mapData && (
					<Radar players={players} image={`./data/${mapData.name}/radar.png`} map_data={mapData} local_team={localTeam}></Radar>
				) || (
					<div id="radar" className="radar">
						<h1>Waiting for data</h1>
					</div>
				)
			}

			<ul id="ct" className="flex flex-col gap-7">
				{
					players.filter((player) => player.data.m_team == 3).map((player) =>
						<PlayerCard right={true} key={player.data.m_idx} playerData={player.data}></PlayerCard>
					)
				}
			</ul>
		</div>
	)
}

export default App
