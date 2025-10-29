import { useState, useEffect } from "react";
import MaskedIcon from "./maskedicon";
import { playerColors, teamEnum, colorSchemePallette } from "../utilities/utilities";

const PlayerCard = ({ playerData, isOnRightSide, settings }) => {
  const [modelName, setModelName] = useState(playerData.m_model_name);

  function getArmor(armor) {
    if (armor<=100 && armor>0) {
      return armor;
    } else {
      return 0;
    }
  }

  useEffect(() => {
    if (playerData.m_model_name)
      setModelName(playerData.m_model_name);
  }, [playerData.m_model_name]);

  return (
    <li
      style={{ opacity: `${(playerData.m_is_dead && `0.5`) || `1`}` }}
      className={`flex ${isOnRightSide && `flex-row-reverse`}`}
    >
      <div className="flex items-center mr-2">

        <div
          className={`flex flex-col gap-[0.375rem] justify-center items-center`}
        >
          <div
            className={`hover:cursor-pointer`}
            onClick={() =>
              window.open(
                `https://steamcommunity.com/profiles/${playerData.m_steam_id}`,
                "_blank",
                "noopener,noreferrer"
              )
            }
          >
            {playerData.m_name}
          </div>
          {/* <div
            className={`w-0 h-0 border-solid border-t-[12px] border-r-[8px] border-b-[12px] border-l-[8px]`}
            style={{
              borderColor: `${
                playerColors[playerData.m_color]
              } transparent transparent transparent`,
            }}
          ></div> */}
          <img
            className={`h-[8rem] ${isOnRightSide && `scale-x-[-1]`}`}
            src={`./assets/characters/${modelName}.png`}
            style={{backgroundColor:playerColors[playerData.m_color], borderRadius:'30px'}}
          ></img>
        </div>

      </div>

      <div
        className={`flex flex-col ${
          isOnRightSide && `flex-row-reverse`
        } justify-center gap-2`}
      >
        <span
          className={`${isOnRightSide && `flex justify-end`} text-radar-green`}
        >
          ${playerData.m_money}
        </span>

        <div className={`flex ${isOnRightSide && `flex-row-reverse`} gap-2`}>
          <div className="flex gap-[4px] items-center">
            <MaskedIcon
              path={`./assets/icons/health.svg`}
              height={16}
              color={`${colorSchemePallette[settings.colorScheme][1]}`}
            />
            <div className="w-20 h-4 bg-neutral-700 rounded-sm border border-black/50 overflow-hidden">
              <div
                className={`h-full transition-all duration-600`}
                style={{ width: `${playerData.m_health}%`, backgroundColor: colorSchemePallette[settings.colorScheme][1] }}
              ></div>
            </div>
            <span className="text-radar-primary">{playerData.m_health}</span>
          </div>

          <div className="flex gap-[4px] items-center">
            <MaskedIcon
              path={`./assets/icons/${
                (playerData.m_has_helmet && `kevlar_helmet`) || `kevlar`
              }.svg`}
              height={16}
              color={`${colorSchemePallette[settings.colorScheme][1]}`}
            />
            <span className="text-radar-primary">{getArmor(playerData.m_armor)}</span>
          </div>
        </div>

        <div className={`flex ${isOnRightSide && `flex-row-reverse`} gap-3`}>
          {playerData.m_weapons && playerData.m_weapons.m_primary && (
            <MaskedIcon
              path={`./assets/icons/${playerData.m_weapons.m_primary}.svg`}
              height={28}
              color={`${
                (playerData.m_weapons.m_active ==
                  playerData.m_weapons.m_primary &&
                  `${colorSchemePallette[settings.colorScheme][0]}`) ||
                `${colorSchemePallette[settings.colorScheme][1]}`
              }`}
            />
          )}

          {playerData.m_weapons && playerData.m_weapons.m_secondary && (
            <MaskedIcon
              path={`./assets/icons/${playerData.m_weapons.m_secondary}.svg`}
              height={28}
              color={`${
                (playerData.m_weapons.m_active ==
                  playerData.m_weapons.m_secondary &&
                  `${colorSchemePallette[settings.colorScheme][0]}`) ||
                `${colorSchemePallette[settings.colorScheme][1]}`
              }`}
            />
          )}

          {playerData.m_weapons &&
            playerData.m_weapons.m_melee &&
            playerData.m_weapons.m_melee.map((melee) => (
              <MaskedIcon
                key={melee}
                path={`./assets/icons/${melee}.svg`}
                height={28}
                color={`${
                  (playerData.m_weapons.m_active == melee &&
                    `${colorSchemePallette[settings.colorScheme][0]}`) ||
                  `${colorSchemePallette[settings.colorScheme][1]}`
                }`}
              />
            ))}
        </div>

        <div className={`flex flex-col relative`}>
          <div
            className={`flex ${
              isOnRightSide && `flex-row-reverse`
            } gap-9 mt-3 items-center`}
          >
            {playerData.m_weapons &&
              playerData.m_weapons.m_utilities &&
              playerData.m_weapons.m_utilities.map((utility) => (
                <MaskedIcon
                  key={utility}
                  path={`./assets/icons/${utility}.svg`}
                  height={28}
                  color={`${
                    (playerData.m_weapons.m_active == utility &&
                      `${colorSchemePallette[settings.colorScheme][0]}`) ||
                    `${colorSchemePallette[settings.colorScheme][1]}`
                  }`}
                />
              ))}

            {[
              ...Array(
                Math.max(
                  4 -
                    ((playerData.m_weapons &&
                      playerData.m_weapons.m_utilities &&
                      playerData.m_weapons.m_utilities.length) ||
                      0),
                  0
                )
              ),
            ].map((_, i) => (
              <div
                key={i}
                className={`rounded-full w-[6px] h-[6px]`}
                style = {{backgroundColor:colorSchemePallette[settings.colorScheme][1]}}
              ></div>
            ))}

            {(playerData.m_team == teamEnum.counterTerrorist &&
              playerData.m_has_defuser && (
                <MaskedIcon
                  path={`./assets/icons/defuser.svg`}
                  height={28}
                  color={`${colorSchemePallette[settings.colorScheme][1]}`}
                />
              )) ||
              (playerData.m_team == teamEnum.terrorist &&
                playerData.m_has_bomb && (
                  <MaskedIcon
                    path={`./assets/icons/c4.svg`}
                    height={28}
                    color={
                      ((playerData.m_weapons &&
                        playerData.m_weapons.m_active) == `c4` &&
                        `${colorSchemePallette[settings.colorScheme][0]}`) ||
                      `${colorSchemePallette[settings.colorScheme][1]}`
                    }
                  />
                ))}
          </div>
        </div>
      </div>
    </li>
  );
};

export default PlayerCard;
