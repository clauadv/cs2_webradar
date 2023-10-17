export const MaskedIcon = ({ path, height, color, active }) => {
    return (
        <div className={`${!active && `bg-radar-secondary`} bg-radar-${color}`} style={{ WebkitMask: `url(${path}) no-repeat center / contain`, width: `auto`, height: height }}>
            <img className="w-full h-full opacity-0" src={path}></img>
        </div>
    )
}