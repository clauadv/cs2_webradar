import { createEffect, createSignal, onMount } from 'solid-js'
import './App.css'
import { PlayerCard } from "./PlayerCard";
import { Radar } from "./Radar";

const App = () => {
	const [players, setPlayers] = createSignal([]);
	const [mapData, setMapData] = createSignal();
	const [localTeam, setLocalTeam] = createSignal();

	onMount(async () => {
		const web_socket = new WebSocket("ws://188.24.175.93:22006/cs2_webradar");

		web_socket.onopen = async () => {
			console.info("connected to the web socket");
		}

		web_socket.onclose = async () => {
			console.error("disconnected from the web socket");
		}

		web_socket.onerror = async (error) => {
			LOG_ERROR(`${error}`);
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
	});

	return (
		<div class="ui-container">
			<ul id="t" class="players-list players-left">
				<For each={players().filter((player) => player.data.m_team == 2)}>{(player) =>
					<PlayerCard data={player.data}></PlayerCard>
				}</For>
			</ul>

			{
				players().length > 0 && mapData() && (
					<Radar players={players} image={`./data/${mapData().name}/radar.png`} map_data={mapData} local_team={localTeam}></Radar>
				) || (
					<div id="radar" class="radar">
						<h1>Waiting for map</h1>
					</div>
				)
			}

			<ul id="ct" class="players-list players-right">
				<For each={players().filter((player) => player.data.m_team == 3)}>{(player) =>
					<PlayerCard data={player.data}></PlayerCard>
				}</For>
			</ul>
		</div>
	)
}

export default App
