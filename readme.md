# cs2_webradar
[![en](https://img.shields.io/badge/lang-en-blue.svg)](https://github.com/clauadv/cs2_webradar/blob/master/readme.md)
[![en](https://img.shields.io/badge/lang-cn-blue.svg)](https://github.com/clauadv/cs2_webradar/blob/master/readme-CN.md) <br>
undetected counter strike 2 browser based radar cheat <br>
<br>
if you have any suggestions, issues or questions, feel free to send them on my [discord server](https://discord.gg/wVzuHN8uXd)

## usage
- download and install [nodejs](https://nodejs.org/en/download/prebuilt-installer/current/)
- in the root folder, run `install.bat` to install dependencies
- in the root folder, run `start.bat` to start both web server and react development server
- compile `usermode` project using [visual studio](https://visualstudio.microsoft.com/vs/community/)
- run `usermode.exe` and, in your browser navigate to `localhost:5173` <br>

## sharing
- in `usermode` project, `/src/common.hpp` line 6, change `#define USE_LOCALHOST 1` to `#define USE_LOCALHOST 0`
- in `react` project, `App.jsx` line 10, change `const USE_LOCALHOST = 1;` to `const USE_LOCALHOST = 0;`
- in `react` project, `App.jsx` line 12, change `const PUBLIC_IP = "your ip";` with your ip address
- in `cmd`, type `ipconfig`, find `Default Gateway` and navigate to it in your browser
- in your router configuration, find `Port Forwarding` tab and forward port `22006/tcp` and `5173/tcp`
- now your friends can see the radar by navigating to `your public ip:5173`

## showcase
https://github.com/clauadv/cs2_webradar/assets/26169415/718d5969-004b-4300-b152-2051a75d490d
