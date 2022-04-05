import * as fs from 'fs'

export let tprDatabase: {
    values: any,
    status: 'disconnected' | 'calibrating' | 'error' | 'working' | 'stopped',
    percentageLeft: number,
} = {
    values: {},
    status: 'disconnected',
    percentageLeft: 0,
}

const DB_FILENAME = 'database.json'

try {
    const file = fs.readFileSync(DB_FILENAME, 'utf-8')
    tprDatabase = JSON.parse(file)
} catch(err) {
    console.error(err)
}

function saveDatabase() {
    fs.writeFile(DB_FILENAME, JSON.stringify(tprDatabase), err => {
        if (err) {
            console.error('Error while writing file: ' + err)
        }
        console.log('Database saved')
    })
}

setInterval(saveDatabase, 300_000) // save every 5 minutes
