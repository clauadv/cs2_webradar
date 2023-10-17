import { MaskedIcon } from "./MaskedIcon";
import { get_color } from "./Radar";

export const PlayerCard = ({ playerData, right }) => {
    return (
        <li className={`flex ${right && `flex-row-reverse`}`}>
            <div className="flex flex-col gap-[0.375rem] justify-center items-center">
                <div>{playerData.m_name}</div>
                <div className="triangle" style={{ borderColor: `${get_color(playerData.m_color)} transparent transparent transparent` }}></div>
                <img className={`h-[128px] ${right && `scale-x-[-1]`}`} src={`./assets/characters/${playerData.m_model_name}.png`}></img>
            </div>

            <div className={`flex flex-col ${right && `flex-row-reverse`} justify-center gap-2`}>
                <span className={`${right && `flex justify-end`} text-radar-green`}>${playerData.m_money}</span>

                <div className={`flex ${right && `flex-row-reverse`} gap-2`}>
                    <div className="flex gap-[2px] items-center">
                        <MaskedIcon path={`./assets/equipment/health.svg`} height={16}></MaskedIcon>
                        <span className="text-radar-primary">{playerData.m_health}</span>
                    </div>

                    <div className="flex gap-[2px] items-center">
                        <MaskedIcon path={`./assets/equipment/${playerData.m_has_helmet && `armor_helmet` || `armor`}.svg`} height={16}></MaskedIcon>
                        <span className="text-radar-primary">{playerData.m_armor}</span>
                    </div>
                </div>

                <div className={`flex ${right && `flex-row-reverse`} gap-3`}>
                    {
                        playerData.m_weapons && playerData.m_weapons.primary &&
                        <MaskedIcon path={`./assets/equipment/${playerData.m_weapons.primary}.svg`} height={28} color={`${playerData.m_weapons.m_active == playerData.m_weapons.primary && `primary` || `secondary`}`}></MaskedIcon>
                    }

                    {
                        playerData.m_weapons && playerData.m_weapons.secondary &&
                        <MaskedIcon path={`./assets/equipment/${playerData.m_weapons.secondary}.svg`} height={28} color={`${playerData.m_weapons.m_active == playerData.m_weapons.secondary && `primary` || `secondary`}`}></MaskedIcon>
                    }

                    {
                        playerData.m_weapons && playerData.m_weapons.knife &&
                        <MaskedIcon path={`./assets/equipment/${playerData.m_weapons.knife}.svg`} height={28} color={`${playerData.m_weapons.m_active == playerData.m_weapons.knife && `primary` || `secondary`}`}></MaskedIcon>
                    }
                </div>

                <div className={`flex flex-col relative`}>
                    <div className={`flex ${right && `flex-row-reverse`} gap-9 mt-3 items-center`}>
                        {
                            playerData.m_weapons && playerData.m_weapons.utility &&
                            playerData.m_weapons.utility.map((utility) =>
                                <MaskedIcon key={utility} path={`./assets/equipment/${utility}.svg`} height={28} color={`${playerData.m_weapons.m_active == utility && `primary` || `secondary`}`}></MaskedIcon>
                            )
                        }

                        {
                            playerData.m_has_defuser &&
                            <MaskedIcon path={`./assets/equipment/defuser.svg`} height={28}></MaskedIcon>
                        }

                        {
                            [...Array(Math.max(4 - (playerData.m_weapons.utility && playerData.m_weapons.utility.length || 0), 0))].map((_, i) => (
                                <div key={i} className="rounded-full w-[6px] h-[6px] bg-radar-primary"></div>
                            ))
                        }
                    </div>
                </div>
            </div>
        </li>
    );
}