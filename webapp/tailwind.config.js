/** @type {import('tailwindcss').Config} */
export default {
  content: [
    "./index.html",
    "./src/**/*.{js,ts,jsx,tsx}",
  ],
  theme: {
    extend: {
      colors: {
        /* Solid dark surfaces — layered by lightness, no translucency. */
        base: "#0b0f16",
        sunken: "#0e131b",
        surface: {
          1: "#141a24",
          2: "#1a2130",
          3: "#212a3b",
        },
        line: {
          DEFAULT: "#232c3b",
          strong: "#2f3b4e",
        },
        content: {
          primary: "#eef4fb",
          secondary: "#a8bbd0",
          muted: "#7387a0",
        },
        accent: {
          DEFAULT: "#4e9bf5",
          strong: "#6cadf8",
          dim: "#21365a",
        },
        status: {
          green: "#43c97a",
          amber: "#eba53c",
          red: "#ef5350",
        },
        team: {
          t: "#de6338",
          ct: "#4e9bf5",
        },
      },
      fontFamily: {
        display: ["TASAOrbiter", "system-ui", "sans-serif"],
      },
      transitionTimingFunction: {
        "out-quint": "cubic-bezier(0.22, 1, 0.36, 1)",
      },
    },
  },
  plugins: [],
}
