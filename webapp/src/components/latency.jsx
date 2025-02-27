import SettingsButton from "./SettingsButton";

let latencyData = {
  averageCount: 0,
  averageSum: 0,
  averageTime: 0,
  lastTime: new Date().getTime(),
};

export const getLatency = () => {
  let currentTime = new Date().getTime();
  let diffInMs = currentTime - latencyData.lastTime;
  latencyData.lastTime = currentTime;

  if (latencyData.averageTime == 0) latencyData.averageTime = diffInMs;

  latencyData.averageCount++;
  latencyData.averageSum += diffInMs;

  if (latencyData.averageCount >= 5) {
    latencyData.averageTime = latencyData.averageSum / latencyData.averageCount;

    latencyData.averageCount = 0;
    latencyData.averageSum = 0;
  }

  return latencyData.averageTime;
};

export const Latency = ({ value, settings, setSettings }) => {
  return (
    <div className={`flex gap-2 absolute text-[normal] right-2.5 top-2.5`}>
      <div className={'flex gap-1'}>
        <img className={`w-[1.3rem]`} src={`./assets/icons/gauge.svg`} />
        <span>{value.toFixed(0)}ms</span>
      </div>

      <SettingsButton settings={settings} onSettingsChange={setSettings} />
    </div>
  );
};
