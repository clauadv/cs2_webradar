// UI copy per language
// {placeholders} are filled by t()
export const translations = {
  en: {
    lang: {
      label: "Language",
      hint: "Applies across the whole interface",
    },

    status: {
      connecting: "Connecting",
      waiting: "Waiting for data",
      live: "Live",
      disconnected: "Disconnected",
      error: "Connection failed",
      retrying: "Reconnecting shortly",
    },

    app: {
      waitingData: "Connected. Waiting for data from usermode.",
      connectionFailed: "Connection failed.",
      openSettings: "Open Settings to change the target IP or port.",
      radarAlt: "{map} radar overview",
      mapLabel: "Map",
    },

    teams: {
      t: "Terrorists",
      ct: "Counter-Terrorists",
      aliveOf: "{alive} of {total} alive",
    },

    bomb: {
      defused: "C4 defused",
      siteSecured: "Site secured",
      defusingInTime: "Defusing — in time",
      defusingTooLate: "Defusing — too late",
      defuseSeconds: "{seconds}s to defuse",
      planted: "C4 planted",
      critical: "Critical",
      detonating: "Detonating",
      c4: "C4",
      timerLabel: "{label}, {seconds} seconds remaining",
      secondsShort: "s",
    },

    card: {
      dead: "Dead",
      health: "{name} health",
      openSteam: "Open {name} on Steam",
      kevlar: "Kevlar",
      kevlarHelmet: "Kevlar and helmet",
      hasDefuser: "Has defuse kit",
      carryingBomb: "Carrying the C4",
    },

    settings: {
      open: "Settings",
      title: "Settings",
      subtitle: "Saved automatically in this browser",
      close: "Close",
      closeAria: "Close settings",
      reset: "Reset to defaults",
      sections: "Settings sections",
    },

    tabs: {
      connection: "Connection",
      radar: "Radar",
      cards: "Player cards",
    },

    connection: {
      useLocalhost: "Use localhost",
      useLocalhostHint: "Connect to the relay running on this machine",
      targetIp: "Target IP",
      targetIpDisabled: "Disabled while localhost is on",
      targetIpHint: "Public IP of the machine running the relay. Leave empty to use this page's host.",
      targetIpPlaceholder: "e.g. 203.0.113.42",
      port: "Port",
      portHint: "Must match the relay port (default 22006)",
      privateIp:
        'A public IP address is required. "{ip}" is a private/local address — turn on "Use localhost" instead, or enter your public IP.',
      noHost: "No target host could be resolved.",
      unreachable: "Could not reach '{url}'. Check the IP, port and that the relay is running.",
    },

    radar: {
      playerSize: "Player marker size",
      bombSize: "Bomb marker size",
      showDead: "Show deceased players",
      showDeadHint: "Death markers stay on the map",
      showLabels: "Show player labels",
      showLabelsHint: "Floating name and HP above each marker",
    },

    cards: {
      keepDead: "Keep deceased cards",
      keepDeadHint: "Dimmed instead of hidden when a player dies",
      money: "Money",
      armor: "Armor and helmet",
      weapons: "Weapons and utilities",
      steam: "Steam profile link",
      steamHint: "Click a player name to open their profile",
    },
  },

  ru: {
    lang: {
      label: "Язык",
      hint: "Применяется ко всему интерфейсу",
    },

    status: {
      connecting: "Подключение",
      waiting: "Ожидание данных",
      live: "В эфире",
      disconnected: "Соединение потеряно",
      error: "Не удалось подключиться",
      retrying: "Скоро переподключимся",
    },

    app: {
      waitingData: "Подключено. Ждём данные из usermode.",
      connectionFailed: "Не удалось подключиться.",
      openSettings: "Откройте настройки, чтобы изменить IP или порт.",
      radarAlt: "Обзорная карта {map}",
      mapLabel: "Карта",
    },

    teams: {
      t: "Террористы",
      ct: "Спецназ",
      aliveOf: "{alive} из {total} живы",
    },

    bomb: {
      defused: "Бомба разряжена",
      siteSecured: "Точка зачищена",
      defusingInTime: "Разминирование — успевает",
      defusingTooLate: "Разминирование — не успевает",
      defuseSeconds: "{seconds} с до разряда",
      planted: "Бомба установлена",
      critical: "Критично",
      detonating: "До взрыва",
      c4: "Бомба",
      timerLabel: "{label}, осталось {seconds} секунд",
      secondsShort: "с",
    },

    card: {
      dead: "Убит",
      health: "Здоровье {name}",
      openSteam: "Открыть профиль {name} в Steam",
      kevlar: "Бронежилет",
      kevlarHelmet: "Бронежилет и шлем",
      hasDefuser: "Есть набор для разминирования",
      carryingBomb: "Несёт бомбу",
    },

    settings: {
      open: "Настройки",
      title: "Настройки",
      subtitle: "Сохраняются автоматически в этом браузере",
      close: "Закрыть",
      closeAria: "Закрыть настройки",
      reset: "Сбросить по умолчанию",
      sections: "Разделы настроек",
    },

    tabs: {
      connection: "Подключение",
      radar: "Радар",
      cards: "Карточки",
    },

    connection: {
      useLocalhost: "Локальное подключение",
      useLocalhostHint: "Подключаться к релею на этом компьютере",
      targetIp: "IP-адрес",
      targetIpDisabled: "Недоступно при локальном подключении",
      targetIpHint:
        "Публичный IP компьютера с релеем. Оставьте пустым, чтобы использовать адрес этой страницы.",
      targetIpPlaceholder: "например 203.0.113.42",
      port: "Порт",
      portHint: "Должен совпадать с портом релея (по умолчанию 22006)",
      privateIp:
        'Нужен публичный IP-адрес. «{ip}» — локальный адрес. Включите «Локальное подключение» или укажите публичный IP.',
      noHost: "Не удалось определить адрес сервера.",
      unreachable: "Адрес «{url}» недоступен. Проверьте IP, порт и запущен ли релей.",
    },

    radar: {
      playerSize: "Размер маркера игрока",
      bombSize: "Размер маркера бомбы",
      showDead: "Показывать убитых",
      showDeadHint: "Метки смерти остаются на карте",
      showLabels: "Показывать подписи игроков",
      showLabelsHint: "Имя и здоровье над маркером",
    },

    cards: {
      keepDead: "Оставлять карточки убитых",
      keepDeadHint: "Приглушать вместо скрытия, когда игрок погибает",
      money: "Деньги",
      armor: "Броня и шлем",
      weapons: "Оружие и гранаты",
      steam: "Ссылка на профиль Steam",
      steamHint: "Клик по имени открывает профиль игрока",
    },
  },
};
