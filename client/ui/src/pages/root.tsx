import {Home, Loader, Settings, Target} from "lucide-react";
import React, {FC, ReactNode} from "react";
import {Knob} from "@/components/knob";
import {Parameters} from "@/lib/parameters";
import {useControlParameterIndexUpdater} from "@/hooks/use_control_parameter_index_updater";
import {TVKnob} from "@/components/tv_knob";
import {parameters} from "@storybook/addon-interactions/preview";
import {Toggle} from "@/components/toggle_button/index";
import {ResizeHandle} from "@/components/resize_handle";

const Root = () => {

    useControlParameterIndexUpdater();
    return (
        <div
            className=" flex flex-col h-screen relative select-none justify-around "
            style={{
                background: 'var(--background)',
                color: 'var(--text)'
            }}

        >
            <div className="w-full ml-0.5  h-[30%] flex flex-col justify-between px-2">
                <Knob identifier={Parameters.Input} showMidpointIndicator={false}/>
                <Toggle identifier={Parameters.Binaural}/>
            </div>
            <div className="w-full  ml-0.5 h-[70%]">
                <TVKnob posIdentifier={Parameters.SpeakerPosition} widthIdentifier={Parameters.SpeakerWidth}/>
            </div>
            <ResizeHandle/>
        </div>
    );
};

export default () => (

    <Root/>


);
