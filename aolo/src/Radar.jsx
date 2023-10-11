export const Mot = (props) => {
    const { data, map_data, radar_img } = props;

    const samp = get_radar_position(map_data, data.m_position);

    const image_bounding = radar_img.getBoundingClientRect();
    var image_translation =
    {
        x: (image_bounding.width * samp.x),
        y: (image_bounding.height * samp.y)
    };

    return (
        <div className="player" style={`transform: translate(${image_translation.x}px, ${image_translation.y}px);`}>
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
    const { players, image, map_data } = props;

    let radar_img = null;

    return (
        <div id="radar" class="radar">
            <img ref={ radar_img } class="radar-image" src={ image } />
            <For each={players()}>{(player) =>
                <Mot data={ player.data } map_data={ map_data } radar_img={ radar_img }></Mot>
            }</For>
        </div>
    )
}