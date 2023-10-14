import { createEffect, createSignal } from "solid-js";

const get_color = (color) => {
    let new_color = "#f7d622";

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

    const samp = get_radar_position(map_data, data.m_position);

    const mata = document.getElementsByClassName("player")[0];
    const mata_bounding = mata && mata.getBoundingClientRect() || 0;

    const image_bounding = radar_img.getBoundingClientRect();
    var image_translation =
    {
        x: (image_bounding.width * samp.x - mata_bounding.width * 0.5),
        y: (image_bounding.height * samp.y - mata_bounding.height * 0.5)
    };

    return (
        <div className="player" style={
            `transform: translate(${image_translation.x}px, ${image_translation.y}px);
             background-color: ${data.m_team == local_team && get_color(data.m_color) || `red`};
             opacity: ${(data.m_is_dead || (samp.x <= 0 && samp.y <= 0)) && `0` || `1;`}`
        }>
            <div className="player__angle"></div>
        </div>
    )
}

const get_radar_position = (map_data, coords) => {
    if (!(coords.x || coords.y)) {
        return { x: 0, y: 0 };
    }

    if (!(map_data().x || map_data().y)) {
        LOG_ERROR("map positions are invalid");
        return { x: 0, y: 0 };
    }

    const position =
    {
        x: (coords.x - map_data().x) / map_data().scale / 1024,
        y: (coords.y - map_data().y) / map_data().scale * -1.0 / 1024
    };

    return position;
}

export const Radar = (props) => {
    const { players, image, map_data, local_team } = props;

    let radar_img = null;

    return (
        <div id="radar" class="radar" >
            <img ref={radar_img} class="radar-image" src={image} />

            <For each={players()}>{(player) =>
                <Player data={player.data} map_data={map_data} radar_img={radar_img} local_team={local_team()}></Player>
            }</For>
        </div >
    )
}