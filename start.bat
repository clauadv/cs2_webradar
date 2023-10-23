@echo off
start "cs2_webradar" cmd /k "cd web_server && npm install && node app.js"
start "cs2_webradar" cmd /k "cd react && npm run dev"