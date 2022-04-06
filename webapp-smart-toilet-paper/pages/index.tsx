import type { NextPage } from 'next'
import Head from 'next/head'
import { useEffect, useState } from 'react'
import Button from '../components/Button'
import { WEBSOCKET_URL } from '../utils/constants'
import styles from './Home.module.css'
import { buildStyles, CircularProgressbar } from 'react-circular-progressbar'
import 'react-circular-progressbar/dist/styles.css'

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
            setSock(socket)
        }
    }, [])

    const percentage = data?.percentageLeft ?? 0

    return (
        <div className={styles.container}>
            <Head>
                <title>Toilet Paper Dispenser</title>
                <meta name="description" content="The smart toilet paper dispenser web page" />
                <link rel="icon" href="/favicon.ico" />
                <link rel="preconnect" href="https://fonts.googleapis.com" />
                <link rel="preconnect" href="https://fonts.gstatic.com" crossOrigin='true' />
                <link href="https://fonts.googleapis.com/css2?family=Overpass:ital,wght@0,100;0,200;0,300;0,400;0,500;0,600;0,700;0,800;0,900;1,100;1,200;1,300;1,400;1,500;1,600;1,700;1,800;1,900&display=swap" rel="stylesheet" />
            </Head>

            <main className={styles.main}>
                <div className={styles.content}>
                    <h1 className={styles.title}>Smart Toilet Paper Dispenser</h1>
                    {JSON.stringify(data)}
                    <CircularProgressbar
                        value={percentage}
                        text={`${percentage}%`}
                        className={styles.percentageLeft}
                        styles={buildStyles({
                            pathColor: '#363d66',
                            textColor: '#363d66',
                            trailColor: '#ddd',
                        })}
                    />
                    <br />
                    <Button className={styles.button} onClick={() => sock?.send('stop')} label="Stop" />
                    <Button className={styles.button} onClick={() => sock?.send('start')} label="Start" />
                    <br />
                    <Button className={styles.button} onClick={() => sock?.send('measure')} label="Measure" />
                    <Button className={styles.button} onClick={() => sock?.send('calibrate')} label="Calibrate" />
                </div>
            </main>

        </div>
    )
}

export default Home
