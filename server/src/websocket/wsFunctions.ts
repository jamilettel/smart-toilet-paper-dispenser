
export type wsFunction = (ws: WebSocket, data: string) => string

export default [
] as {func: wsFunction, command: string}[];
