/** @type {import('tailwindcss').Config} */
export default {
  content: [
    "./index.html",
    "./src/**/*.{js,ts,jsx,tsx}",
  ],
  theme: {
    extend: {
      colors: {
        radar: {
          "primary": "#b1d0e7",
          "secondary": "#6492b4",
          "green": "#50904c",
          "red": "#c90b0be6"
        }
      },
    },
  },
  plugins: [],
}

