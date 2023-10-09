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

    m_players: [],
    m_last_data: []
};

function convertTo0to360(angle) {
    // Ensure the angle is within the range -180 to 180
    while (angle < -180) {
        angle += 360;
    }
    while (angle > 180) {
        angle -= 360;
    }

    // If the angle is negative, convert it to the corresponding positive angle
    if (angle < 0) {
        angle += 360;
    }

    return angle;
}

const LOG_INFO = async (...args) => console.info(`[info] ${args.join(' ')}`);
const LOG_ERROR = async (...args) => console.error(`[error] ${args.join(' ')}`);

const draw_latency = async () => {
    const current_time = new Date().getTime();
    const last_connection_time = globals.latency.m_last_time;
    const diff_in_ms = current_time - last_connection_time;

    if (globals.latency.m_average_time === 0) {
        globals.latency.m_average_time = diff_in_ms;
    }

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
        on_map_change();
    }

    web_socket.onclose = async () => {
        LOG_ERROR("disconnected from the web socket");
    }

    web_socket.onerror = async (error) => {
        LOG_ERROR(`error: ${error}`);
    }

    web_socket.onmessage = async (event) => {
        const parsed_data = JSON.parse(await event.data.text());
        await update_radar(parsed_data);

        draw_latency();
    };
}

const add_player = (idx) => {
    const div = document.createElement("div");
    div.classList.add("player");

    const body = document.createElement("div");
    body.classList.add("player__body");

    div.appendChild(body);

    globals.m_players[idx] =
    {
        m_html: div
    };

    globals.map.m_div.appendChild(globals.m_players[idx].m_html);
}

function calculateRotaiton(nR, player) {
    var aR;
    player.rotation = player.rotation || 0; // if rot undefined or 0, make 0, else rot
    aR = player.rotation % 360;
    if ( aR < 0 ) { aR += 360; }
    if ( aR < 180 && (nR > (aR + 180)) ) { player.rotation -= 360; }
    if ( aR >= 180 && (nR <= (aR - 180)) ) { player.rotation += 360; }
    player.rotation += (nR - aR);

    return ("rotate( " + player.rotation + "deg )");
}
const update_player = (idx, data) => {
    if (!globals.m_players[idx]) {
        add_player(idx);
        return;
    }

    const old_data = globals.m_last_data[idx] == undefined ? data : globals.m_last_data[idx];
    console.log('old_data ' + old_data);
    
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

    console.log(JSON.stringify(data))

    const view_angle = 270 - data.m_eye_angle;

    div.style.opacity = (data.m_is_dead ? "0" : "1");
    div.style.backgroundColor = (data.m_team == globals.local_player.m_team ? "cyan" : "red");

    div.style.transition = `transform ${globals.latency.m_average_time}ms linear`;

    div.style.transform = `translate(${image_translation.x}px, ${image_translation.y}px) ${calculateRotaiton(view_angle, globals.m_players[idx])}`;
    
}

const on_map_change = async () => {
    globals.map.m_image.src = `data/${globals.map.m_current}/radar.png`;
    globals.map.m_image.classList.add('radar__image');
    globals.map.m_data = await (await fetch(`data/${globals.map.m_current}/data.json`)).json();
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

    data.players.forEach(player => {
        update_player(player.m_idx, player.data);
    });


    {
        var main_player = data.players[0];
        if (main_player !== undefined){
            const data = main_player.data;

            const position = get_radar_position(data.m_position);

            const image_bounding = globals.map.m_image.getBoundingClientRect();
           /* const image_translation =
            {
                x: (image_bounding.width * position.x - div_bounding.width * 0.5) / globals.map.m_zoom_level,
                y: (image_bounding.height * position.y - div_bounding.height * 0.5) / globals.map.m_zoom_level
            };
        */
           // globals.map.m_image.style.transform = `translate(${position.x * 100}%,${position.y * 100}%)`;

        }
    }

    const current_time = new Date().getTime();
    globals.m_players.filter(player => (current_time - player.m_last_time) / 1000 >= 1).forEach(player => player.m_html.remove());

}


const dom_content_loaded = async () => {
    globals.map.m_div = document.createElement("div");
    globals.map.m_image = document.createElement("img");

    globals.map.m_div.classList.add('radar__container');
    globals.map.m_image.classList.add('radar__image');

    globals.map.m_div.appendChild(globals.map.m_image);
    document.body.appendChild(globals.map.m_div);

    globals.latency.m_html = document.createElement('div');
    globals.latency.m_html.classList.add('radar__latency');

    globals.map.m_div.appendChild(globals.latency.m_html);
    //globals.map.m_div.style.transform = `scale(${globals.map.m_zoom_level})`
   // globals.map.m_image.style.transform = `scale(${globals.map.m_zoom_level})`;
    await setup_connection();
}

document.addEventListener("DOMContentLoaded", dom_content_loaded);