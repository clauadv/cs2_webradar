# cs2_webradar

<a href="https://www.buymeacoffee.com/clauadv"><img src="https://www.buymeacoffee.com/assets/img/custom_images/orange_img.png" alt="en" width="95" /></a>
[![en](https://img.shields.io/badge/lang-en-blue.svg)](https://github.com/clauadv/cs2_webradar/blob/master/readme.md)
[![en](https://img.shields.io/badge/lang-cn-blue.svg)](https://github.com/clauadv/cs2_webradar/blob/master/readme-CN.md) <br>
undetected counter strike 2 browser based radar cheat <br>
if you have any suggestions, issues or questions, feel free to send them on my [discord server](https://discord.gg/wVzuHN8uXd)
# hurracan
i’m excited to introduce <a href="https://hurracan.com">hurracan</a> —<b>a plug & play</b> version that’s designed to make your life easier.<br>
no more headaches with setup or configuration. just run it, and a server is automatically created with <b>a shareable link</b>—ready to go in seconds!<br>

## requirements
- [nodejs](https://nodejs.org/en/download/prebuilt-installer/current/)
- [visual studio ide](https://visualstudio.microsoft.com/vs/community/)

## usage
- in the root folder, run `install.bat` to install dependencies
- in the root folder, run `start.bat` to start the webapp
- in `usermode` project, run `cs2_webradar.sln`
- in visual studio's toolbar, hover on `Build` and press `Build Solution` or press `Ctrl + Shift + B`
- in `release` folder, run `usermode.exe` and, in your browser navigate to `localhost:5173` <br>

## sharing
- after you built `usermode` project, open `config.json` and change `m_use_localhost` to `false`
- in `react` project, `App.jsx` line 10, change `const USE_LOCALHOST = 1;` to `const USE_LOCALHOST = 0;`
- in `react` project, `App.jsx` line 12, change `const PUBLIC_IP = "your ip";` with your ip address
- in `cmd`, type `ipconfig`, find `Default Gateway` and navigate to it in your browser
- in your router configuration, find `Port Forwarding` tab and forward port `22006/tcp` and `5173/tcp`
- now your friends can see the radar by navigating to `your public ip:5173`

## showcase
https://github.com/clauadv/cs2_webradar/assets/26169415/718d5969-004b-4300-b152-2051a75d490d

## license
this project is licensed under the [GPL-3.0 license](https://github.com/clauadv/cs2_webradar?tab=GPL-3.0-1-ov-file#readme)

## contributors
<a href="https://github.com/clauadv/cs2_webradar/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=clauadv/cs2_webradar" />
</a>
