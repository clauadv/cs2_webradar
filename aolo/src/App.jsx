import { createEffect, createSignal, onMount } from 'solid-js'
import solidLogo from './assets/solid.svg'
import viteLogo from '/vite.svg'
import './App.css'
import { PlayerCard } from "./PlayerCard";
import { Radar } from "./Radar";

function App() {
	const [players, setPlayers] = createSignal([]);
	const [mapData, setMapData] = createSignal();

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
		};

		var cristos = await (await fetch(`data/de_mirage/data.json`)).json();
		setMapData(cristos);
	});

	return (
		<div class="ui-container">
			<ul id="t" class="players-list players-left">
				<For each={players().filter((player) => player.data.m_team == 2)}>{(player) =>
					<PlayerCard data={player.data}></PlayerCard>
				}</For>
			</ul>

			<Radar players={ players } image={ "./data/de_mirage/radar.png" } map_data={ mapData }></Radar>

			<ul id="ct" class="players-list players-right">
				<For each={players().filter((player) => player.data.m_team == 3)}>{(player) =>
					<PlayerCard data={player.data}></PlayerCard>
				}</For>
			</ul>
		</div>
	)
}

export default App
