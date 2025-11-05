import { useState, useEffect } from "react";
import MaskedIcon from "./maskedicon";
import { playerColors, teamEnum } from "../utilities/utilities";

const PlayerCard = ({ playerData, isOnRightSide }) => {
  const [modelName, setModelName] = useState(playerData.m_model_name);

  useEffect(() => {
    if (playerData.m_model_name) setModelName(playerData.m_model_name);
  }, [playerData.m_model_name]);

  return (
    <li style={{ opacity: playerData.m_is_dead ? 0.5 : 1 }} className={`flex gap-3 ${isOnRightSide && 'flex-row-reverse'}`}>
      <div className="flex flex-col gap-2 justify-center items-center">
        <div className="text-sm text-center max-w-[100px] truncate">{playerData.m_name}</div>
        <div className="w-0 h-0 border-solid border-t-[10px] border-r-[7px] border-b-[10px] border-l-[7px]" style={{ borderColor: `${playerColors[playerData.m_color]} transparent transparent transparent` }}></div>
        <img className={`h-28 ${isOnRightSide && 'scale-x-[-1]'}`} src={`./assets/characters/${modelName}.png`} alt="" />
      </div>
      <div className={`flex flex-col ${isOnRightSide && 'items-end'} justify-center gap-2`}>
        <span className="text-radar-green text-sm">${playerData.m_money}</span>
        <div className="flex gap-2">
          <div className="flex gap-1 items-center"><MaskedIcon path="./assets/icons/health.svg" height={14} color="bg-radar-secondary" /><span className="text-xs">{playerData.m_health}</span></div>
          <div className="flex gap-1 items-center"><MaskedIcon path={`./assets/icons/${playerData.m_has_helmet ? 'kevlar_helmet' : 'kevlar'}.svg`} height={14} color="bg-radar-secondary" /><span className="text-xs">{playerData.m_armor}</span></div>
        </div>
        <div className="flex gap-2">
          {playerData.m_weapons?.m_primary && <MaskedIcon path={`./assets/icons/${playerData.m_weapons.m_primary}.svg`} height={24} color={playerData.m_weapons.m_active == playerData.m_weapons.m_primary ? 'bg-radar-primary' : 'bg-radar-secondary'} />}
          {playerData.m_weapons?.m_secondary && <MaskedIcon path={`./assets/icons/${playerData.m_weapons.m_secondary}.svg`} height={24} color={playerData.m_weapons.m_active == playerData.m_weapons.m_secondary ? 'bg-radar-primary' : 'bg-radar-secondary'} />}
          {playerData.m_weapons?.m_melee?.map((m) => <MaskedIcon key={m} path={`./assets/icons/${m}.svg`} height={24} color={playerData.m_weapons.m_active == m ? 'bg-radar-primary' : 'bg-radar-secondary'} />)}
        </div>
        <div className="flex gap-2 flex-wrap max-w-[120px]">
          {playerData.m_weapons?.m_utilities?.map((u) => <MaskedIcon key={u} path={`./assets/icons/${u}.svg`} height={22} color={playerData.m_weapons.m_active == u ? 'bg-radar-primary' : 'bg-radar-secondary'} />)}
          {playerData.m_team == teamEnum.counterTerrorist && playerData.m_has_defuser && <MaskedIcon path="./assets/icons/defuser.svg" height={22} color="bg-radar-secondary" />}
          {playerData.m_team == teamEnum.terrorist && playerData.m_has_bomb && <MaskedIcon path="./assets/icons/c4.svg" height={22} color={playerData.m_weapons?.m_active == 'c4' ? 'bg-radar-primary' : 'bg-radar-secondary'} />}
        </div>
      </div>
    </li>
  );
};

export default PlayerCard;


