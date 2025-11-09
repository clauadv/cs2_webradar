import { useState, useRef } from "react";

const SettingsButton = ({ settings, onSettingsChange }) => {
  const [isOpen, setIsOpen] = useState(false);
  const settingsMenu = useRef(null);

  const closeSettingsIfOpen = (e)=>{
    if(isOpen && !settingsMenu.current?.contains(e.target) && !settingsbut.contains(e.target)){
      setIsOpen(false)
    }
  }

  document.addEventListener('mousedown',closeSettingsIfOpen)

  return (
    <div className="z-50">
      {window.innerHeight>500 && (
      <button
        onClick={() => setIsOpen(!isOpen)}
        className="flex items-center gap-1 transition-all rounded-xl"
        id="settingsbut"
      >
        <img className={`w-[1.3rem]`} src={`./assets/icons/cog.svg`} />
        <span className="text-radar-primary">Settings</span>
      </button>
      )}

      {isOpen && (
        <div className="absolute right-0 mt-2 bg-black bg-opacity-50 w-64 backdrop-blur-lg rounded-xl p-4 shadow-xl border border-radar-secondary/20" ref={settingsMenu}>
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
                <label className="flex items-center justify-between p-3 rounded-lg hover:bg-radar-secondary_default/20 transition-colors cursor-pointer">
                <span className="text-radar-secondary text-sm">Show Only Enemies</span>
                <input
                  type="checkbox"
                  checked={settings.showOnlyEnemies}
                  onChange={(e) => onSettingsChange({ ...settings, showOnlyEnemies: e.target.checked })}
                  className="relative h-5 w-9 rounded-full shadow-sm bg-radar-secondary/30 checked:bg-radar-secondary transition-colors duration-200 appearance-none before:absolute before:h-4 before:w-4 before:top-0.5 before:left-0.5 before:bg-white before:rounded-full before:transition-transform before:duration-200 checked:before:translate-x-4"
                />
              </label>
              {(!settings.showOnlyEnemies) && (
              <label className="flex items-center justify-between p-3 rounded-lg hover:bg-radar-secondary/20 transition-colors cursor-pointer">
                <span className="text-radar-secondary text-sm">Ally Names</span>
                <input
                  type="checkbox"
                  checked={settings.showAllNames}
                  onChange={(e) => onSettingsChange({ ...settings, showAllNames: e.target.checked })}
                  className="relative h-5 w-9 rounded-full shadow-sm bg-radar-secondary/30 checked:bg-radar-secondary transition-colors duration-200 appearance-none before:absolute before:h-4 before:w-4 before:top-0.5 before:left-0.5 before:bg-white before:rounded-full before:transition-transform before:duration-200 checked:before:translate-x-4"
                />
              </label>
              )}

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
                <span className="text-radar-secondary text-sm">View Player Cones</span>
                <input
                  type="checkbox"
                  checked={settings.showViewCones}
                  onChange={(e) => onSettingsChange({ ...settings, showViewCones: e.target.checked })}
                  className="relative h-5 w-9 rounded-full shadow-sm bg-radar-secondary/30 checked:bg-radar-secondary transition-colors duration-200 appearance-none before:absolute before:h-4 before:w-4 before:top-0.5 before:left-0.5 before:bg-white before:rounded-full before:transition-transform before:duration-200 checked:before:translate-x-4"
                />
              </label>

              <label className="flex items-center justify-between p-3 rounded-lg hover:bg-radar-secondary/20 transition-colors cursor-pointer">
                <span className="text-radar-secondary text-sm">Show Grenades</span>
                <input
                  type="checkbox"
                  checked={settings.showGrenades}
                  onChange={(e) => onSettingsChange({ ...settings, showGrenades: e.target.checked })}
                  className="relative h-5 w-9 rounded-full shadow-sm bg-radar-secondary/30 checked:bg-radar-secondary transition-colors duration-200 appearance-none before:absolute before:h-4 before:w-4 before:top-0.5 before:left-0.5 before:bg-white before:rounded-full before:transition-transform before:duration-200 checked:before:translate-x-4"
                />
              </label>

              {settings.showGrenades && (
                <div>

                  <label className="flex items-center justify-between p-3 rounded-lg hover:bg-radar-secondary/20 transition-colors cursor-pointer">
                  <span className="text-radar-secondary text-sm">ㅤㅤ↑ Grenade Color</span>
                  <input
                    type="color"
                    value={settings.thrownGrenadeColor}
                    onChange={(e) => onSettingsChange({ ...settings, thrownGrenadeColor: e.target.value })}
                    className="relative h-5 w-9 rounded-full bg-radar-secondary/0"
                  />
                  </label>

                  <div className="flex justify-between items-center mb-2 mt-1.5 space-y-2">
                  <span className="text-radar-secondary text-sm">ㅤㅤㅤ↑ Grenade Size</span>
                  <span className="text-radar-primary text-sm font-mono">{settings.thrownGrenadeSize}x</span>
                  </div>
                  <input
                    type="range"
                    min="0.1"
                    max="2"
                    step="0.1"
                    value={settings.thrownGrenadeSize}
                    onChange={(e) => onSettingsChange({ ...settings, thrownGrenadeSize: parseFloat(e.target.value) })}
                    className="w-full ml-11 mb-2 h-2 rounded-lg appearance-none cursor-pointer accent-radar-primary"
                    style={{
                      background: `linear-gradient(to right, #b1d0e7 ${((settings.thrownGrenadeSize - 0.1) / 1.9) * 100}%, rgba(59, 130, 246, 0.2) ${((settings.thrownGrenadeSize - 0.1) / 1.9) * 100}%)`,
                      width: "80%"
                    }}
                  />

                </div>
              )}

              <label className="flex items-center justify-between p-3 rounded-lg hover:bg-radar-secondary/20 transition-colors cursor-pointer">
                <span className="text-radar-secondary text-sm">Show Droppped Weapons</span>
                <input
                  type="checkbox"
                  checked={settings.showDroppedWeapons}
                  onChange={(e) => onSettingsChange({ ...settings, showDroppedWeapons: e.target.checked })}
                  className="relative h-5 w-9 rounded-full shadow-sm bg-radar-secondary/30 checked:bg-radar-secondary transition-colors duration-200 appearance-none before:absolute before:h-4 before:w-4 before:top-0.5 before:left-0.5 before:bg-white before:rounded-full before:transition-transform before:duration-200 checked:before:translate-x-4"
                />
              </label>

              {settings.showDroppedWeapons && (
              <div>

                <label className="flex items-center justify-between p-3 rounded-lg hover:bg-radar-secondary/20 transition-colors cursor-pointer">
                <span className="text-radar-secondary text-sm">ㅤㅤ↑ Use Lighter Color</span>
                <input
                  type="checkbox"
                  checked={settings.droppedWeaponGlow}
                  onChange={(e) => onSettingsChange({ ...settings, droppedWeaponGlow: e.target.checked })}
                  className="relative h-5 w-9 rounded-full shadow-sm bg-radar-secondary/30 checked:bg-radar-secondary transition-colors duration-200 appearance-none before:absolute before:h-4 before:w-4 before:top-0.5 before:left-0.5 before:bg-white before:rounded-full before:transition-transform before:duration-200 checked:before:translate-x-4"
                />
                </label>

                <label className="flex items-center justify-between p-3 rounded-lg hover:bg-radar-secondary/20 transition-colors cursor-pointer">
                <span className="text-radar-secondary text-sm">ㅤㅤ↑ Ignore Grenades</span>
                <input
                  type="checkbox"
                  checked={settings.droppedWeaponIgnoreNade}
                  onChange={(e) => onSettingsChange({ ...settings, droppedWeaponIgnoreNade: e.target.checked })}
                  className="relative h-5 w-9 rounded-full shadow-sm bg-radar-secondary/30 checked:bg-radar-secondary transition-colors duration-200 appearance-none before:absolute before:h-4 before:w-4 before:top-0.5 before:left-0.5 before:bg-white before:rounded-full before:transition-transform before:duration-200 checked:before:translate-x-4"
                />
                </label>

                <div className="flex justify-between items-center mb-2 mt-1.5 space-y-2">
                  <span className="text-radar-secondary text-sm">ㅤㅤㅤ↑ Weapon Size</span>
                  <span className="text-radar-primary text-sm font-mono">{settings.droppedWeaponSize}x</span>
                </div>
                <input
                  type="range"
                  min="0.1"
                  max="2"
                  step="0.1"
                  value={settings.droppedWeaponSize}
                  onChange={(e) => onSettingsChange({ ...settings, droppedWeaponSize: parseFloat(e.target.value) })}
                  className="w-full ml-11 mb-2 h-2 rounded-lg appearance-none cursor-pointer accent-radar-primary"
                  style={{
                    background: `linear-gradient(to right, #b1d0e7 ${((settings.droppedWeaponSize - 0.1) / 1.9) * 100}%, rgba(59, 130, 246, 0.2) ${((settings.droppedWeaponSize - 0.1) / 1.9) * 100}%)`,
                    width: "80%"
                  }}
                />

              </div>
              )}

              <label className="flex items-center justify-between p-3 rounded-lg hover:bg-radar-secondary/20 transition-colors cursor-pointer">
                <span className="text-radar-secondary text-sm">Theme Color</span>
                <select 
                  value={settings.colorScheme} 
                  onChange={(e) => onSettingsChange({ ...settings, colorScheme: e.target.value })} 
                  className="ml-2 bg-radar-panel text-radar-primary rounded-md px-2 py-1 text-sm focus:outline-none focus:ring-2 focus:ring-radar-primary"
                  style={{
                  background: `rgba(59, 130, 246, 0.2)`,
                  border: `none`
                  }}
                  >
                  <option value="default">Default</option>
                  <option value="white">White</option>
                  <option value="light_blue">Light Blue</option>
                  <option value="dark_blue">Dark Blue</option> 
                  <option value="purple">Purple</option>
                  <option value="red">Red</option>
                  <option value="orange">Orange</option>
                  <option value="yellow">Yellow</option>
                  <option value="green">Green</option>
                  <option value="light_green">Light Green</option>
                  <option value="pink">Pink</option>
                </select>
              </label>

              <button className="flex items-center justify-between p-3 bg-radar-redbutton rounded-lg hover:bg-radar-redbutton_hover transition-colors cursor-pointer w-full" onClick={(e) => onSettingsChange({ ...settings, whichPlayerAreYou: "0" })}>
                  <span className="text-white text-sm ml-7">Choose Yourself Again</span>
              </button>
            </div>
          </div>
        </div>
      )}
    </div>
  );
};

export default SettingsButton;