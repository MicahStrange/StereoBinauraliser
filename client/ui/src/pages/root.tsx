import {Home, Loader, Settings, Target} from "lucide-react";
import React, {FC, ReactNode, useState} from "react";
import {Knob} from "@/components/knob";
import {Parameters} from "@/lib/parameters";
import {useControlParameterIndexUpdater} from "@/hooks/use_control_parameter_index_updater";
import {TVKnob} from "@/components/tv_knob";
import {Toggle} from "@/components/toggle_button/index";
import {ResizeHandle} from "@/components/resize_handle";
import {useHeadTracking} from "@/hooks/use_headtracking";
import HeadTrackingPanel from "@/pages/head_tracking_panel";


const Root = () => {

    useControlParameterIndexUpdater();
    return (
        <div
            className=" flex flex-col h-screen relative select-none justify-around pt-2 pb-2"
            style={{
                background: 'var(--background)',
                color: 'var(--text)'
            }}
        >
            <div className="w-full ml-0.5  h-[20%] flex flex-col justify-between px-2">
                <div className={"flex flex-row items-center pb-2"}>
                    < Knob identifier={Parameters.Input} showMidpointIndicator={false}/>
                    <h1 className={"text-6xl font-bold text-center flex-grow items-center"}>
                        STEREO BINAURALISER
                    </h1>
                </div>
                <Toggle identifier={Parameters.Binaural}/>
            </div>
            <div className="w-full  ml-0.5 h-[60%]">
                <TVKnob posIdentifier={Parameters.SpeakerPosition} widthIdentifier={Parameters.SpeakerWidth}/>
            </div>
            <div className={" w-full h-[20%] p-2"}>
                <HeadTrackingPanel/>
            </div>
            <ResizeHandle/>
        </div>
    )
        ;
};

export default () => (

    <Root/>


);
