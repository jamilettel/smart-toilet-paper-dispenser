import { WebSocket } from 'ws'

export type WebSocketClient = WebSocket & { subscribed: boolean }
export type wsFunction = (ws: WebSocketClient, data: string[]) => boolean
