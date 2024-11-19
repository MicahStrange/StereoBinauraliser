import {Home, Loader, Settings, Target} from "lucide-react";
import React, {FC, ReactNode} from "react";
import {Knob} from "@/components/knob";
import {Parameters} from "@/lib/parameters";
import {useControlParameterIndexUpdater} from "@/hooks/use_control_parameter_index_updater";
import {TVKnob} from "@/components/tv_knob";
import {parameters} from "@storybook/addon-interactions/preview";
import {Toggle} from "@/components/toggle_button/index";

const Root = () => {

    useControlParameterIndexUpdater();
    return (
        <div
            className="fill-background flex flex-col h-screen relative select-none"
        >
            <div className="w-full ml-0.5">
                <Knob identifier={Parameters.Input} showMidpointIndicator={false}/>
                <Toggle identifier={Parameters.Binaural}/>
                {/*<Knob identifier={Parameters.SpeakerPosition}/>*/}
                {/*<Knob identifier={Parameters.SpeakerWidth}/>*/}

            </div>
            <div className="w-full ml-0.5">
                <TVKnob posIdentifier={Parameters.SpeakerPosition} widthIdentifier={Parameters.SpeakerWidth}/>
            </div>
        </div>
    );
};

export default () => (

    <Root/>


);
