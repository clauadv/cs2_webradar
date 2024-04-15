# cs2_网页雷达
未检测到的 Counter Strike 2 基于浏览器的雷达作弊 **(进程式)** <br>
<br>
如果您有任何建议、问题或疑问，请随时将它们发送到 [Discord 服务器](https://discord.gg/wVzuHN8uXd)上

## 用法
- 下载并安装 [nodejs](https://nodejs.org/en/download/current/)
- 在根文件夹运行 `install.bat` 来安装依赖
- 在根文件夹运行, 运行 `start.bat` 来开启网页服务器与react development服务器
- 使用 [visual studio](https://visualstudio.microsoft.com/vs/community/) 编译 `usermode` 项目
- 打开 `usermode.exe` 然后在你的浏览器中输入`localhost:5173` <br>

## 共享给他人
- 在 `usermode` 文件夹中, `usermode.hpp` 第二十四行, 把修改 `#define USE_LOCALHOST 1` 成 `#define USE_LOCALHOST 0`
- 在 `react` 文件夹中的src文件夹中, `App.jsx` 第十行, 把修改 `const USE_LOCALHOST = 1;` 成 `const USE_LOCALHOST = 0;`
- 在 `react` 文件夹中的src文件夹中, `App.jsx` 第十二行, 把修改 `const PUBLIC_IP = "your ip";` 成你的ip地址
- 在 `cmd`, 输入 `ipconfig`, 找到 `默认网关` 并在浏览器中导航到它
- 在你的路由器设置中, 找到 `端口转发` 功能 并转发端口 `22006/tcp` 和 `5173/tcp`
- 现在你的朋友能够通过网页`你的ip:5173`看到你的雷达了 
- 注：如果你没有公网ip，你可能需要内网穿透

## 视频展示
https://github.com/clauadv/cs2_webradar/assets/26169415/718d5969-004b-4300-b152-2051a75d490d

## 支持
- 官匹 ✅
- R0对战平台 ✅
- 完美世界 ❌
- 5E对战平台 ❌


