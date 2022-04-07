import styles from './Button.module.css'

export default function Button(props: {
    onClick?: () => void,
    className?: string,
    label?: string,
    disabled?: boolean,
}) {
    const onClick = props.disabled ? undefined : props.onClick;
    let className = styles.button
    if (props.className)
        className += ` ${props.className}`
    if (props.disabled)
        className += ` ${styles.disabled}`
    return (
        <button
            className={className}
            onClick={onClick}
        >
            {props.label ?? 'Button'}
        </button>
    );
}
