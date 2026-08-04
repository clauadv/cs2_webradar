import { useCallback, useEffect, useId, useRef, useState } from "react";
import { createPortal } from "react-dom";
import { DEFAULT_SETTINGS, trackFill } from "../utilities/utilities";
import { LANGUAGES, useT } from "../i18n/i18n";

// Tab & UI Icons

const iconProps = {
  viewBox: "0 0 24 24",
  fill: "none",
  stroke: "currentColor",
  strokeWidth: 1.7,
  strokeLinecap: "round",
  strokeLinejoin: "round",
};

const GlobeIcon = (props) => (
  <svg {...iconProps} {...props}>
    <circle cx="12" cy="12" r="9" />
    <path d="M3 12h18M12 3c2.4 2.6 2.4 15.4 0 18M12 3c-2.4 2.6-2.4 15.4 0 18" />
  </svg>
);

const TargetIcon = (props) => (
  <svg {...iconProps} {...props}>
    <circle cx="12" cy="12" r="9" />
    <circle cx="12" cy="12" r="4.5" />
    <circle cx="12" cy="12" r="1.2" fill="currentColor" stroke="none" />
  </svg>
);

const CardsIcon = (props) => (
  <svg {...iconProps} {...props}>
    <rect x="3" y="6" width="12" height="15" rx="2" />
    <path d="M8 3h9a2 2 0 0 1 2 2v12" />
  </svg>
);

const CloseIcon = (props) => (
  <svg {...iconProps} {...props}>
    <path d="M6 6l12 12M18 6L6 18" />
  </svg>
);

const CheckIcon = (props) => (
  <svg {...iconProps} strokeWidth={2.6} {...props}>
    <path d="M5 13l4 4L19 7" />
  </svg>
);

const TABS = [
  { id: "connection", key: "tabs.connection", Icon: GlobeIcon },
  { id: "radar", key: "tabs.radar", Icon: TargetIcon },
  { id: "cards", key: "tabs.cards", Icon: CardsIcon },
];

// Input & Control Primitives

// Row is hit target
const Toggle = ({ label, hint, checked, onChange }) => (
  <label className="group flex min-h-[52px] cursor-pointer items-center justify-between gap-4 rounded-lg px-3 transition-colors duration-150 hover:bg-surface-2">
    <span className="flex min-w-0 flex-col gap-0.5">
      <span className="text-[13px] font-medium text-content-primary">{label}</span>
      {hint && <span className="text-[11px] leading-snug text-content-muted">{hint}</span>}
    </span>

    <span className="relative shrink-0">
      <input
        type="checkbox"
        className="peer absolute inset-0 h-full w-full cursor-pointer opacity-0"
        checked={checked}
        onChange={(event) => onChange(event.target.checked)}
      />
      <span
        aria-hidden="true"
        className="block h-[26px] w-[46px] rounded-full border border-line-strong bg-surface-3 transition-colors duration-200 ease-out-quint peer-checked:border-accent peer-checked:bg-accent peer-focus-visible:outline peer-focus-visible:outline-2 peer-focus-visible:outline-offset-2 peer-focus-visible:outline-accent"
      />
      <span
        aria-hidden="true"
        className="pointer-events-none absolute left-[4px] top-[4px] flex h-[18px] w-[18px] items-center justify-center rounded-full bg-content-secondary shadow-sm transition-all duration-200 ease-out-quint peer-checked:translate-x-[20px] peer-checked:bg-white"
      >
        <CheckIcon className="h-2.5 w-2.5 text-accent opacity-0 transition-opacity duration-150 peer-checked:opacity-100" />
      </span>
    </span>
  </label>
);

const Slider = ({ label, value, min, max, step, onChange }) => {
  const id = useId();
  const fill = trackFill(value, min, max);

  return (
    <div className="flex flex-col gap-2.5 px-3 py-3">
      <div className="flex items-center justify-between gap-2">
        <label className="text-[13px] font-medium text-content-primary" htmlFor={id}>
          {label}
        </label>
        <span className="tnum rounded-md bg-surface-3 px-2 py-0.5 text-[11px] font-semibold text-accent">
          {value.toFixed(1)}×
        </span>
      </div>
      <input
        id={id}
        type="range"
        className="range"
        min={min}
        max={max}
        step={step}
        value={value}
        onChange={(event) => onChange(parseFloat(event.target.value))}
        style={{
          background: `linear-gradient(to right, var(--accent) ${fill}%, var(--surface-3) ${fill}%)`,
        }}
      />
      <div className="tnum flex justify-between text-[10px] text-content-muted">
        <span>{min.toFixed(1)}×</span>
        <span>{max.toFixed(1)}×</span>
      </div>
    </div>
  );
};

const Field = ({ label, hint, ...inputProps }) => (
  <div className="flex flex-col gap-1.5 px-3 py-2">
    <label className="text-[13px] font-medium text-content-primary" htmlFor={inputProps.id}>
      {label}
    </label>
    <input
      {...inputProps}
      className="h-11 rounded-lg border border-line-strong bg-sunken px-3 text-[13px] text-content-primary transition-colors duration-150 placeholder:text-content-muted focus:border-accent focus:outline-none focus:ring-2 focus:ring-accent/35 disabled:cursor-not-allowed disabled:opacity-40"
    />
    {hint && <span className="text-[11px] leading-snug text-content-muted">{hint}</span>}
  </div>
);

// Segmented control for language
const Segmented = ({ label, hint, options, value, onChange }) => (
  <div className="flex min-h-[52px] items-center justify-between gap-4 px-3 py-2">
    <span className="flex min-w-0 flex-col gap-0.5">
      <span className="text-[13px] font-medium text-content-primary">{label}</span>
      {hint && <span className="text-[11px] leading-snug text-content-muted">{hint}</span>}
    </span>

    <div
      role="radiogroup"
      aria-label={label}
      className="flex shrink-0 gap-1 rounded-lg border border-line-strong bg-sunken p-1"
    >
      {options.map((option) => {
        const isActive = option.id === value;
        return (
          <button
            key={option.id}
            type="button"
            role="radio"
            aria-checked={isActive}
            title={option.name}
            onClick={() => onChange(option.id)}
            className={`min-w-[46px] rounded-md px-2.5 py-1.5 text-xs font-semibold transition-colors duration-150 hover:cursor-pointer ${isActive
                ? "bg-accent text-white"
                : "text-content-muted hover:bg-surface-2 hover:text-content-secondary"
              }`}
          >
            {option.short}
          </button>
        );
      })}
    </div>
  </div>
);

const Divider = () => <hr className="my-1 border-line" />;

// Settings Modal Dialog

const FOCUSABLE =
  'button:not([disabled]), input:not([disabled]), [href], select, textarea, [tabindex]:not([tabindex="-1"])';

const SettingsButton = ({ settings, onSettingsChange, connectionState }) => {
  const { t } = useT();
  const [isOpen, setIsOpen] = useState(false);
  const [activeTab, setActiveTab] = useState("connection");
  const triggerRef = useRef(null);
  const dialogRef = useRef(null);

  const update = useCallback(
    (patch) => onSettingsChange({ ...settings, ...patch }),
    [onSettingsChange, settings]
  );

  const close = useCallback(() => {
    setIsOpen(false);
    triggerRef.current?.focus();
  }, []);

  useEffect(() => {
    if (!isOpen) return undefined;

    const onKeyDown = (event) => {
      if (event.key === "Escape") {
        close();
        return;
      }

      if (event.key !== "Tab") return;

      // Trap Tab inside dialog
      const nodes = dialogRef.current?.querySelectorAll(FOCUSABLE);
      if (!nodes || nodes.length === 0) return;

      const first = nodes[0];
      const last = nodes[nodes.length - 1];

      if (event.shiftKey && document.activeElement === first) {
        event.preventDefault();
        last.focus();
      } else if (!event.shiftKey && document.activeElement === last) {
        event.preventDefault();
        first.focus();
      }
    };

    document.addEventListener("keydown", onKeyDown);
    dialogRef.current?.focus();

    // Lock background scroll
    const previousOverflow = document.body.style.overflow;
    document.body.style.overflow = "hidden";

    return () => {
      document.removeEventListener("keydown", onKeyDown);
      document.body.style.overflow = previousOverflow;
    };
  }, [isOpen, close]);

  // Arrow keys move between tabs
  const onTabKeyDown = (event) => {
    const direction = (event.key === "ArrowRight" && 1) || (event.key === "ArrowLeft" && -1) || 0;
    if (!direction) return;

    event.preventDefault();
    const index = TABS.findIndex((tab) => tab.id === activeTab);
    setActiveTab(TABS[(index + direction + TABS.length) % TABS.length].id);
  };

  const modal = (
    <div
      className="animate-fade-in scrim fixed inset-0 flex items-center justify-center p-4"
      style={{ zIndex: "var(--z-modal)" }}
      onClick={close}
    >
      <div
        ref={dialogRef}
        role="dialog"
        aria-modal="true"
        aria-label={t("settings.title")}
        tabIndex={-1}
        // Stop clicks reaching scrim
        onClick={(event) => event.stopPropagation()}
        className="animate-modal-in modal-surface flex max-h-[calc(100dvh-2rem)] w-full max-w-[520px] flex-col overflow-hidden rounded-2xl focus:outline-none"
      >
        {/* Header */}
        <header className="flex items-start justify-between gap-3 px-5 pb-4 pt-5">
          <div className="flex flex-col gap-0.5">
            <h2 className="text-[17px] font-semibold leading-tight text-content-primary">
              {t("settings.title")}
            </h2>
            <span className="text-[11px] text-content-muted">{t("settings.subtitle")}</span>
          </div>
          <button
            type="button"
            onClick={close}
            aria-label={t("settings.closeAria")}
            className="-mr-1.5 -mt-1.5 flex h-10 w-10 shrink-0 items-center justify-center rounded-lg text-content-muted transition-colors duration-150 hover:cursor-pointer hover:bg-surface-2 hover:text-content-primary"
          >
            <CloseIcon className="h-[18px] w-[18px]" aria-hidden="true" />
          </button>
        </header>

        {/* Tabs */}
        <div
          role="tablist"
          aria-label={t("settings.sections")}
          onKeyDown={onTabKeyDown}
          className="mx-5 mb-1 flex gap-1 rounded-xl border border-line bg-sunken p-1"
        >
          {TABS.map(({ id, key, Icon }) => {
            const isActive = activeTab === id;
            return (
              <button
                key={id}
                type="button"
                role="tab"
                id={`tab-${id}`}
                aria-selected={isActive}
                aria-controls={`panel-${id}`}
                tabIndex={isActive ? 0 : -1}
                onClick={() => setActiveTab(id)}
                className={`flex min-h-[40px] flex-1 items-center justify-center gap-1.5 rounded-lg px-2 text-xs font-semibold transition-colors duration-150 ease-out-quint hover:cursor-pointer ${isActive
                    ? "bg-accent text-white shadow-sm"
                    : "text-content-muted hover:bg-surface-2 hover:text-content-secondary"
                  }`}
              >
                <Icon className="h-[15px] w-[15px] shrink-0" aria-hidden="true" />
                <span className="truncate">{t(key)}</span>
              </button>
            );
          })}
        </div>

        {/* Panels */}
        <div className="thin-scroll min-h-0 flex-1 overflow-y-auto px-2 py-2">
          {activeTab === "connection" && (
            <div
              role="tabpanel"
              id="panel-connection"
              aria-labelledby="tab-connection"
              className="animate-tab-in flex flex-col"
            >
              {connectionState && (
                <div className="mx-3 mb-1 mt-1 flex items-center gap-2.5 rounded-lg border border-line bg-sunken px-3 py-2.5">
                  <span
                    className="h-2 w-2 shrink-0 rounded-full"
                    style={{
                      backgroundColor: connectionState.color,
                      boxShadow: `0 0 8px -1px ${connectionState.color}`,
                    }}
                    aria-hidden="true"
                  />
                  <div className="flex min-w-0 flex-1 flex-col leading-tight" aria-live="polite">
                    <span
                      className="text-[11px] font-semibold uppercase tracking-wider"
                      style={{ color: connectionState.color }}
                    >
                      {connectionState.label}
                    </span>
                    {connectionState.detail && (
                      <span className="truncate text-[11px] text-content-muted">
                        {connectionState.detail}
                      </span>
                    )}
                  </div>
                </div>
              )}

              <Toggle
                label={t("connection.useLocalhost")}
                hint={t("connection.useLocalhostHint")}
                checked={settings.useLocalhost}
                onChange={(checked) => update({ useLocalhost: checked })}
              />

              <Field
                id="settings-ip"
                label={t("connection.targetIp")}
                hint={
                  settings.useLocalhost
                    ? t("connection.targetIpDisabled")
                    : t("connection.targetIpHint")
                }
                type="text"
                inputMode="url"
                autoComplete="off"
                spellCheck={false}
                placeholder={t("connection.targetIpPlaceholder")}
                disabled={settings.useLocalhost}
                value={settings.targetIp}
                onChange={(event) => update({ targetIp: event.target.value })}
              />

              <Field
                id="settings-port"
                label={t("connection.port")}
                hint={t("connection.portHint")}
                type="number"
                inputMode="numeric"
                min={1}
                max={65535}
                value={settings.port}
                onChange={(event) => update({ port: event.target.value })}
              />

              <Divider />

              <Segmented
                label={t("lang.label")}
                hint={t("lang.hint")}
                options={LANGUAGES}
                value={settings.language}
                onChange={(id) => update({ language: id })}
              />
            </div>
          )}

          {activeTab === "radar" && (
            <div
              role="tabpanel"
              id="panel-radar"
              aria-labelledby="tab-radar"
              className="animate-tab-in flex flex-col"
            >
              <Slider
                label={t("radar.playerSize")}
                value={settings.dotSize}
                min={0.6}
                max={2}
                step={0.1}
                onChange={(value) => update({ dotSize: value })}
              />
              <Divider />
              <Slider
                label={t("radar.bombSize")}
                value={settings.bombSize}
                min={0.5}
                max={2}
                step={0.1}
                onChange={(value) => update({ bombSize: value })}
              />
              <Divider />
              <Toggle
                label={t("radar.showDead")}
                hint={t("radar.showDeadHint")}
                checked={settings.showDeadOnRadar}
                onChange={(checked) => update({ showDeadOnRadar: checked })}
              />
              <Toggle
                label={t("radar.showLabels")}
                hint={t("radar.showLabelsHint")}
                checked={settings.showRadarLabels}
                onChange={(checked) => update({ showRadarLabels: checked })}
              />
            </div>
          )}

          {activeTab === "cards" && (
            <div
              role="tabpanel"
              id="panel-cards"
              aria-labelledby="tab-cards"
              className="animate-tab-in flex flex-col"
            >
              <Toggle
                label={t("cards.keepDead")}
                hint={t("cards.keepDeadHint")}
                checked={settings.keepDeadCards}
                onChange={(checked) => update({ keepDeadCards: checked })}
              />
              <Divider />
              <Toggle
                label={t("cards.money")}
                checked={settings.showMoney}
                onChange={(checked) => update({ showMoney: checked })}
              />
              <Toggle
                label={t("cards.armor")}
                checked={settings.showArmor}
                onChange={(checked) => update({ showArmor: checked })}
              />
              <Toggle
                label={t("cards.weapons")}
                checked={settings.showWeapons}
                onChange={(checked) => update({ showWeapons: checked })}
              />
              <Toggle
                label={t("cards.steam")}
                hint={t("cards.steamHint")}
                checked={settings.showSteamLink}
                onChange={(checked) => update({ showSteamLink: checked })}
              />
            </div>
          )}
        </div>

        {/* Footer */}
        <footer className="flex items-center justify-between gap-3 border-t border-line px-5 py-3.5">
          <button
            type="button"
            onClick={() => onSettingsChange({ ...DEFAULT_SETTINGS, language: settings.language })}
            className="h-10 rounded-lg px-3 text-[13px] text-content-muted transition-colors duration-150 hover:cursor-pointer hover:bg-surface-2 hover:text-content-secondary"
          >
            {t("settings.reset")}
          </button>
          <button
            type="button"
            onClick={close}
            className="h-10 rounded-lg bg-accent px-6 text-[13px] font-semibold text-white transition-colors duration-150 hover:cursor-pointer hover:bg-accent-strong"
          >
            {t("settings.close")}
          </button>
        </footer>
      </div>
    </div>
  );

  return (
    <>
      <button
        ref={triggerRef}
        type="button"
        onClick={() => setIsOpen(true)}
        aria-expanded={isOpen}
        aria-haspopup="dialog"
        className="flex h-10 items-center gap-2 rounded-lg border border-line-strong bg-surface-2 px-3 text-[13px] font-medium text-content-secondary transition-colors duration-150 ease-out-quint hover:cursor-pointer hover:border-accent hover:bg-surface-3 hover:text-content-primary"
      >
        <img className="h-4 w-4" src="./assets/icons/cog.svg" alt="" aria-hidden="true" />
        <span className="hidden sm:inline">{t("settings.open")}</span>
      </button>

      {/* Portal escapes containing blocks */}
      {isOpen && typeof document !== "undefined" && createPortal(modal, document.body)}
    </>
  );
};

export default SettingsButton;
