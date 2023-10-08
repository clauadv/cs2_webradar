import { WebSocketServer } from "ws";
import http from "http";

const LOG_INFO = async (...args) => console.info(`[info] ${args.join(' ')}`);
const LOG_ERROR = async (...args) => console.error(`[error] ${args.join(' ')}`);

const port = 22006;
const server = http.createServer();
const web_socket_server = new WebSocketServer(
{
    server: server, path: "/cs2_webradar"
});

web_socket_server.on("connection", (web_socket, request) =>
{
    const client_address = request.socket.remoteAddress.replace("::ffff:", "");

    LOG_INFO(`${client_address} connected`);

    web_socket.on("message", (message) =>
    {
        web_socket_server.clients.forEach((client) =>
        {
            client.send(message);
        });
    });

    web_socket.on("close", () =>
    {
        LOG_INFO(`${client_address} disconnected \n`);
    });

    web_socket.on("error", (error) =>
    {
        LOG_ERROR(`${error}`);
    });
});

server.listen(port);
LOG_INFO(`listening on port ${port}`);