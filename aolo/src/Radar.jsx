import { createEffect, createSignal } from "solid-js";

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

    const position = get_radar_position(map_data, data.m_position);
    const invalid_position = position.x <= 0 && position.y <= 0;

    const player_class = document.getElementsByClassName("player")[0];
    const player_bounding = player_class && player_class.getBoundingClientRect() || 0;

    const image_bounding = radar_img.getBoundingClientRect();
    var image_translation =
    {
        x: (image_bounding.width * position.x - player_bounding.width * 0.5),
        y: (image_bounding.height * position.y - player_bounding.height * 0.5)
    };

    return (
        <div className="player" style={
            `transform: translate(${image_translation.x}px, ${image_translation.y}px);
             background-color: ${data.m_team == local_team && get_color(data.m_color) || `red`};
             opacity: ${data.m_is_dead && `0.5` || invalid_position && `0` || `1`};
             ${data.m_is_dead && `
             -webkit-mask: url('./assets/icons/icon-enemy-death_png.png') no-repeat center / contain;
             width: 1vw;
             height: 1vw;`}`
        }>
            {/* <div className="player-angle"></div> */}
        </div>
    )
}

export const Bomb = (props) => {
    const { map_data, radar_img } = props;

    const image_bounding = radar_img.getBoundingClientRect();
    console.log(image_bounding)
    var image_translation =
    {
        x: (image_bounding.width * position.x - player_bounding.width * 0.5),
        y: (image_bounding.height * position.y - player_bounding.height * 0.5)
    };

    return (
        <></>
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

            <Bomb map_data={map_data} radar_img={radar_img}></Bomb>
        </div >
    )
}