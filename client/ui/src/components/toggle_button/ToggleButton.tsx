import {FC, useState} from "react";

interface ToggleButtonProps {
    onClick: () => void;
    leftText: string;
    rightText: string;
}

const ToggleButton: FC<ToggleButtonProps>
    = ({onClick, leftText, rightText}) => {
    const [active, setActive] = useState(true);

    const handleToggle = () => {
        setActive(prevState => !prevState);
        onClick();
    };

    return (
        <div>
            <button onClick={handleToggle} className="ToggleButton">
                <div className={"w-full h-full flex flex-row fl "}>
                    <div className={"flex-1 "} style={{
                        background: active ? 'var(--highlight)' : "none",
                        fontWeight: active ? 'bold' : 'normal',
                        color: active ? 'var(--text)' : 'var(--text-disabled)',
                        borderRadius: 0,
                    }}>
                        {leftText}
                    </div>
                    <div className={"flex-1 "} style={{
                        background: !active ? 'var(--highlight)' : "none",
                        fontWeight: !active ? 'bold' : 'normal',
                        color: !active ? 'var(--text)' : 'var(--text-disabled)',
                        borderRadius: 0,
                    }}>
                        {rightText}
                    </div>
                </div>
            </button>


        </div>
    )
        ;


}
export {ToggleButton}