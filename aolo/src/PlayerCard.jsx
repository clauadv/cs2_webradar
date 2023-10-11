export const PlayerCard = (props) => {
    const { data } = props;

    return (
        <li class="player-card">
            <div class="player-card-first-row">
                <div class="player-card-name">{data.m_name}</div>
                <div class="player-card-vitals">
                    <div class="player-card-vital">
                        <div class="icon">
                            <img
                                class="icon-image"
                                src="./assets/icons/health.svg"
                                alt=""
                            />
                        </div>
                        {data.m_health}
                    </div>
                    <div class="player-card-vital">
                        <div class="masked-icon" style={`-webkit-mask: url('./assets/equipment/${data.m_has_helmet && `helmet` || `armor`}.png') no-repeat center / contain`}>
                            <img
                                class="icon-image-for-mask"
                                src={`./assets/equipment/${data.m_has_helmet && "helmet" || "armor"}.svg`}
                                alt=""
                            />
                        </div>
                        {data.m_armor}
                    </div>
                </div>
            </div>
            <div class="player-card-first-row">
                <div class="player-card-column">
                    <div class="player-card-money">$<span>{data.m_money}</span></div>
                    <div class="player-card-stats">
                        <div class="player-card-stat">
                            <div class="player-card-statname">K</div>
                            <div class="player-card-statvalue">
                                {
                                    data.m_stats.kills < 10 &&
                                    <span style={{ color: 'var(--text-secondary)' }}>0</span>
                                }
                                {`${data.m_stats.kills}`}
                            </div>
                        </div>
                        <div class="player-card-stat">
                            <div class="player-card-statname">D</div>
                            <div class="player-card-statvalue">
                                {
                                    data.m_stats.deaths < 10 &&
                                    <span style={{ color: 'var(--text-secondary)' }}>0</span>
                                }
                                {`${data.m_stats.deaths}`}
                            </div>
                        </div>
                        <div class="player-card-stat">
                            <div class="player-card-statname">A</div>
                            <div class="player-card-statvalue">
                                {
                                    data.m_stats.assists < 10 &&
                                    <span style={{ color: 'var(--text-secondary)' }}>0</span>
                                }
                                {`${data.m_stats.assists}`}
                            </div>
                        </div>
                    </div>
                </div>
                <div class="player-card-column player-card-column-right">
                    {
                        data.m_weapons && data.m_weapons.primary &&
                        <div class={`player-card-weapons-primary ${data.m_weapons.m_active == data.m_weapons.primary && "player-card-weapon-active"}`} style={`-webkit-mask: url("./assets/equipment/${data.m_weapons.primary}.svg") no-repeat center / contain`}>
                            <img
                                class="player-card-img-primary"
                                src={`./assets/equipment/${data.m_weapons.primary}.svg`}
                            />
                        </div>
                    }

                    {
                        data.m_weapons && data.m_weapons.secondary &&
                        <div class={`player-card-weapons-secondary ${data.m_weapons.m_active == data.m_weapons.secondary && "player-card-weapon-active"}`} style={`-webkit-mask: url("./assets/equipment/${data.m_weapons.secondary}.svg") no-repeat center / contain`}>
                            <img
                                class="player-card-img-secondary"
                                src={`./assets/equipment/${data.m_weapons.secondary}.svg`}
                            />
                        </div>
                    }

                    <div class="player-card-row-utilities">
                        {
                            data.m_weapons && data.m_weapons.utility &&
                            <For each={data.m_weapons.utility}>{(utility) =>
                                <div class={`player-card-weapons-utility ${data.m_weapons.m_active == utility && "player-card-weapon-active"}`} style={`-webkit-mask: url("./assets/equipment/${utility}.svg") no-repeat center / contain`}>
                                    <img
                                        class="player-card-img-utility"
                                        src={`./assets/equipment/${utility}.svg`}
                                    />
                                </div>
                            }</For>
                        }
                        {
                            data.m_has_defuser &&
                            <div class="player-card-weapons-utility" style={`-webkit-mask: url("./assets/equipment/defuser.svg") no-repeat center / contain`}>
                                <img
                                    class="player-card-img-utility"
                                    src={`./assets/equipment/defuser.svg`}
                                />
                            </div>
                        }
                    </div>
                </div>
            </div>
        </li>
    );
}