
let LatencyData = {
    averageCount: 0,
    averageSum: 0,
    averageTime: 0,
    lastTime: new Date().getTime()
}

export const GetLatency = () => {
    let currentTime = new Date().getTime();
    let diffInMs = currentTime - LatencyData.lastTime;
    LatencyData.lastTime = currentTime;

    if (LatencyData.averageTime == 0)
        LatencyData.averageTime = diffInMs;

    LatencyData.averageCount++;
    LatencyData.averageSum += diffInMs;

    if (LatencyData.averageCount >= 5) {
        LatencyData.averageTime = LatencyData.averageSum / LatencyData.averageCount;

        LatencyData.averageCount = 0;
        LatencyData.averageSum = 0;
    }

    return LatencyData.averageTime;
}

export const LatencyContainer = ({ value }) => {
    return (
        <div className={`flex gap-1 absolute text-[normal] right-2.5 top-2.5`}>
            <img className={`w-[1.3rem]`} src={`./assets/icons/gauge.svg`}></img>
            <span>{value.toFixed(0)}ms</span>
        </div>
    )
}