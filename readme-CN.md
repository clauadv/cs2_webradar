# cs2_网页雷达

> [!IMPORTANT]
> 此版本仍受支持，但 **[hurracan](https://hurracan.com)** 提供了即插即用体验和即时分享功能。

[![discord](https://img.shields.io/badge/discord-5865F2?style=flat&logo=discord&logoColor=white)](https://discord.gg/qEBpufFkXK)
[![en](https://img.shields.io/badge/lang-en-blue.svg)](https://github.com/clauadv/cs2_webradar/blob/master/readme.md) <br>
未检测到的 Counter-Strike 2 基于浏览器的雷达作弊

## 系统要求
- [Node.js](https://nodejs.org/en/download)
- [Visual Studio Community](https://visualstudio.microsoft.com/vs/community/)

## 使用方法
- 在根文件夹中，运行 `install.bat` 来安装依赖
- 在根文件夹中，运行 `start.bat` 来启动网页应用
- 在 `usermode`   项目中，运行 `cs2_webradar.sln`
- 在 `Visual Studio` 的工具栏中，将鼠标悬停在 `生成` 上并点击 `生成解决方案` 或按 `Ctrl + Shift + B`
- 在 `release` 文件夹中，运行 `usermode.exe`，然后在浏览器中访问 `localhost:5173`

## 分享给他人
> [!TIP]
> 为了更轻松的分享，考虑使用 **[hurracan](https://hurracan.com)**，它可以自动处理此功能。
- 构建 `usermode` 项目后，打开 `config.json` 并将 `m_use_localhost` 改为 `false`
- 在 `react` 项目的 `App.jsx` 第10行，将 `const USE_LOCALHOST = 1;` 改为 `const USE_LOCALHOST = 0;`
- 在 `react` 项目的 `App.jsx` 第12行，将 `const PUBLIC_IP = "your ip";` 中的内容改为你的IP地址
- 在 `cmd` 中，输入 `ipconfig`，找到 `默认网关` 并在浏览器中访问它
- 在路由器配置中，找到 `端口转发` 标签页并转发端口 `22006/tcp` 和 `5173/tcp`
- 现在你的朋友可以通过访问 `你的公网IP:5173` 来查看雷达

## 视频展示
https://github.com/clauadv/cs2_webradar/assets/26169415/718d5969-004b-4300-b152-2051a75d490d

## 支持
- 官匹 ✅
- R0对战平台 ？
- 完美世界 ❌
- 5E对战平台 ❌

## 许可协议
本项目基于 [GPL-3.0 license](https://github.com/clauadv/cs2_webradar?tab=GPL-3.0-1-ov-file#readme) 许可协议