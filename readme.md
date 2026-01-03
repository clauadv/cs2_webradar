# cs2_webradar

<a href="https://www.buymeacoffee.com/clauadv"><img src="https://www.buymeacoffee.com/assets/img/custom_images/orange_img.png" alt="en" width="95" /></a>
[![en](https://img.shields.io/badge/lang-en-blue.svg)](https://github.com/clauadv/cs2_webradar/blob/master/readme.md)
[![en](https://img.shields.io/badge/lang-cn-blue.svg)](https://github.com/clauadv/cs2_webradar/blob/master/readme-CN.md) <br>
undetected counter strike 2 browser based radar cheat <br>
if you have any suggestions, issues or questions, feel free to send them on my [discord server](https://discord.gg/wVzuHN8uXd)
# hurracan
i’m excited to introduce <a href="https://hurracan.com">hurracan</a> —<b>a plug & play</b> version that’s designed to make your life easier.<br>
no more headaches with setup or configuration. just run it, and a server is automatically created with <b>a shareable link</b>—ready to go in seconds!<br>

## Requirements
- [nodejs](https://nodejs.org/en/download)
- [Visual Studio (2022 or higher)](https://visualstudio.microsoft.com/vs/community/)
- MSVC v143 (Visual Studio 2022 C++ x64/x86 build tools)
- Windows SDK

## Usage
1. Install dependencies:
    - In the root directory, run `install.bat` to install dependencies.

2. Build the usermode application:
    - Open `usermode\cs2_webradar.sln` in Visual Studio
    - Set the build configuration to `release` and platform to `x64`
    - In Visual Studio's toolbar, click `Build` -> `Build Solution` or press `Ctrl + Shift + B`

3. Start the web application:
    - In the root directory, run `start.bat` to start the web server.
    - A new terminal will appear. Keep this open.

4. Running the radar:
    - Start `cs2.exe`
    - In the `release` directory, run `usermode.exe`
    - In your browser, navigate to `localhost:5173`

5. Shutting down:
  - To stop the web server, press `Ctrl + C` in the terminal window or simply close it.
  - Close `usermode.exe`
  - As a precaution, fully close `cs2.exe` or restart your system before switching accounts.

> **Note:** On first startup, `usermode.exe` will fail because `config.json` doesn't exist.
The application will create this file automatically. Simply run `usermode.exe` again following this file creation.

## Sharing
- after you built `usermode` project, open `config.json` and change `m_use_localhost` to `false`
- in `react` project, `App.jsx` line 10, change `const USE_LOCALHOST = 1;` to `const USE_LOCALHOST = 0;`
- in `react` project, `App.jsx` line 12, change `const PUBLIC_IP = "your ip";` with your ip address
- in `cmd`, type `ipconfig`, find `Default Gateway` and navigate to it in your browser
- in your router configuration, find `Port Forwarding` tab and forward port `22006/tcp` and `5173/tcp`
- now your friends can see the radar by navigating to `your public ip:5173`

## Showcase
https://github.com/clauadv/cs2_webradar/assets/26169415/718d5969-004b-4300-b152-2051a75d490d

## License
this project is licensed under the [GPL-3.0 license](https://github.com/clauadv/cs2_webradar?tab=GPL-3.0-1-ov-file#readme)

## Contributors
<a href="https://github.com/clauadv/cs2_webradar/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=clauadv/cs2_webradar" />
</a>
