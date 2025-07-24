import { WebSocketServer } from 'ws';
import http from 'http';
import express from 'express';
import dotenv from 'dotenv';

dotenv.config();

const WS_PORT = process.env.PORT || 22006;
const HTTP_PORT = process.env.HTTP_PORT || 5173;
const AUTH_TOKEN = process.env.AUTH_TOKEN;

const app = express();
app.use(express.static('dist'));
const server = http.createServer(app);
const wss = new WebSocketServer({ port: WS_PORT, path: '/cs2_webradar' });

wss.on('connection', (ws, req) => {
  const url = new URL(req.url, `http://${req.headers.host}`);
  const token = url.searchParams.get('token');
  if (AUTH_TOKEN && token !== AUTH_TOKEN) {
    console.info(`unauthorized connection from ${req.socket.remoteAddress}`);
    ws.close();
    return;
  }

  const client = req.socket.remoteAddress?.replace('::ffff:', '') || 'unknown';
  console.info(`${client} connected`);

  ws.on('message', message => {
    wss.clients.forEach(client => {
      if (client.readyState === ws.OPEN) {
        client.send(message);
      }
    });
  });

  ws.on('close', () => console.info(`${client} disconnected`));
  ws.on('error', error => console.error(error));
});

server.listen(HTTP_PORT, () => {
  console.info(`listening on port '${HTTP_PORT}'`);
});
