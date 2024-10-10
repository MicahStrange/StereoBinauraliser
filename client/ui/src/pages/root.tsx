import {Home, Loader, Settings, Target} from "lucide-react";
import React, {FC, ReactNode} from "react";
import {Knob} from "@/components/knob";
import {Parameters} from "@/lib/parameters";
import {useControlParameterIndexUpdater} from "@/hooks/use_control_parameter_index_updater";

const Root = () => {

    useControlParameterIndexUpdater();
    return (
        <div
            className="flex flex-row h-screen bg-background relative select-none"
            data-theme="dark"
        >
            <div className="w-full ml-0.5">
                <Knob identifier={Parameters.Input}/>
                <Knob identifier={Parameters.FilterSizeLow}/>
                <Knob identifier={Parameters.FilterSizeHigh}/>
                <Knob identifier={Parameters.Binaural}/>
            </div>
        </div>
    );
};

export default () => (

    <Root/>


);
