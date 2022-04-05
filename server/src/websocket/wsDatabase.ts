export const tprDatabase: {
    values: any,
    status: 'disconnected' | 'calibrating' | 'error' | 'working',
    percentageLeft: number,
} = {
    values: {},
    status: 'disconnected',
    percentageLeft: 0,
}

setInterval(() => {
    
}, 300_000) // save every 5 minutes
