# cs2_webradar
counter strike 2 browser based radar cheat **(work in progress)** <br>
<br>
if you have any issues or questions, feel free to ask on my [discord server](https://discord.gg/wVzuHN8uXd)

## usage
- in `react` folder, type `npm install` to install the required dependencies
- in the root folder, run `start.bat` which will start both web server and react development server
- compile `usermode` project
- navigate to [a2x's offset dumper](https://github.com/a2x/cs2-dumper/tree/main/generated), download `client.dll.json` and `offsets.json` and place them in the same folder with `usermode.exe`
- run `usermode.exe` and, in your browser navigate to `localhost:5173` <br><br>

## sharing
- in `react` project, `App.jsx` line 21, change `ws://localhost:22006/cs2_webradar` with `ws://<YOUR PUBLIC IP>:22006/cs2_webradar`
- you can find your public ip [here](https://ipinfo.io/ip)
- example: `ws://188.212.166.147:22006/cs2_webradar` <br><br>
- in `usermode` project, `dllmain.cpp` line 23, change `ws://localhost:22006/cs2_webradar` with `ws://192.168.x.x:22006/cs2_webradar`
- you can find your ipv4 address by typing `ipconfig` in command prompt
- example: `ws://192.168.1.100:22006/cs2_webradar`

## showcase
https://github.com/clauadv/cs2_webradar/assets/26169415/718d5969-004b-4300-b152-2051a75d490d
