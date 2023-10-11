import { create_player_card, update_player_card } from "./player_card.js"

const USE_NEW_DESIGN = true;

export const globals =
{
    ui_container_element: null,
    map:
    {
        m_div: null,
        m_image: null,
        m_data: null,
        m_current: "invalid",
        m_zoom_level: 3
    },

    latency:
    {
        m_html: null,
        m_last_time: new Date().getTime(),
        m_average_time: 0,
        m_average_calc: 0,
        m_data_count: 0,
        m_is_connected: false
    },

    local_player:
    {
        m_team: -1
    },

    m_terro_div: null,
    m_counter_div: null,

    m_players: []
};

const LOG_INFO = async (...args) => console.info(`[info] ${args.join(" ")}`);
const LOG_ERROR = async (...args) => console.error(`[error] ${args.join(" ")}`);

const draw_latency = async () => {
    const current_time = new Date().getTime();
    const last_connection_time = globals.latency.m_last_time;
    const diff_in_ms = current_time - last_connection_time;

    if (globals.latency.m_average_time === 0) {
        globals.latency.m_average_time = diff_in_ms;
    }

    if (!globals.latency.m_is_connected)
        globals.latency.m_html.innerText = "not connected";
    else
        globals.latency.m_html.innerText = `${globals.latency.m_average_time}ms`;

    if (globals.latency.m_average_calc >= 10) {
        globals.latency.m_average_time = globals.latency.m_average_calc / globals.latency.m_data_count;

        globals.latency.m_data_count = 0;
        globals.latency.m_average_calc = 0;
    }
    else {
        globals.latency.m_data_count++;
        globals.latency.m_average_calc += diff_in_ms;
    }

    globals.latency.m_last_time = new Date().getTime();
}

const setup_connection = async () => {
    const web_socket = new WebSocket("ws://188.24.175.93:22006/cs2_webradar");

    web_socket.onopen = async () => {
        LOG_INFO("connected to the web socket");
        globals.latency.m_is_connected = true;
        await on_map_change();
    }

    web_socket.onclose = async () => {
        LOG_ERROR("disconnected from the web socket");
        globals.latency.m_is_connected = false;
    }

    web_socket.onerror = async (error) => {
        LOG_ERROR(`${error}`);
    }

    web_socket.onmessage = async (event) => {
        const parsed_data = JSON.parse(await event.data.text());
        await update_radar(parsed_data);

        draw_latency();
    };
}

const get_color = (color) => {
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

const calculate_rotation = (view_angle, player) => {
    player.m_rotation = (player.m_rotation || 0) % 360;
    player.m_rotation += (view_angle - player.m_rotation + 540) % 360 - 180;

    return player.m_rotation;
}

const add_player = (idx, data) => {
    const div = document.createElement("div");
    div.classList.add("player");

    const angle = document.createElement("div");
    angle.classList.add("player__angle");

    div.appendChild(angle);

    // console.log(`added player with idx ${idx}`)

    globals.m_players[idx] =
    {
        m_idx: idx,
        m_html: div,
        m_player_card: null,
        m_angle_html: angle,
        m_last_time: new Date().getTime(),
    };

    let playerCard = create_player_card(globals.m_players[idx], data);

    globals.m_players[idx].m_player_card = playerCard;

    globals.map.m_div.appendChild(globals.m_players[idx].m_html);
}

const update_player = (idx, data) => {
    if (!globals.m_players[idx]) {
        add_player(idx, data);
        return;
    }

    // console.log(`updated player with idx ${idx}`)

    if (data.m_team == 2 && (globals.m_players[idx].m_player_card.m_parent_element.getAttribute("data-team") != data.m_team)) {
        globals.m_terro_div.appendChild(globals.m_players[idx].m_player_card.m_parent_element);
    }
    else if (data.m_team == 3 && (globals.m_players[idx].m_player_card.m_parent_element.getAttribute("data-team") != data.m_team)) {
        globals.m_counter_div.appendChild(globals.m_players[idx].m_player_card.m_parent_element);
    }

    globals.m_players[idx].m_player_card.m_parent_element.setAttribute("data-team", data.m_team);

    update_player_card(globals.m_players[idx], data);

    globals.m_players[idx].m_last_time = new Date().getTime();

    const div = globals.m_players[idx].m_html;
    const div_bounding = div.getBoundingClientRect();
    const position = get_radar_position(data.m_position);

    const image_bounding = globals.map.m_image.getBoundingClientRect();
    var image_translation =
    {
        x: (image_bounding.width * position.x - div_bounding.width * 0.5), /// globals.map.m_zoom_level * 2,
        y: (image_bounding.height * position.y - div_bounding.height * 0.5) /// globals.map.m_zoom_level* 2,
    };

    const view_angle = 270 - data.m_eye_angle;
    const rotation = calculate_rotation(view_angle, globals.m_players[idx]);

    const angle_div = globals.m_players[idx].m_angle_html;
    angle_div.style.backgroundColor = (data.m_team == globals.local_player.m_team ? get_color(data.m_color) : "red");

    div.style.opacity = (data.m_is_dead ? "0" : "1");

    div.style.transition = `transform ${globals.latency.m_average_time}ms linear`;
    div.style.transform = `translate(${image_translation.x}px, ${image_translation.y}px) rotate(${rotation}deg)`;
}

const on_map_change = async () => {
    if (globals.map.m_current == "invalid")
    {
        document.getElementsByClassName("awaiting")[0].style.display = "block";
        globals.map.m_image.style.display = "none";

        document.body.style.backgroundImage = "url()";

        while (globals.m_counter_div.firstChild)
        {
            globals.m_counter_div.firstChild.remove();
        }

        while (globals.m_terro_div.firstChild)
        {
            globals.m_terro_div.firstChild.remove();
        }

        return;
    }

    document.getElementsByClassName("awaiting")[0].style.display = "none";
    globals.map.m_image.src = `data/${globals.map.m_current}/radar.png`;
    globals.map.m_image.classList.add("radar__image");
    globals.map.m_data = await (await fetch(`data/${globals.map.m_current}/data.json`)).json();
    globals.map.m_image.style.display = "flex";
    document.body.style.backgroundImage = `url(./data/${globals.map.m_current}/background.png)`;
    LOG_INFO(`changed map to ${globals.map.m_current}`);
}

const get_radar_position = (coords) => {
    if (!(coords.x || coords.y)) {
        LOG_ERROR("player positions are invalid");
        return { x: 0, y: 0 };
    }

    if (!(globals.map.m_data.x || globals.map.m_data.y)) {
        LOG_ERROR("map positions are invalid");
        return { x: 0, y: 0 };
    }

    const position =
    {
        x: (coords.x - globals.map.m_data.x) / globals.map.m_data.scale / 1024,
        y: (coords.y - globals.map.m_data.y) / globals.map.m_data.scale * -1.0 / 1024
    };

    return position;
}

const update_radar = async (data) => {
    if (!data.m_map) {
        LOG_ERROR("data.m_map is null");
        return;
    }

    if (data.m_map !== globals.map.m_current) {
        globals.map.m_current = data.m_map;
        await on_map_change();
    }

    if (!data.players) {
        LOG_ERROR("data.players is null");
        return;
    }

    globals.local_player.m_team = data.local_player[0].m_team;

    data.players.forEach(player => update_player(player.m_idx, player.data));

    const current_time = new Date().getTime();

    for (var i = 0; i < globals.m_players.length; i++)
    {
        const player = globals.m_players[i];
        if (player == undefined)
            continue;

        if ((current_time - player.m_last_time) / 1000 >= 1)
        {
            console.log(`removed ${i}`)
            globals.m_players[i].m_html.remove();
            globals.m_players[i].m_player_card.m_parent_element.remove();
            globals.m_players[i] = undefined;
        }
    }
}

const dom_content_loaded = async () => {
    globals.ui_container_element = document.querySelector(".ui-container");

    globals.m_terro_div = document.querySelector("#t");
    globals.m_counter_div = document.querySelector("#ct");

    globals.map.m_div = document.querySelector("#radar");
    globals.map.m_image = document.querySelector("#radar-image");

    let latency_container = document.createElement("div");
    latency_container.classList.add("radar__latency");

    let latency_icon = document.createElement("img");
    latency_icon.style.width = "1.3rem";
    latency_icon.src = "./assets/icons/gauge.svg";
    globals.latency.m_html = document.createElement("span");

    latency_container.appendChild(latency_icon);
    latency_container.appendChild(globals.latency.m_html);

    globals.ui_container_element.appendChild(latency_container);

    await setup_connection();
}

document.addEventListener("DOMContentLoaded", dom_content_loaded);