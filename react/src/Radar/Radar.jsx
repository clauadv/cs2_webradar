import { useRef } from "react";
import { Player } from "./Player";
import { Bomb } from "./Bomb";

export const Radar = ({ playerArray, radarImage, mapData, localTeam, averageLatency, bombData }) => {
    const radarImageRef = useRef();

    return (
        <div id="radar" className={`relative overflow-hidden origin-center`} >
            <img ref={radarImageRef} className={`w-full h-auto`} src={radarImage} />

            {
                playerArray.map((player) =>
                    <Player key={player.m_idx} playerData={player} mapData={mapData} radarImage={radarImageRef.current} localTeam={localTeam} averageLatency={averageLatency} />
                )
            }

            {
                bombData &&
                <Bomb bombData={bombData} mapData={mapData} radarImage={radarImageRef.current} localTeam={localTeam} averageLatency={averageLatency} />
            }
        </div >
    )
}