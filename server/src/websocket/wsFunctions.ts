
export type wsFunction = (ws: WebSocket, data: string[]) => string

export default [
    {func: (_ws, _data) => {}, command: "test"},
] as {func: wsFunction, command: string}[];
