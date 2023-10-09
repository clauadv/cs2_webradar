const USE_NEW_DESIGN = true;

const globals =
{
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
        m_data_count: 0
    },

    local_player:
    {
        m_team: -1
    },

    m_left_div: null,
    m_right_div: null,

    m_players: []
};

const LOG_INFO = async (...args) => console.info(`[info] ${args.join(" ")}`);
const LOG_ERROR = async (...args) => console.error(`[error] ${args.join(" ")}`);

const draw_latency = async () =>
{
    const current_time = new Date().getTime();
    const last_connection_time = globals.latency.m_last_time;
    const diff_in_ms = current_time - last_connection_time;

    if (globals.latency.m_average_time === 0)
    {
        globals.latency.m_average_time = diff_in_ms;
    }

    globals.latency.m_html.innerText = `${globals.latency.m_average_time}ms`;

    if (globals.latency.m_average_calc >= 10)
    {
        globals.latency.m_average_time = globals.latency.m_average_calc / globals.latency.m_data_count;

        globals.latency.m_data_count = 0;
        globals.latency.m_average_calc = 0;
    }
    else
    {
        globals.latency.m_data_count++;
        globals.latency.m_average_calc += diff_in_ms;
    }

    globals.latency.m_last_time = new Date().getTime();
}

const setup_connection = async () =>
{
    const web_socket = new WebSocket("ws://188.24.175.93:22006/cs2_webradar");

    web_socket.onopen = async () =>
    {
        LOG_INFO("connected to the web socket");
        on_map_change();
    }

    web_socket.onclose = async () =>
    {
        LOG_ERROR("disconnected from the web socket");
    }

    web_socket.onerror = async (error) =>
    {
        LOG_ERROR(`${error}`);
    }

    web_socket.onmessage = async (event) =>
    {
        const parsed_data = JSON.parse(await event.data.text());
        await update_radar(parsed_data);

        draw_latency();
    };
}

const get_color = (color) =>
{
    let new_color = "grey";

    switch (color)
    {
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

const calculate_rotation = (view_angle, player) =>
{
    player.m_rotation = (player.m_rotation || 0) % 360;
    player.m_rotation += (view_angle - player.m_rotation + 540) % 360 - 180;

    return player.m_rotation;
}

const add_player = (idx) =>
{
    const div = document.createElement("div");
    div.classList.add("player");

    const angle = document.createElement("div");
    angle.classList.add("player__angle");

    div.appendChild(angle);

    globals.m_players[idx] =
    {
        m_html: div,
        m_angle_html: angle
    };

    globals.map.m_div.appendChild(globals.m_players[idx].m_html);
}

const update_player = (idx, data) =>
{
    if (!globals.m_players[idx])
    {
        add_player(idx);
        return;
    }

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

const on_map_change = async () =>
{
    globals.map.m_image.src = `data/${globals.map.m_current}/radar.png`;
    globals.map.m_image.classList.add("radar__image");
    globals.map.m_data = await (await fetch(`data/${globals.map.m_current}/data.json`)).json();
    LOG_INFO(`changed map to ${globals.map.m_current}`);
}

const get_radar_position = (coords) =>
{
    if (!(coords.x || coords.y))
    {
        LOG_ERROR("player positions are invalid");
        return { x: 0, y: 0 };
    }

    if (!(globals.map.m_data.x || globals.map.m_data.y))
    {
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

const update_radar = async (data) =>
{
    if (!data.m_map)
    {
        LOG_ERROR("data.m_map is null");
        return;
    }

    if (data.m_map !== globals.map.m_current)
    {
        globals.map.m_current = data.m_map;
        await on_map_change();
    }

    if (!data.players)
    {
        LOG_ERROR("data.players is null");
        return;
    }

    globals.local_player.m_team = data.local_player[0].m_team;

    data.players.forEach(player => update_player(player.m_idx, player.data));

    const current_time = new Date().getTime();
    globals.m_players.filter(player => (current_time - player.m_last_time) / 1000 >= 1).forEach(player => player.m_html.remove());
}

const dom_content_loaded = async () =>
{
    let old_container = document.getElementById("old-container");

    if (USE_NEW_DESIGN) 
    {   
        old_container.style.display = "none";
        document.getElementsByClassName("ui-container")[0].style.display = "flex";
        return;
    }

    globals.map.m_div = document.createElement("div");
    globals.map.m_image = document.createElement("img");

    globals.m_left_div = document.createElement("div");
    globals.m_right_div = document.createElement("div");

    globals.map.m_div.classList.add("radar__container");
    globals.map.m_image.classList.add("radar__image");

    globals.map.m_div.appendChild(globals.map.m_image);
    old_container.appendChild(globals.m_left_div);
    old_container.appendChild(globals.map.m_div);
    old_container.appendChild(globals.m_right_div);

    globals.m_left_div.classList.add("player__list");
    globals.m_right_div.classList.add("player__list");

    globals.latency.m_html = document.createElement("div");
    globals.latency.m_html.classList.add("radar__latency");

    globals.map.m_div.appendChild(globals.latency.m_html);
    //globals.map.m_div.style.transform = `scale(${globals.map.m_zoom_level})`
   // globals.map.m_image.style.transform = `scale(${globals.map.m_zoom_level})`;
    await setup_connection();
}

document.addEventListener("DOMContentLoaded", dom_content_loaded);