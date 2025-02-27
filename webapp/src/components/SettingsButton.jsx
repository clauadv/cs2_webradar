import { useState } from "react";

const SettingsButton = ({ settings, onSettingsChange }) => {
  const [isOpen, setIsOpen] = useState(false);

  return (
    <div className="z-50">
      <button
        onClick={() => setIsOpen(!isOpen)}
        className="flex items-center gap-1 transition-all rounded-xl"
      >
        <img className={`w-[1.3rem]`} src={`./assets/icons/cog.svg`} />
        <span className="text-radar-primary">Settings</span>
      </button>

      {isOpen && (
        <div className="absolute right-0 mt-2 w-64 bg-radar-panel/90 backdrop-blur-lg rounded-xl p-4 shadow-xl border border-radar-secondary/20">
          <h3 className="text-radar-primary text-lg font-semibold mb-4">Radar Settings</h3>

          <div className="space-y-3">
            <div>
              <div className="flex justify-between items-center mb-2">
                <span className="text-radar-secondary text-sm">Dot Size</span>
                <span className="text-radar-primary text-sm font-mono">{settings.dotSize}x</span>
              </div>
              <input
                type="range"
                min="0.5"
                max="2"
                step="0.1"
                value={settings.dotSize}
                onChange={(e) => onSettingsChange({ ...settings, dotSize: parseFloat(e.target.value) })}
                className="w-full h-2 rounded-lg appearance-none cursor-pointer accent-radar-primary"
                style={{
                  background: `linear-gradient(to right, #b1d0e7 ${((settings.dotSize - 0.5) / 1.5) * 100}%, rgba(59, 130, 246, 0.2) ${((settings.dotSize - 0.5) / 1.5) * 100}%)`
                }}
              />
            </div>

            <div>
              <div className="flex justify-between items-center mb-2">
                <span className="text-radar-secondary text-sm">Bomb Size</span>
                <span className="text-radar-primary text-sm font-mono">{settings.bombSize}x</span>
              </div>
              <input
                type="range"
                min="0.1"
                max="2"
                step="0.1"
                value={settings.bombSize}
                onChange={(e) => onSettingsChange({ ...settings, bombSize: parseFloat(e.target.value) })}
                className="w-full h-2 rounded-lg appearance-none cursor-pointer accent-radar-primary"
                style={{
                  background: `linear-gradient(to right, #b1d0e7 ${((settings.bombSize - 0.1) / 1.9) * 100}%, rgba(59, 130, 246, 0.2) ${((settings.bombSize - 0.1) / 1.9) * 100}%)`
                }}
              />
            </div>

            <div className="space-y-1">
              <label className="flex items-center justify-between p-3 rounded-lg hover:bg-radar-secondary/20 transition-colors cursor-pointer">
                <span className="text-radar-secondary text-sm">Ally Names</span>
                <input
                  type="checkbox"
                  checked={settings.showAllNames}
                  onChange={(e) => onSettingsChange({ ...settings, showAllNames: e.target.checked })}
                  className="relative h-5 w-9 rounded-full shadow-sm bg-radar-secondary/30 checked:bg-radar-secondary transition-colors duration-200 appearance-none before:absolute before:h-4 before:w-4 before:top-0.5 before:left-0.5 before:bg-white before:rounded-full before:transition-transform before:duration-200 checked:before:translate-x-4"
                />
              </label>

              <label className="flex items-center justify-between p-3 rounded-lg hover:bg-radar-secondary/20 transition-colors cursor-pointer">
                <span className="text-radar-secondary text-sm">Enemy Names</span>
                <input
                  type="checkbox"
                  checked={settings.showEnemyNames}
                  onChange={(e) => onSettingsChange({ ...settings, showEnemyNames: e.target.checked })}
                  className="relative h-5 w-9 rounded-full shadow-sm bg-radar-secondary/30 checked:bg-radar-secondary transition-colors duration-200 appearance-none before:absolute before:h-4 before:w-4 before:top-0.5 before:left-0.5 before:bg-white before:rounded-full before:transition-transform before:duration-200 checked:before:translate-x-4"
                />
              </label>

              <label className="flex items-center justify-between p-3 rounded-lg hover:bg-radar-secondary/20 transition-colors cursor-pointer">
                <span className="text-radar-secondary text-sm">View Cones</span>
                <input
                  type="checkbox"
                  checked={settings.showViewCones}
                  onChange={(e) => onSettingsChange({ ...settings, showViewCones: e.target.checked })}
                  className="relative h-5 w-9 rounded-full shadow-sm bg-radar-secondary/30 checked:bg-radar-secondary transition-colors duration-200 appearance-none before:absolute before:h-4 before:w-4 before:top-0.5 before:left-0.5 before:bg-white before:rounded-full before:transition-transform before:duration-200 checked:before:translate-x-4"
                />
              </label>
            </div>
          </div>
        </div>
      )}
    </div>
  );
};

export default SettingsButton;