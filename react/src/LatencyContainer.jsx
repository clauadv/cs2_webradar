export const LatencyContainer = ({ value }) => {
    return (
        <div className={`flex gap-1 absolute text-[normal] right-2.5 top-2.5`}>
            <img className={`w-[1.3rem]`} src={`./assets/icons/gauge.svg`}></img>
            <span>{value}ms</span>
        </div>
    )
}