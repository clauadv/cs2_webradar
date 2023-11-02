@echo off
start "cs2_webradar" cmd /k "cd web_server && node app.js"
start "cs2_webradar" cmd /k "cd react && npm run dev"