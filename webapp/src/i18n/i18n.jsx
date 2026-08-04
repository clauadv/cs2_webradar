import { createContext, useContext, useMemo } from "react";
import { translations } from "./translations";

export const LANGUAGES = [
  { id: "en", short: "EN", name: "English" },
  { id: "ru", short: "RU", name: "Русский" },
];

export const DEFAULT_LANGUAGE = "en";

// Picks language from browser locale
export const detectLanguage = () => {
  try {
    const tag = (navigator.language || "").toLowerCase();
    const match = LANGUAGES.find((lang) => tag.startsWith(lang.id));
    return match ? match.id : DEFAULT_LANGUAGE;
  } catch {
    return DEFAULT_LANGUAGE;
  }
};

const resolve = (dict, path) =>
  path.split(".").reduce((acc, part) => (acc == null ? undefined : acc[part]), dict);

const interpolate = (text, vars) =>
  Object.keys(vars).reduce((acc, key) => acc.split(`{${key}}`).join(String(vars[key])), text);

const LanguageContext = createContext({
  language: DEFAULT_LANGUAGE,
  locale: "en-US",
  t: (path) => path,
});

export const LanguageProvider = ({ language, children }) => {
  const value = useMemo(() => {
    const active = translations[language] ? language : DEFAULT_LANGUAGE;

    // Falls back to English
    const t = (path, vars) => {
      const text = resolve(translations[active], path) ?? resolve(translations.en, path);
      if (typeof text !== "string") return path;
      return vars ? interpolate(text, vars) : text;
    };

    return { language: active, locale: active === "ru" ? "ru-RU" : "en-US", t };
  }, [language]);

  return <LanguageContext.Provider value={value}>{children}</LanguageContext.Provider>;
};

export const useT = () => useContext(LanguageContext);
