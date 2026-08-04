import { memo, useEffect, useState } from "react";
import MaskedIcon from "./maskedicon";
import { useT } from "../i18n/i18n";
import {
  clamp,
  formatMoney,
  getTeamColor,
  playerColors,
  teamEnum,
} from "../utilities/utilities";

const HEALTH_STOPS = [
  { min: 60, color: "#43c97a" },
  { min: 30, color: "#eba53c" },
  { min: 0, color: "#ef5350" },
];

const healthColor = (health) =>
  (HEALTH_STOPS.find((stop) => health >= stop.min) || HEALTH_STOPS[2]).color;

// Weapon slot, active one accented
const WeaponSlot = ({ icon, name, isActive, size = 22 }) => (
  <div
    className={`flex h-[30px] items-center justify-center rounded-md border px-1.5 transition-colors duration-150 ${
      isActive ? "border-accent bg-accent-dim" : "border-transparent bg-surface-2"
    }`}
    title={name}
  >
    <MaskedIcon
      path={icon}
      height={size}
      color={isActive ? "bg-content-primary" : "bg-content-muted"}
      label={name}
    />
  </div>
);

const PlayerCard = ({ playerData, isOnRightSide = false, settings }) => {
  const { t, locale } = useT();
  const [modelName, setModelName] = useState(playerData.m_model_name);

  // Keep model, avoid flicker
  useEffect(() => {
    if (playerData.m_model_name) setModelName(playerData.m_model_name);
  }, [playerData.m_model_name]);

  const isDead = playerData.m_is_dead;
  if (isDead && !settings.keepDeadCards) return null;

  const weapons = playerData.m_weapons || {};
  const active = weapons.m_active;
  const utilities = weapons.m_utilities || [];
  const melee = weapons.m_melee || [];

  const health = clamp(playerData.m_health, 0, 100);
  const armor = clamp(playerData.m_armor, 0, 100);
  const teamColor = getTeamColor(playerData.m_team);
  const slotColor = playerColors[playerData.m_color] || teamColor;

  const hasObjectiveIcon =
    (playerData.m_team === teamEnum.counterTerrorist && playerData.m_has_defuser) ||
    (playerData.m_team === teamEnum.terrorist && playerData.m_has_bomb);

  return (
    <li
      className={`animate-card-in relative overflow-hidden rounded-xl border bg-surface-1 transition-opacity duration-200 ${
        isDead ? "border-line opacity-40 saturate-0" : "border-line-strong opacity-100"
      }`}
    >
      {/* Team rail, mirrors right */}
      <span
        aria-hidden="true"
        className={`absolute inset-y-0 w-[3px] ${isOnRightSide ? "right-0" : "left-0"}`}
        style={{ backgroundColor: isDead ? "var(--border-strong)" : teamColor }}
      />

      <div
        className={`flex items-stretch gap-2.5 py-2.5 ${
          isOnRightSide ? "flex-row-reverse pl-2.5 pr-4" : "pl-4 pr-2.5"
        }`}
      >
        {/* Portrait */}
        <div className="relative flex w-[52px] shrink-0 flex-col items-center justify-end">
          <img
            className={`h-[60px] w-auto object-contain object-bottom ${
              isOnRightSide ? "scale-x-[-1]" : ""
            }`}
            src={`./assets/characters/${modelName}.png`}
            alt=""
            draggable={false}
          />
          {/* Slot colour chip */}
          <span
            aria-hidden="true"
            className="absolute bottom-0 h-1.5 w-7 rounded-full"
            style={{ backgroundColor: slotColor }}
          />
          {isDead && (
            <span className="absolute inset-x-0 top-0 text-center text-[9px] font-bold uppercase tracking-widest text-status-red">
              {t("card.dead")}
            </span>
          )}
        </div>

        {/* Stats */}
        <div className={`flex min-w-0 flex-1 flex-col gap-2 ${isOnRightSide ? "items-end" : ""}`}>
          {/* name + money */}
          <div
            className={`flex w-full items-baseline gap-2 ${isOnRightSide ? "flex-row-reverse" : ""}`}
          >
            {settings.showSteamLink ? (
              <button
                type="button"
                onClick={() =>
                  window.open(
                    `https://steamcommunity.com/profiles/${playerData.m_steam_id}`,
                    "_blank",
                    "noopener,noreferrer"
                  )
                }
                title={t("card.openSteam", { name: playerData.m_name })}
                className="min-w-0 truncate rounded text-[13px] font-semibold text-content-primary underline-offset-4 transition-colors duration-150 hover:cursor-pointer hover:text-accent hover:underline"
              >
                {playerData.m_name}
              </button>
            ) : (
              <span className="min-w-0 truncate text-[13px] font-semibold text-content-primary">
                {playerData.m_name}
              </span>
            )}

            {settings.showMoney && (
              <span className="tnum shrink-0 text-[11px] font-semibold text-status-green">
                {formatMoney(playerData.m_money, locale)}
              </span>
            )}
          </div>

          {/* health bar + armor */}
          <div
            className={`flex w-full items-center gap-2 ${isOnRightSide ? "flex-row-reverse" : ""}`}
          >
            <div
              className="relative h-1.5 min-w-0 flex-1 overflow-hidden rounded-full bg-surface-3"
              role="meter"
              aria-valuenow={health}
              aria-valuemin={0}
              aria-valuemax={100}
              aria-label={t("card.health", { name: playerData.m_name })}
            >
              <span
                className={`absolute inset-y-0 block rounded-full transition-[width] duration-200 ease-out ${
                  isOnRightSide ? "right-0" : "left-0"
                }`}
                style={{ width: `${health}%`, backgroundColor: healthColor(health) }}
              />
            </div>

            <span className="tnum w-7 shrink-0 text-right text-[11px] font-semibold text-content-primary">
              {health}
            </span>

            {settings.showArmor && armor > 0 && (
              <span
                className={`flex shrink-0 items-center gap-1 ${
                  isOnRightSide ? "flex-row-reverse" : ""
                }`}
              >
                <MaskedIcon
                  path={`./assets/icons/${
                    playerData.m_has_helmet ? "kevlar_helmet" : "kevlar"
                  }.svg`}
                  height={13}
                  color="bg-accent"
                  label={t(playerData.m_has_helmet ? "card.kevlarHelmet" : "card.kevlar")}
                />
                <span className="tnum text-[11px] text-content-secondary">{armor}</span>
              </span>
            )}
          </div>

          {/* weapons + utilities + objective */}
          {settings.showWeapons && (
            <div
              className={`flex w-full flex-wrap items-center gap-1 ${
                isOnRightSide ? "flex-row-reverse" : ""
              }`}
            >
              {weapons.m_primary && (
                <WeaponSlot
                  icon={`./assets/icons/${weapons.m_primary}.svg`}
                  name={weapons.m_primary}
                  isActive={active === weapons.m_primary}
                  size={24}
                />
              )}

              {weapons.m_secondary && (
                <WeaponSlot
                  icon={`./assets/icons/${weapons.m_secondary}.svg`}
                  name={weapons.m_secondary}
                  isActive={active === weapons.m_secondary}
                  size={20}
                />
              )}

              {melee.map((item) => (
                <WeaponSlot
                  key={item}
                  icon={`./assets/icons/${item}.svg`}
                  name={item}
                  isActive={active === item}
                  size={17}
                />
              ))}

              {utilities.map((utility) => (
                <WeaponSlot
                  key={utility}
                  icon={`./assets/icons/${utility}.svg`}
                  name={utility}
                  isActive={active === utility}
                  size={17}
                />
              ))}

              {hasObjectiveIcon && (
                <span
                  className="ml-auto flex h-[30px] items-center rounded-md border px-1.5"
                  style={{ borderColor: teamColor, backgroundColor: "var(--surface-2)" }}
                >
                  {playerData.m_team === teamEnum.counterTerrorist ? (
                    <MaskedIcon
                      path="./assets/icons/defuser.svg"
                      height={17}
                      color="bg-team-ct"
                      label={t("card.hasDefuser")}
                    />
                  ) : (
                    <MaskedIcon
                      path="./assets/icons/c4.svg"
                      height={17}
                      color={active === "c4" ? "bg-content-primary" : "bg-team-t"}
                      label={t("card.carryingBomb")}
                    />
                  )}
                </span>
              )}
            </div>
          )}
        </div>
      </div>
    </li>
  );
};

export default memo(PlayerCard);
