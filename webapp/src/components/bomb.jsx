import { memo } from "react";
import { useT } from "../i18n/i18n";
import {
  BOMB_FUSE_SECONDS,
  clamp,
  getRadarPosition,
  teamEnum,
} from "../utilities/utilities";

// Radar marker component

const Bomb = ({ bombData, mapData, localTeam, settings }) => {
  const { t } = useT();
  const position = getRadarPosition(mapData, bombData);
  if (position.x <= 0 && position.y <= 0) return null;

  const size = 2.2 * settings.bombSize; // % of the radar's width

  const color =
    (bombData.m_is_defused && "#43c97a") ||
    (localTeam === teamEnum.counterTerrorist && "#4e9bf5") ||
    "#ef5350";

  const isTicking = bombData.m_blow_time > 0 && !bombData.m_is_defused;

  return (
    <div
      // Percentages, no layout reads
      className="radar-entity absolute origin-center"
      style={{
        left: `${position.x * 100}%`,
        top: `${position.y * 100}%`,
        width: `${size}%`,
        aspectRatio: "1 / 1",
        transform: "translate(-50%, -50%)",
        zIndex: 3,
      }}
      title={t(bombData.m_is_defused ? "bomb.defused" : "bomb.c4")}
    >
      {isTicking && (
        <span
          className="animate-pulse-critical absolute inset-0 rounded-full"
          style={{ boxShadow: `0 0 12px 4px ${color}`, opacity: 0.45 }}
          aria-hidden="true"
        />
      )}
      <div
        className="relative h-full w-full"
        style={{
          backgroundColor: color,
          WebkitMask: `url('./assets/icons/c4_sml.png') no-repeat center / contain`,
          mask: `url('./assets/icons/c4_sml.png') no-repeat center / contain`,
        }}
      />
    </div>
  );
};

// Objective HUD component

// Objective state to descriptor
const resolveObjective = (bombData) => {
  if (bombData.m_is_defused) {
    return {
      labelKey: "bomb.defused",
      detailKey: "bomb.siteSecured",
      color: "var(--status-green)",
      progress: 0,
      critical: false,
    };
  }

  const blowTime = bombData.m_blow_time;
  if (!blowTime || blowTime <= 0) return null;

  if (bombData.m_is_defusing) {
    const willSucceed = blowTime - bombData.m_defuse_time > 0;
    return {
      labelKey: willSucceed ? "bomb.defusingInTime" : "bomb.defusingTooLate",
      detailKey: "bomb.defuseSeconds",
      detailValues: { seconds: bombData.m_defuse_time.toFixed(1) },
      color: willSucceed ? "var(--status-green)" : "var(--status-red)",
      progress: clamp((blowTime / BOMB_FUSE_SECONDS) * 100, 0, 100),
      critical: !willSucceed,
    };
  }

  const critical = blowTime <= 10;
  return {
    labelKey: "bomb.planted",
    detailKey: critical ? "bomb.critical" : "bomb.detonating",
    color: critical ? "var(--status-red)" : "var(--status-amber)",
    progress: clamp((blowTime / BOMB_FUSE_SECONDS) * 100, 0, 100),
    critical,
  };
};

// Bomb HUD top bar component
export const BombHud = ({ bombData }) => {
  const { t } = useT();
  if (!bombData) return null;

  const state = resolveObjective(bombData);
  if (!state) return null;

  const label = t(state.labelKey);
  const showTimer = !bombData.m_is_defused && bombData.m_blow_time > 0;

  return (
    <div
      className="animate-fade-in flex w-[min(92vw,360px)] flex-col gap-2 rounded-xl border bg-surface-2 px-3 py-2"
      style={{ borderColor: state.color }}
      role="status"
      aria-live="polite"
      aria-label={
        showTimer
          ? t("bomb.timerLabel", { label, seconds: bombData.m_blow_time.toFixed(0) })
          : label
      }
    >
      <div className="flex items-center gap-2.5">
        <div
          className={`h-5 w-5 shrink-0 ${state.critical ? "animate-pulse-critical" : ""}`}
          style={{
            backgroundColor: state.color,
            WebkitMask: `url('./assets/icons/c4_sml.png') no-repeat center / contain`,
            mask: `url('./assets/icons/c4_sml.png') no-repeat center / contain`,
          }}
          aria-hidden="true"
        />

        <div className="flex min-w-0 flex-1 flex-col leading-tight">
          <span
            className="truncate text-[11px] font-semibold uppercase tracking-wider"
            style={{ color: state.color }}
          >
            {label}
          </span>
          <span className="truncate text-[11px] text-content-muted">
            {t(state.detailKey, state.detailValues)}
          </span>
        </div>

        {showTimer && (
          <span className="tnum shrink-0 text-lg font-semibold" style={{ color: state.color }}>
            {bombData.m_blow_time.toFixed(1)}
            <span className="ml-0.5 text-[11px] text-content-muted">
              {t("bomb.secondsShort")}
            </span>
          </span>
        )}
      </div>

      {showTimer && (
        <div className="relative h-1 w-full overflow-hidden rounded-full bg-surface-3">
          {/* Width only, no reflow */}
          <span
            className="absolute inset-y-0 left-0 block rounded-full transition-[width] duration-150 ease-linear"
            style={{ width: `${state.progress}%`, backgroundColor: state.color }}
          />
          {/* Defuse overlaid for comparison */}
          {bombData.m_is_defusing && (
            <span
              className="absolute inset-y-0 left-0 block rounded-full border-r-2 border-white/90 bg-white/25"
              style={{
                width: `${clamp((bombData.m_defuse_time / BOMB_FUSE_SECONDS) * 100, 0, 100)}%`,
              }}
              aria-hidden="true"
            />
          )}
        </div>
      )}
    </div>
  );
};

export default memo(Bomb);
