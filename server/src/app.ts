import * as express from 'express'
import { Request, Response } from 'express'
import { wss } from '@/websocket/wsSetup'
import * as cors from 'cors'


const app: express.Application = express()
const port: number = parseInt(process.env.PORT || '3000')

app.use(cors())

app.get('/', (_req: Request, res: Response) => {
    return res.json({hello: 'world'})
})

const server = app.listen(port, () => {
    console.log(`Server listening at port: ${port}`)
})

export default server

server.on('upgrade', (request, socket, head) => {
    wss.handleUpgrade(request, socket, head, socket => {
        wss.emit('connection', socket, request)
    })
})
