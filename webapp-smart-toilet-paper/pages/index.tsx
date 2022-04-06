import type { NextPage } from 'next'
import Head from 'next/head'
import { useEffect, useState } from 'react'
import { WEBSOCKET_URL } from '../utils/constants'
import styles from './Home.module.css'

type DataType = {
    values: any,
    status: 'disconnected' | 'calibrating' | 'error' | 'working' | 'stopped',
    percentageLeft: number,
}

const Home: NextPage = () => {
    const [sock, setSock] = useState(null as WebSocket | null)
    const [data, setData] = useState(undefined as DataType | undefined)

    useEffect(() => {
        if (typeof window !== 'undefined') {
            let socket = new WebSocket(WEBSOCKET_URL)
            socket.onopen = () => {
                socket.send('subscribe')
                socket.send('get-database')
            }
            socket.onmessage = (message) => {
                try { setData(JSON.parse(message.data)) } catch (_) { }
            }
            setSock(sock)
        }
    }, [])
    return (
        <div className={styles.container}>
            <Head>
                <title>Toilet Paper Dispenser</title>
                <meta name="description" content="The smart toilet paper dispenser web page" />
                <link rel="icon" href="/favicon.ico" />
            </Head>

            <main className={styles.main}>
                {JSON.stringify(data)}
            </main>

        </div>
    )
}

export default Home
