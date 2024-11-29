import {z} from "zod";

import {
    addNativeEventListener,
    juce,
    removeNativeEventListener,
} from "@/lib/juce";


const HeadTrackingState = z.object({
    yaw: z.number(),
    pitch: z.number(),
});

export type HeadTrackingState = z.infer<typeof HeadTrackingState>;

const getHeadTrackingNative = juce.getNativeFunction("get_head_tracking_native");

export const defaultHeadTrackingState: HeadTrackingState = {
    yaw: 0,
    pitch: 0,
};

const handleReceiveHeadTracking = (data: any) => {
    try {
        let h = defaultHeadTrackingState;
        h.yaw = JSON.parse(data)[0];
        h.pitch = JSON.parse(data)[1];
        return h;
    } catch (err) {
        console.error("Failed to parse HeadTrackingIPC!", err);
    }

    return defaultHeadTrackingState;
};

export const getHeadTracking = async () => {
    return handleReceiveHeadTracking(await getHeadTrackingNative());
};