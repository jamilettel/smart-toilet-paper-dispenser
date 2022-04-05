import { WebSocketServer } from 'ws';
import wsFunctions, { setStatus, getDatabase } from '@/websocket/wsFunctions';
import { WebSocketClient } from '@/websocket/wsTypes';

export const wss = new WebSocketServer<WebSocketClient>({
    noServer: true,
});

function sendToSubscribers() {
    wss.clients.forEach(client => {
        if (client.subscribed)
            getDatabase(client, ['get-database'])
    })
}

wss.on('connection', (ws) => {
    ws.subscribed = false

    ws.onmessage = (message) => {
        let args = (message.data as string).split(' ')
        if (args.length == 0)
            return

        for (let func of wsFunctions)
            if (func.command == args[0]) {
                if (func.func(ws, args))
                    sendToSubscribers()
            }

        console.log(`command not found: ${args[0]}`)
    };
    ws.onclose = () => {
        setStatus(ws, ['status', 'disconnected'])
    }
});
