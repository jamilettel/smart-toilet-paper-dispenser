import { WebSocketServer } from 'ws'
import wsFunctions, { setStatus, getDatabase } from '@/websocket/wsFunctions'
import { WebSocketClient } from '@/websocket/wsTypes'

export const wss = new WebSocketServer<WebSocketClient>({
    noServer: true,
})

function sendToSubscribers() {
    wss.clients.forEach(client => {
        if (client.subscribed)
            getDatabase(client, ['get-database'])
    })
}

wss.on('connection', (ws) => {
    ws.subscribed = false

    ws.onmessage = (message) => {

        console.log(message.data)

        const args = (message.data as string).trim().split(' ')
        if (args.length === 0)
            return

        for (const func of wsFunctions)
            if (func.command === args[0]) {
                if (func.func(ws, args) === true)
                    sendToSubscribers()
                return
            }

        console.log(`command not found: ${args[0]}`)
    }
    ws.onclose = () => {
        if (ws.subscribed === false) {
            setStatus(ws, ['status', 'disconnected'])
            sendToSubscribers()
        }
    }
})
