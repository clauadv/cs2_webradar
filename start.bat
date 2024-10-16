@echo off

start "cs2_webradar - webapp" cmd /k "if exist "webapp" (cd webapp && npm run dev) else (echo [cs2_webradar] failed to start webapp! make sure you followed the instructions carefully && pause)"