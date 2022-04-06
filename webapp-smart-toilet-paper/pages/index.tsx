import type { NextPage } from 'next'
import Head from 'next/head'
import { useEffect, useState } from 'react'
import SockJS from 'sockjs-client'
import styles from '../styles/Home.module.css'

const Home: NextPage = () => {
    const [sock, setSock] = useState(null as WebSocket | null)

    useEffect(() => {
        if (typeof window !== 'undefined') {
            const socket = new SockJS('http://167.71.9.47:3000')
            socket.onopen = () => {
                socket.send('subscribe')
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
                Hello
            </main>

        </div>
    )
}

export default Home
