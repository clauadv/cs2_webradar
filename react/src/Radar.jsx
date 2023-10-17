import { useEffect, useRef, useState } from "react";

export const get_color = (color) => {
    let new_color = "grey";

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
const calculate_rotation = (view_angle, rot) => {
    rot = (rot || 0) % 360;
    rot += (view_angle - rot + 540) % 360 - 180;

    return rot;
}
export const Player = (props) => {
    const { data, map_data, radar_img, local_team } = props;

    const {rot, setRot} = useState(0);

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

    const view_angle = 270 - data.m_eye_angle;

    setRot(calculate_rotation(view_angle, rot));

    return (
        <div className={`absolute origin-center transition-transform duration-[250ms] ease-linear rounded-[100%] left-0 top-0`} ref={playerRef} style={
            {
                transform: `translate(${image_translation.x}px, ${image_translation.y}px) rotate(${data.m_is_dead && `0` || rot}deg)`,
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

export const Radar = (props) => {
    const { players, image, map_data, local_team } = props;

    let radarImage = useRef();

    return (
        <div id="radar" className={`relative overflow-hidden origin-center`} >
            <img ref={radarImage} className={`w-full h-auto`} src={image} />

            {
                players.map((player) =>
                    <Player key={player.data.m_idx} data={player.data} map_data={map_data} radar_img={radarImage.current} local_team={local_team}></Player>
                )
            }
        </div >
    )
}