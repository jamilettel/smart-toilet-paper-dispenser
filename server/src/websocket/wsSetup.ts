import { WebSocketServer } from 'ws';
import wsFunctions from '@/websocket/wsFunctions';

export const wss = new WebSocketServer({
    noServer: true,
});

wss.on('connection', (ws: WebSocket) => {
    ws.onmessage = (message: MessageEvent) => {
        let args = (message.data as string).split(' ')
        if (args.length == 0)
            return

        for (let func of wsFunctions)
            if (func.command == args[0])
                func.func(ws, args)
    };
});
