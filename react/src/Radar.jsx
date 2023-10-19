import { useEffect, useRef, useState } from "react";

export const get_color = (color) => {
    let new_color = "white";

    switch (color) {
        case 0:
            new_color = "#84c8ed";
            break;
        case 1:
            new_color = "#009a7d";
            break;
        case 2:
            new_color = "#eadd40";
            break;
        case 3:
            new_color = "#df7d29";
            break;
        case 4:
            new_color = "#b72b92";
            break;
        default:
            break;
    }

    return new_color;
}

let rotations = [];
export const calculate_rotation = (data) => {
    const view_angle = 270 - data.m_eye_angle;
    const idx = data.m_idx;

    rotations[idx] = (rotations[idx] || 0) % 360;
    rotations[idx] += (view_angle - rotations[idx] + 540) % 360 - 180;

    return rotations[idx];
}

export const Bomb = ({ data, map_data, radar_img, averageLatency }) => {
    const bombRef = useRef();

    const position = get_radar_position(map_data, data);

    const player_bounding = bombRef.current && bombRef.current.getBoundingClientRect() || { width: 0, height: 0 };

    const image_bounding = radar_img != undefined && radar_img.getBoundingClientRect() || { width: 0, height: 0 };
    var image_translation =
    {
        x: (image_bounding.width * position.x - player_bounding.width * 0.5),
        y: (image_bounding.height * position.y - player_bounding.height * 0.5)
    };

    return (
        <div className={`absolute origin-center transition-transform duration-[${averageLatency}ms] ease-linear rounded-[100%] left-0 top-0`} ref={bombRef} style={
            {
                transform: `translate(${image_translation.x}px, ${image_translation.y}px)`,
                backgroundColor: `${data.m_is_defused && `#50904c` || `#c90b0be6`}`,
                WebkitMask: `url('./assets/icons/c4_sml.png') no-repeat center / contain`,
                width: `0.8vw`,
                height: `0.8vw`,
                opacity: `1`,
                zIndex: `1`
            }
        }/>
    )
}

export const Player = ({ data, map_data, radar_img, local_team, averageLatency }) => {
    const playerRef = useRef();

    const position = get_radar_position(map_data, data.m_position);
    const invalid_position = position.x <= 0 && position.y <= 0;

    const player_bounding = playerRef.current && playerRef.current.getBoundingClientRect() || { width: 0, height: 0 };

    const image_bounding = radar_img != undefined && radar_img.getBoundingClientRect() || { width: 0, height: 0 };
    var image_translation =
    {
        x: (image_bounding.width * position.x - player_bounding.width * 0.5),
        y: (image_bounding.height * position.y - player_bounding.height * 0.5)
    };

    const rotation = calculate_rotation(data);

    return (
        <div className={`absolute origin-center transition-transform duration-[${averageLatency}ms] ease-linear rounded-[100%] left-0 top-0`} ref={playerRef} style={
            {
                transform: `translate(${image_translation.x}px, ${image_translation.y}px) rotate(${data.m_is_dead && `0` || rotation}deg)`,
                backgroundColor: `${data.m_team == local_team && get_color(data.m_color) || `red`}`,
                opacity: `${data.m_is_dead && `0.8` || invalid_position && `0` || `1`}`,
                WebkitMask: `${data.m_is_dead && `url('./assets/icons/icon-enemy-death_png.png') no-repeat center / contain` || `none`}`,
                width: `${data.m_is_dead && `1vw` || `0.7vw`}`,
                height: `${data.m_is_dead && `1vw` || `0.7vw`}`,
                zIndex: `${data.m_is_dead && `0` || `1`}`
            }
        }>

            <div className={`w-[0.7vw] h-[0.7vw] rotate-[315deg] rounded-[50%_50%_50%_0%]`} style={{ backgroundColor: `${data.m_team == local_team && get_color(data.m_color) || `red`}` }}></div>
        </div>
    )
}

const get_radar_position = (map_data, coords) => {
    if (!(coords.x || coords.y)) {
        return { x: 0, y: 0 };
    }

    if (!(map_data.x || map_data.y)) {
        LOG_ERROR("map positions are invalid");
        return { x: 0, y: 0 };
    }

    const position =
    {
        x: (coords.x - map_data.x) / map_data.scale / 1024,
        y: (coords.y - map_data.y) / map_data.scale * -1.0 / 1024
    };

    return position;
}

export const Radar = ({ players, image, map_data, local_team, averageLatency, bomb }) => {
    let radarImage = useRef();

    return (
        <div id="radar" className={`relative overflow-hidden origin-center`} >
            <img ref={radarImage} className={`w-full h-auto`} src={image} />

            {
                players.map((player) =>
                    <Player key={player.m_idx} data={player} map_data={map_data} radar_img={radarImage.current} local_team={local_team} averageLatency={averageLatency} />
                )
            }

            {
                bomb &&
                <Bomb data={bomb} map_data={map_data} radar_img={radarImage.current} averageLatency={averageLatency} />
            }
        </div >
    )
}