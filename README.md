# CS2 WebRadar

This project provides a browser-based radar for Counter Strike 2. The web application uses React and a WebSocket server written in Node.js.

## Features
- Real-time radar display in the browser
- Simple token-based authentication for the WebSocket server
- Configuration through `.env` and `config.json`
- Docker support via `docker-compose`

## Getting Started

### 1. Configuration
1. Copy `.env.example` to `.env` and adjust the values if needed. The most important settings are the WebSocket `PORT` and the `AUTH_TOKEN` used for authentication.
2. Run the program once so that `config.json` is generated in the project root. Edit `m_local_ip` in this file to match the local IP address of the machine running `usermode.exe`. Adjust `m_ws_port` if you changed the WebSocket port.

### 2. Build and Run with Docker
Make sure you have Docker and docker‑compose installed.

```bash
docker-compose up --build
```

The web interface will be available on port `5173` and the WebSocket server on the port defined in `.env`/`config.json` (default `22006`).

### 3. Using the Application
1. Build the `usermode` project using Visual Studio and run `usermode.exe`.
2. Open your browser at `http://localhost:5173` (or the host configured in `.env`).
3. The `usermode` component connects to the WebSocket server using the IP from `config.json` and the token from `.env`.

## Authentication
All WebSocket connections must include the token as a query parameter named `token`:

```
ws://<host>:<port>/cs2_webradar?token=<AUTH_TOKEN>
```

Connections without the correct token are rejected.

## License
This project is licensed under the [GPL-3.0](LICENSE) license.
