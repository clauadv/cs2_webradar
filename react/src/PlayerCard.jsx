import { MaskedIcon } from "./MaskedIcon";
import { get_color } from "./Radar";

export const PlayerCard = ({ playerData, right }) => {
    return (
        <li style={{ opacity: `${playerData.m_is_dead && `0.5` || `1`}` }} className={`flex ${right && `flex-row-reverse`}`}>
            <div className={`flex flex-col gap-[0.375rem] justify-center items-center`}>
                <div className={`hover:cursor-pointer`} onClick={() => window.open(`https://faceitfinder.com/profile/${playerData.m_steam_id}`, '_blank', 'noopener,noreferrer')}>{playerData.m_name}</div>
                <div className={`w-0 h-0 border-solid border-t-[12px] border-r-[8px] border-b-[12px] border-l-[8px]`} style={{ borderColor: `${get_color(playerData.m_color)} transparent transparent transparent` }}></div>
                <img className={`h-[128px] ${right && `scale-x-[-1]`}`} src={`./assets/characters/${playerData.m_model_name}.png`}></img>
            </div>

            <div className={`flex flex-col ${right && `flex-row-reverse`} justify-center gap-2`}>
                <span className={`${right && `flex justify-end`} text-radar-green`}>${playerData.m_money}</span>

                <div className={`flex ${right && `flex-row-reverse`} gap-2`}>
                    <div className="flex gap-[4px] items-center">
                        <MaskedIcon path={`./assets/icons/health.svg`} height={16} />
                        <span className="text-radar-primary">{playerData.m_health}</span>
                    </div>

                    <div className="flex gap-[4px] items-center">
                        <MaskedIcon path={`./assets/icons/${playerData.m_has_helmet && `kevlar_helmet` || `kevlar`}.svg`} height={16} />
                        <span className="text-radar-primary">{playerData.m_armor}</span>
                    </div>
                </div>

                <div className={`flex ${right && `flex-row-reverse`} gap-3`}>
                    {
                        playerData.m_weapons && playerData.m_weapons.primary &&
                        <MaskedIcon path={`./assets/icons/${playerData.m_weapons.primary}.svg`} height={28} color={`${playerData.m_weapons.m_active == playerData.m_weapons.primary && `primary` || `secondary`}`} active={playerData.m_weapons.m_active == playerData.m_weapons.primary} />
                    }

                    {
                        playerData.m_weapons && playerData.m_weapons.secondary &&
                        <MaskedIcon path={`./assets/icons/${playerData.m_weapons.secondary}.svg`} height={28} color={`${playerData.m_weapons.m_active == playerData.m_weapons.secondary && `primary` || `secondary`}`} active={playerData.m_weapons.m_active == playerData.m_weapons.secondary} />
                    }

                    {
                        playerData.m_weapons && playerData.m_weapons.knife &&
                        <MaskedIcon path={`./assets/icons/${playerData.m_weapons.knife}.svg`} height={28} color={`${playerData.m_weapons.m_active == playerData.m_weapons.knife && `primary` || `secondary`}`} active={playerData.m_weapons.m_active == playerData.m_weapons.knife} />
                    }
                </div>

                <div className={`flex flex-col relative`}>
                    <div className={`flex ${right && `flex-row-reverse`} gap-9 mt-3 items-center`}>
                        {
                            playerData.m_weapons && playerData.m_weapons.utility &&
                            playerData.m_weapons.utility.map((utility) =>
                                <MaskedIcon key={utility} path={`./assets/icons/${utility}.svg`} height={28} color={`${playerData.m_weapons.m_active == utility && `primary` || `secondary`}`} active={playerData.m_weapons.m_active == utility} />
                            )
                        }

                        {
                            [...Array(Math.max(4 - ((playerData.m_weapons && playerData.m_weapons.utility) && playerData.m_weapons.utility.length || 0), 0))].map((_, i) => (
                                <div key={i} className="rounded-full w-[6px] h-[6px] bg-radar-primary"></div>
                            ))
                        }

                        {
                            playerData.m_has_defuser &&
                            <MaskedIcon path={`./assets/icons/defuser.svg`} height={28} />
                            ||
                            playerData.m_has_bomb &&
                            <MaskedIcon path={`./assets/icons/c4.svg`} height={28} />
                        }
                    </div>
                </div>
            </div>
        </li>
    );
}