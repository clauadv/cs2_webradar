# cs2_webradar
counter strike 2 browser based radar cheat **(work in progress)** <br>
<br>
if you have any issues or questions, feel free to ask on my [discord server](https://discord.gg/wVzuHN8uXd)

## usage
- install [nodejs](https://nodejs.org/en/download/current)
- in `react` and `web_server` folders, run `install.bat`
- in the root folder, run `start.bat` which will start both web server and react development server
- compile `usermode` project
- run `usermode.exe` and, in your browser navigate to `localhost:5173` <br>

## sharing
- in `usermode` project, `usermode.hpp` line 13, change `#define USE_LOCALHOST 1` to `#define USE_LOCALHOST 0`
- in `react` project, `App.jsx` line 10, change `const USE_LOCALHOST = 1;` to `const USE_LOCALHOST = 0;`
- in `react` project, `App.jsx` line 11, change `const PUBLIC_IP = "your ip";` with your ip address

## showcase
https://github.com/clauadv/cs2_webradar/assets/26169415/718d5969-004b-4300-b152-2051a75d490d
