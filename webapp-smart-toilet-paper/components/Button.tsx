import styles from './Button.module.css'

export default function Button(props: {
    onClick?: () => void,
    className?: string,
    label?: string,
}) {
    return (
        <button className={styles.button + ' ' + props.className ?? ''} onClick={props.onClick}>
            {props.label ?? 'Button'}
        </button>
    );
}
