import { WebSocketServer } from 'ws';

export const wss = new WebSocketServer({
    noServer: true,
});

wss.on('connection', (ws: WebSocket) => {
    ws.onmessage = (message: MessageEvent) => {
        console.log(`received ${message.data}`);
        ws.send(message.data);
    };
});
