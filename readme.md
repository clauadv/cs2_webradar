# cs2_webradar

> [!IMPORTANT]
> This version is still supported, but **[hurracan](https://hurracan.com)** provides a plug & play experience with instant sharing, completely new and responsive interface, a custom overlay for seamless integration with the game, and kernel level security. **[Get your 7-day free trial today!](https://hurracan.com/account/products/web-radar/buy-plan)**

[![discord](https://img.shields.io/badge/discord-5865F2?style=flat&logo=discord&logoColor=white)](https://discord.gg/qEBpufFkXK)
[![en](https://img.shields.io/badge/lang-en-blue.svg)](https://github.com/clauadv/cs2_webradar/blob/master/readme.md)
[![en](https://img.shields.io/badge/lang-cn-blue.svg)](https://github.com/clauadv/cs2_webradar/blob/master/readme-CN.md) <br>
Undetected Counter-Strike 2 browser-based radar cheat

## Requirements
- [Node.js](https://nodejs.org/en/download)
- [Visual Studio Community](https://visualstudio.microsoft.com/vs/community/)

## Usage
- In the root folder, run `install.bat` to install dependencies
- In the root folder, run `start.bat` to start the webapp
- In `usermode` project, run `cs2_webradar.sln`
- In visual studio's toolbar, hover on `Build` and press `Build Solution` or press `Ctrl + Shift + B`
- In `release` folder, run `usermode.exe` and, in your browser navigate to `localhost:5173` <br>

## Sharing
> [!TIP]
> For easier sharing, consider **[hurracan](https://hurracan.com)** which handles this automatically.
- After you built `usermode` project, open `config.json` and change `m_use_localhost` to `false`
- In `react` project, `App.jsx` line 10, change `const USE_LOCALHOST = 1;` to `const USE_LOCALHOST = 0;`
- In `react` project, `App.jsx` line 12, change `const PUBLIC_IP = "your ip";` with your IP Address
- In `cmd`, type `ipconfig`, find `Default Gateway` and navigate to it in your browser
- In your router configuration, find `Port Forwarding` tab and forward port `22006/tcp` and `5173/tcp`
- Now your friends can see the radar by navigating to `your public ip:5173`

## Showcase
https://github.com/clauadv/cs2_webradar/assets/26169415/718d5969-004b-4300-b152-2051a75d490d

## License
This project is licensed under the [GPL-3.0 license](https://github.com/clauadv/cs2_webradar?tab=GPL-3.0-1-ov-file#readme)
