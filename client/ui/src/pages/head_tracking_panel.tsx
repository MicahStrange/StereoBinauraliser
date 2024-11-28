import {useHeadTracking} from "@/hooks/use_headtracking";
import React from "react";

const HeadTrackingPanel = () => {
    const {headTracking} = useHeadTracking();

    return (
        <div
            className={" text-xl w-full h-full flex flex-col justify-around items-center border-2"}
            style={{
                border: '3px solid var(--secondary-background)',
                background: ' var(--primary)'
            }}>
            < div> HEAD TRACKING</div>
            <div className={" text-xl w-full flex flex-row justify-around items-center"}>
                <div>
                    <div>YAW:</div>
                    <div className={"p-2 w-16"}>{headTracking.yaw.toFixed(1)}</div>
                </div>
                <div>
                    <div>PITCH:</div>
                    <div className={"p-2 w-16"}>{headTracking.pitch.toFixed(1)}</div>
                </div>
            </div>
        </div>
    )
}

export default HeadTrackingPanel;