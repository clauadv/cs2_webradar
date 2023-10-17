export const PlayerCard = (props) => {
    const { data } = props;

    return (
        <li className="player-card">
            <div className="player-card-first-row">
                <div className="player-card-name">{data.m_name}</div>
                <div className="player-card-vitals">
                    <div className="player-card-vital">
                        <div className="icon">
                            <img
                                className="icon-image"
                                src="./assets/icons/health.svg"
                                alt=""
                            />
                        </div>
                        {data.m_health}
                    </div>
                    <div className="player-card-vital">
                        <div className="masked-icon" style={{ WebkitMask: `url('./assets/equipment/${data.m_has_helmet && `helmet` || `armor`}.png') no-repeat center / contain` }}>
                            <img
                                className="icon-image-for-mask"
                                src={`./assets/equipment/${data.m_has_helmet && "helmet" || "armor"}.svg`}
                                alt=""
                            />
                        </div>
                        {data.m_armor}
                    </div>
                </div>
            </div>
            <div className="player-card-first-row">
                <div className="player-card-column">
                    <div className="player-card-money">$<span>{data.m_money}</span></div>
                    <div className="player-card-stats">
                        <div className="player-card-stat">
                            <div className="player-card-statname">K</div>
                            <div className="player-card-statvalue">
                                {
                                    data.m_stats.kills < 10 &&
                                    <span style={{ color: 'var(--text-secondary)' }}>0</span>
                                }
                                {`${data.m_stats.kills}`}
                            </div>
                        </div>
                        <div className="player-card-stat">
                            <div className="player-card-statname">D</div>
                            <div className="player-card-statvalue">
                                {
                                    data.m_stats.deaths < 10 &&
                                    <span style={{ color: 'var(--text-secondary)' }}>0</span>
                                }
                                {`${data.m_stats.deaths}`}
                            </div>
                        </div>
                        <div className="player-card-stat">
                            <div className="player-card-statname">A</div>
                            <div className="player-card-statvalue">
                                {
                                    data.m_stats.assists < 10 &&
                                    <span style={{ color: 'var(--text-secondary)' }}>0</span>
                                }
                                {`${data.m_stats.assists}`}
                            </div>
                        </div>
                    </div>
                </div>
                <div className="player-card-column player-card-column-right">
                    {
                        data.m_weapons && data.m_weapons.primary &&
                        <div className={`player-card-weapons-primary ${data.m_weapons.m_active == data.m_weapons.primary && "player-card-weapon-active"}`} style={{ WebkitMask: `url("./assets/equipment/${data.m_weapons.primary}.svg") no-repeat center / contain` }}>
                            <img
                                className="player-card-img-primary"
                                src={`./assets/equipment/${data.m_weapons.primary}.svg`}
                            />
                        </div>
                    }

                    {
                        data.m_weapons && data.m_weapons.secondary &&
                        <div className={`player-card-weapons-secondary ${data.m_weapons.m_active == data.m_weapons.secondary && "player-card-weapon-active"}`} style={{ WebkitMask: `url("./assets/equipment/${data.m_weapons.secondary}.svg") no-repeat center / contain` }}>
                            <img
                                className="player-card-img-secondary"
                                src={`./assets/equipment/${data.m_weapons.secondary}.svg`}
                            />
                        </div>
                    }

                    <div className="player-card-row-utilities">
                        {
                            data.m_weapons && data.m_weapons.utility &&
                            data.m_weapons.utility.map((utility) => {
                                <div className={`player-card-weapons-utility ${data.m_weapons.m_active == utility && "player-card-weapon-active"}`} style={{ WebkitMask: `url("./assets/equipment/${utility}.svg") no-repeat center / contain` }}>
                                    <img
                                        className="player-card-img-utility"
                                        src={`./assets/equipment/${utility}.svg`}
                                    />
                                </div>
                            })
                        }
                        {
                            data.m_has_defuser &&
                            <div className="player-card-weapons-utility" style={{ WebkitMask: `url("./assets/equipment/defuser.svg") no-repeat center / contain` }}>
                                <img
                                    className="player-card-img-utility"
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