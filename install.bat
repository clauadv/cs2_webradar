@echo off

start "cs2_webradar - web_server" cmd /k "if exist "web_server" (echo [cs2_webradar] installing web server dependencies.. && cd "web_server" && npm install && echo [cs2_webradar] installed web server dependencies) else (echo "error" && exit /b 1)"
start "cs2_webradar - react" cmd /k "if exist "react" (echo [cs2_webradar] installing react dependencies.. && cd "react" && npm install && npm audit fix && echo [cs2_webradar] installed react dependencies) else (echo "error" && exit /b 1)"
