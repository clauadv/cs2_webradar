import { WebSocketServer } from "ws";
import http from "http";

const port = 22006;
const server = http.createServer();
const web_socket_server = new WebSocketServer({
    server: server, path: "/cs2_webradar"
});

web_socket_server.on("connection", (web_socket, request) => {
    const client_address = request.socket.remoteAddress.replace("::ffff:", "");

    console.log(`[info] ${client_address} connected`);

    web_socket.on("message", (message) => {
        web_socket_server.clients.forEach((client) => {
            client.send(message);
        });
    });

    web_socket.on("close", () => {
        console.log(`[info] ${client_address} disconnected \n`);
    });

    web_socket.on("error", function (error) {
        console.log(`[error] ${error}`);
    });
});

server.listen(port);
console.log(`[info] listening on port ${port}`);