import { wsFunction } from '@/websocket/wsTypes'
import { tprDatabase } from '@/websocket/wsDatabase'
import { wss } from '@/websocket/wsSetup'

const saveValueInDb: wsFunction = (_ws, args) => {
    if (args.length !== 3)
        return false
    const key = args[1]
    const value = args[2]
    if (value == tprDatabase.values[key])
        return false
    tprDatabase.values[key] = value
    return true
}

const getValue: wsFunction = (ws, args) => {
    if (args.length !== 2)
        return false
    const key = args[1]
    const value = tprDatabase.values[key]
    if (tprDatabase.values[key] === undefined)
        return false
    ws.send(`value ${key} ${value}`)
    return false
}

export const setStatus: wsFunction = (_ws, args) => {
    if (args.length !== 2)
        return false
    const status = args[1]
    if (status === 'disconnected' || status === 'calibrating' || status === 'error' || status === 'working' || status === 'stopped')
        tprDatabase.status = status
    return true
}

const setPercentage: wsFunction = (_ws, args) => {
    if (args.length !== 2)
        return false
    const percentage = parseFloat(args[1])
    tprDatabase.percentageLeft = percentage
    return true
}

const subscribe: wsFunction = (ws, _args) => {
    ws.subscribed = true
    return false
}

export const getDatabase: wsFunction = (ws, _args) => {
    ws.send(JSON.stringify(tprDatabase))
    return false
}

const calibrate: wsFunction = (_ws, _args) => {
    wss.clients.forEach(client => {
        if (client.subscribed === false)
            client.send('calibrate')
    })
    return false
}

const measure: wsFunction = (_ws, _args) => {
    wss.clients.forEach(client => {
        if (client.subscribed === false)
            client.send('measure')
    })
    return false
}

const stopTpd: wsFunction = (_ws, _args) => {
    wss.clients.forEach(client => {
        if (client.subscribed === false)
            client.send('stop')
    })
    return false
}

const startTpd: wsFunction = (_ws, _args) => {
    wss.clients.forEach(client => {
        if (client.subscribed === false)
            client.send('start')
    })
    return false
}

export default [

    // IoT commands
    { func: saveValueInDb, command: 'save' },
    { func: getValue, command: 'get' },
    { func: setStatus, command: 'status' },
    { func: setPercentage, command: 'set-percentage' },

    // WebApp commands
    { func: subscribe, command: 'subscribe' },
    { func: getDatabase, command: 'get-database' },
    { func: calibrate, command: 'calibrate' },
    { func: measure, command: 'measure' },
    { func: stopTpd, command: 'stop' },
    { func: startTpd, command: 'start' },

] as { func: wsFunction, command: string }[]
