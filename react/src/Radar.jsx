import { useEffect, useRef } from "react";

const get_color = (color) => {
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

export const Player = (props) => {
    const { data, map_data, radar_img, local_team } = props;

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

    return (
        <div className="player" ref={playerRef} style={
            {
                transform: `translate(${image_translation.x}px, ${image_translation.y}px) rotate(${data.m_is_dead && `0` || view_angle}deg)`,
                backgroundColor: `${data.m_team == local_team && get_color(data.m_color) || `red`}`,
                opacity: `${data.m_is_dead && `0.8` || invalid_position && `0` || `1`}`,
                WebkitMask: `${data.m_is_dead && `url('./assets/icons/icon-enemy-death_png.png') no-repeat center / contain` || `none`}`,
                width: `${data.m_is_dead && `1vw` || `0.7vw`}`,
                height: `${data.m_is_dead && `1vw` || `0.7vw`}`,
                zIndex: `${data.m_is_dead && `0` || `1`}`
            }
        }>
            <div className="player-angle" style={
                {
                    backgroundColor: `${data.m_team == local_team && get_color(data.m_color) || `red`}`,
                }
            }></div>
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
        <div id="radar" className="radar" >
            <img ref={radarImage} className="radar-image" src={image} />

            {
                players.map((player) =>
                    <Player key={player.data.m_idx} data={player.data} map_data={map_data} radar_img={radarImage.current} local_team={local_team}></Player>
                )
            }
        </div >
    )
}