import {interval, timer} from "d3";
import {useEffect, useState} from "react";

import {
    HeadTrackingState,
    getHeadTracking,
    defaultHeadTrackingState,

} from "@/ipc/headtracking_ipc";

export interface IUseHeadTracking {
    headTracking: HeadTrackingState;
}

export const useHeadTracking = (): IUseHeadTracking => {
    const [headTracking, setHeadTracking] = useState<HeadTrackingState>(defaultHeadTrackingState);

    useEffect(() => {
        const updateInterval = 100;
        let previousTime = 0;

        let newHeadTracking: HeadTrackingState = defaultHeadTrackingState;

        const updateTimer = interval(async () => {
            newHeadTracking = await getHeadTracking();
        }, updateInterval);


        const headTrackingTimer = timer(async (elapsed) => {

            if (elapsed - previousTime >= updateInterval) {
                previousTime = elapsed;
                setHeadTracking({yaw: newHeadTracking.yaw, pitch: newHeadTracking.pitch});
            }
        });


        return () => {
            headTrackingTimer.stop();
            updateTimer.stop();
        };
    }, []);

    return {headTracking};
};
