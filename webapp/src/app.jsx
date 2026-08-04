import { useEffect, useMemo, useRef, useState } from "react";
import "./app.css";
import PlayerCard from "./components/playercard";
import Radar from "./components/radar";
import SettingsButton from "./components/settings";
import { BombHud } from "./components/bomb";
import { LanguageProvider, detectLanguage, useT } from "./i18n/i18n";
import {
  CONNECTION_KEYS,
  loadSettings,
  readSocketPayload,
  resolveWebSocketURL,
  saveSettings,
  teamEnum,
} from "./utilities/utilities";

const CONNECTION_TIMEOUT = 5000;
const RECONNECT_DELAY = 2500;

const STATUS_COLORS = {
  connecting: "var(--status-amber)",
  waiting: "var(--status-green)",
  live: "var(--status-green)",
  disconnected: "var(--status-red)",
  error: "var(--status-red)",
};

const TEAM_META = {
  [teamEnum.terrorist]: { id: "terrorist", key: "teams.t", color: "var(--team-t)" },
  [teamEnum.counterTerrorist]: { id: "counterTerrorist", key: "teams.ct", color: "var(--team-ct)" },
};

// Team column component

const TeamColumn = ({ team, players, isOnRightSide, settings }) => {
  const { t } = useT();
  const meta = TEAM_META[team];
  const alive = players.filter((player) => !player.m_is_dead).length;

  return (
    <div className="flex w-full min-w-0 flex-col gap-2 lg:w-[310px] xl:w-[340px]">
      <div className={`flex items-center gap-2 px-1 ${isOnRightSide ? "flex-row-reverse" : ""}`}>
        <span
          className="h-2.5 w-2.5 rounded-full"
          style={{ backgroundColor: meta.color }}
          aria-hidden="true"
        />
        <h2 className="text-[11px] font-semibold uppercase tracking-[0.14em] text-content-secondary">
          {t(meta.key)}
        </h2>
        <span className="tnum text-[11px] text-content-muted">
          {t("teams.aliveOf", { alive, total: players.length })}
        </span>
      </div>

      <ul id={meta.id} className="m-0 flex list-none flex-col gap-2 p-0">
        {players.map((player) => (
          <PlayerCard
            key={player.m_idx}
            playerData={player}
            isOnRightSide={isOnRightSide}
            settings={settings}
          />
        ))}
      </ul>
    </div>
  );
};

// Dashboard shell

const Dashboard = ({
  settings,
  setSettings,
  playerArray,
  mapData,
  localTeam,
  bombData,
  status,
  statusDetail,
}) => {
  const { t } = useT();

  const terrorists = playerArray.filter((player) => player.m_team === teamEnum.terrorist);
  const counterTerrorists = playerArray.filter(
    (player) => player.m_team === teamEnum.counterTerrorist
  );

  const hasRadar = playerArray.length > 0 && mapData;
  const statusColor = STATUS_COLORS[status] || STATUS_COLORS.connecting;

  // Detail is key or URL
  const detailText =
    (statusDetail?.key && t(statusDetail.key, statusDetail.values)) || statusDetail?.text || "";

  return (
    <div className="flex min-h-[100dvh] w-full flex-col bg-base">
      {/* Top bar */}
      <header
        className="sticky top-0 flex items-center justify-between gap-3 border-b border-line bg-surface-1 px-3 py-2.5"
        style={{ zIndex: "var(--z-hud)" }}
      >
        <div className="flex min-w-0 items-center gap-2.5">
          <span
            className={`h-2 w-2 shrink-0 rounded-full ${
              status === "connecting" ? "animate-pulse-critical" : ""
            }`}
            style={{ backgroundColor: statusColor }}
            aria-hidden="true"
          />
          <div className="flex min-w-0 flex-col leading-tight">
            <span
              className="text-[11px] font-semibold uppercase tracking-[0.14em]"
              style={{ color: statusColor }}
            >
              {t(`status.${status}`)}
            </span>
            <span className="truncate text-[11px] text-content-muted">
              {(mapData && `${t("app.mapLabel")} · ${mapData.name}`) || detailText}
            </span>
          </div>
        </div>

        <div className="hidden flex-1 justify-center md:flex">
          <BombHud bombData={bombData} />
        </div>

        <SettingsButton
          settings={settings}
          onSettingsChange={setSettings}
          connectionState={{
            label: t(`status.${status}`),
            detail: detailText,
            color: statusColor,
          }}
        />
      </header>

      {/* Stacked HUD on narrow screens */}
      {bombData && (
        <div className="flex justify-center px-3 pt-2 md:hidden">
          <BombHud bombData={bombData} />
        </div>
      )}

      {/* Main */}
      <main className="flex w-full flex-1 items-start justify-center px-3 py-3">
        <div className="flex w-full max-w-[1600px] flex-col items-center gap-4 lg:flex-row lg:items-start lg:justify-center lg:gap-5">
          <TeamColumn
            team={teamEnum.terrorist}
            players={terrorists}
            isOnRightSide={false}
            settings={settings}
          />

          <div className="order-first w-full max-w-[min(92vw,720px)] shrink-0 lg:order-none lg:max-w-none lg:flex-1 lg:basis-[520px]">
            {hasRadar ? (
              <Radar
                playerArray={playerArray}
                radarImage={`./data/${mapData.name}/radar.png`}
                mapData={mapData}
                localTeam={localTeam}
                bombData={bombData}
                settings={settings}
              />
            ) : (
              <div
                id="radar"
                className="panel flex aspect-square w-full flex-col items-center justify-center gap-4 rounded-2xl px-6 text-center"
              >
                <span
                  className="animate-spin-slow h-9 w-9 rounded-full border-2 border-line-strong"
                  style={{ borderTopColor: "var(--accent)" }}
                  aria-hidden="true"
                />
                <p
                  className="radar_message max-w-[38ch] text-[13px] text-content-secondary"
                  aria-live="polite"
                >
                  {status === "error" ? detailText || t("app.connectionFailed") : t("app.waitingData")}
                </p>
                {status === "error" && (
                  <p className="text-[11px] text-content-muted">{t("app.openSettings")}</p>
                )}
              </div>
            )}
          </div>

          <TeamColumn
            team={teamEnum.counterTerrorist}
            players={counterTerrorists}
            isOnRightSide={true}
            settings={settings}
          />
        </div>
      </main>
    </div>
  );
};

// Root App component

const App = () => {
  const [playerArray, setPlayerArray] = useState([]);
  const [mapData, setMapData] = useState(null);
  const [localTeam, setLocalTeam] = useState(teamEnum.none);
  const [bombData, setBombData] = useState(null);
  const [settings, setSettings] = useState(() => loadSettings(detectLanguage()));
  const [status, setStatus] = useState("connecting");
  const [statusDetail, setStatusDetail] = useState(null);

  useEffect(() => {
    saveSettings(settings);
  }, [settings]);

  // Sync document language
  useEffect(() => {
    document.documentElement.lang = settings.language;
  }, [settings.language]);

  // Only connection fields reconnect socket
  const connectionKey = useMemo(
    () => CONNECTION_KEYS.map((key) => String(settings[key])).join("|"),
    // eslint-disable-next-line react-hooks/exhaustive-deps
    CONNECTION_KEYS.map((key) => settings[key])
  );

  const settingsRef = useRef(settings);
  settingsRef.current = settings;

  // Map already requested
  const requestedMapRef = useRef(null);

  useEffect(() => {
    const { url, errorKey, errorValues } = resolveWebSocketURL(settingsRef.current);

    if (!url) {
      setStatus("error");
      setStatusDetail({ key: errorKey, values: errorValues });
      return undefined;
    }

    let socket = null;
    let connectionTimeout = null;
    let reconnectTimeout = null;
    let cancelled = false;

    // New endpoint may change map
    requestedMapRef.current = null;

    const connect = () => {
      if (cancelled) return;

      setStatus("connecting");
      setStatusDetail({ text: url });

      try {
        socket = new WebSocket(url);
      } catch (caught) {
        setStatus("error");
        setStatusDetail({ text: `${caught}` });
        return;
      }

      connectionTimeout = setTimeout(() => {
        if (socket && socket.readyState !== WebSocket.OPEN) socket.close();
      }, CONNECTION_TIMEOUT);

      socket.onopen = () => {
        clearTimeout(connectionTimeout);
        if (cancelled) return;
        setStatus("waiting");
        setStatusDetail({ text: url });
        console.info("connected to the web socket");
      };

      socket.onmessage = async (event) => {
        try {
          const parsed = await readSocketPayload(event.data);
          if (cancelled) return;

          setStatus("live");
          setPlayerArray(parsed.m_players || []);
          setLocalTeam(parsed.m_local_team);
          setBombData(parsed.m_bomb || null);

          const map = parsed.m_map;
          if (map && map !== "invalid" && requestedMapRef.current !== map) {
            // Static per map, fetch once
            requestedMapRef.current = map;
            try {
              const response = await fetch(`./data/${map}/data.json`);
              const data = await response.json();
              if (!cancelled) setMapData({ ...data, name: map });
            } catch (caught) {
              requestedMapRef.current = null;
              console.error(`failed to load data for '${map}'`, caught);
            }
          }
        } catch (caught) {
          console.error("failed to parse websocket payload", caught);
        }
      };

      socket.onerror = (caught) => {
        clearTimeout(connectionTimeout);
        console.error(caught);
        if (cancelled) return;
        setStatus("error");
        setStatusDetail({ key: "connection.unreachable", values: { url } });
      };

      socket.onclose = () => {
        clearTimeout(connectionTimeout);
        if (cancelled) return;
        setStatus((previous) => (previous === "error" ? "error" : "disconnected"));
        setStatusDetail((previous) =>
          previous?.key === "connection.unreachable" ? previous : { key: "status.retrying" }
        );
        console.warn("disconnected from the web socket");
        reconnectTimeout = setTimeout(connect, RECONNECT_DELAY);
      };
    };

    connect();

    // Detach before close, avoid stray reconnect
    return () => {
      cancelled = true;
      clearTimeout(connectionTimeout);
      clearTimeout(reconnectTimeout);

      if (socket) {
        socket.onopen = null;
        socket.onmessage = null;
        socket.onerror = null;
        socket.onclose = null;
        if (socket.readyState === WebSocket.OPEN || socket.readyState === WebSocket.CONNECTING) {
          socket.close();
        }
      }
    };
  }, [connectionKey]);

  return (
    <LanguageProvider language={settings.language}>
      <Dashboard
        settings={settings}
        setSettings={setSettings}
        playerArray={playerArray}
        mapData={mapData}
        localTeam={localTeam}
        bombData={bombData}
        status={status}
        statusDetail={statusDetail}
      />
    </LanguageProvider>
  );
};

export default App;
