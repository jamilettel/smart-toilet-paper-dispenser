import styles from './Status.module.css'

const mapStatus = {
    'disconnected': 'Disconnected',
    'calibrating': 'Measuring',
    'error': 'Empty',
    'working': 'Working',
    'stopped': 'Stopped',
}

export default function Status(props: {
    status: 'disconnected' | 'calibrating' | 'error' | 'working' | 'stopped',
    className?: string,
}) {
    return (
        <div className={styles.status + ` ${props.className ?? ''}`}>
            <h2>Status</h2>
            <div className={styles.container + ' ' + styles[props.status]}>
                <div className={styles.statusText}>
                    {mapStatus[props.status]}
                    <div className={styles.emoji}></div>
                </div>
            </div>
        </div>
    )
}
