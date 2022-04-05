import { wsFunction } from '@/websocket/wsTypes'
import { tprDatabase } from '@/websocket/wsDatabase'

export const saveValueInDb: wsFunction = (_ws, args) => {
    if (args.length !== 3)
        return false
    const key = args[1]
    const value = args[2]
    tprDatabase.values[key] = value
    return true
}

export const getValue: wsFunction = (ws, args) => {
    if (args.length !== 2)
        return false
    const key = args[1]
    const value = tprDatabase.values[key]
    if (tprDatabase.values[key] === undefined)
        return false
    ws.send(`value ${key} ${value}`)
    return true
}

export const setStatus: wsFunction = (_ws, args) => {
    if (args.length !== 2)
        return false
    const status = args[1]
    if (status == 'disconnected' || status == 'calibrating' || status == 'error' || status == 'working')
        tprDatabase.status = status
    return true
}

export const setPercentage: wsFunction = (_ws, args) => {
    if (args.length !== 2)
        return false
    const percentage = parseFloat(args[1])
    tprDatabase.percentageLeft = percentage
    return true
}

export const subscribe: wsFunction = (ws, _args) => {
    ws.subscribed = true
    return false
}

export const getDatabase: wsFunction = (ws, _args) => {
    ws.send(JSON.stringify(tprDatabase))
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

] as { func: wsFunction, command: string }[]
