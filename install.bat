@echo off

start "cs2_webradar - webapp" cmd /k "if exist "webapp" (echo [cs2_webradar] installing webapp dependencies.. && cd "webapp" && npm install && npm audit fix && echo [cs2_webradar] installed react dependencies) else (echo [cs2_webradar] failed to install webapp dependencies! make sure you followed the instructions carefully && pause)"