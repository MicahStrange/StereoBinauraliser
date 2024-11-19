import {mapFrom01Linear, mapTo01Linear} from "@dsp-ts/math";
import {FC, useState} from "react";

import {KnobBase} from "@/components/knob/knob_base";
import {controlParameterIndexAnnotation} from "@/hooks/use_control_parameter_index_updater";
import {useSlider} from "@/hooks/use_slider";
import {TVKnobBase} from "@/components/tv_knob/tv_knob_base";
import {Button} from "@/components/ui/button";
import {ToggleButton} from "@/components/toggle_button/ToggleButton";


const Toggle: FC<{
    identifier: string;

}> = ({
          identifier
      }) => {
    const {
        properties: properties,
        valueScaled: valueScaled,
        handleChange: handleChange,
        changeCommitted: changeCommitted,
        mouseDown: mouseDown,
        toNormalised: toNormalised,
    } = useSlider(identifier)
    // const stepFn = (valueRaw: number) => {
    //     return posProperties.interval;
    // };
    // const stepFnLarger = (valueRaw: number) => {
    //     return posProperties.interval * 10;
    // };

    const toggleMode = () => {
        if (valueScaled < 0.5) {
            handleChange(1);
        } else {
            handleChange(0);
        }
    }
    // const setValueRaw = (valueRaw: number) => {
    //
    //     if (posEnabled) {
    //         let value = valueRaw
    //         if (value === 360) {
    //             value = 0; // Wrap around when it hits 360
    //         } else if (value === 0) {
    //             value = 360; // Wrap around when it hits 0
    //         }
    //         posHandleChange(posToNormalised(value));
    //     } else {
    //         widthHandleChange(widthToNormalised(valueRaw));
    //     }
    //
    // };

    // const rawValueCommit = (valueRaw: number) => {
    //
    //     if (posEnabled) {
    //         let value = valueRaw
    //         if (value === 360) {
    //             value = 0; // Wrap around when it hits 360
    //         } else if (value === 0) {
    //             value = 360; // Wrap around when it hits 0
    //         }
    //         posChangeCommitted(posToNormalised(valueRaw));
    //     } else {
    //         widthChangeCommitted(widthToNormalised(valueRaw));
    //     }
    // };

    const resetToDefault = () => {
        mouseDown();
        changeCommitted(properties.defaultValue);
    };

    // const valueRawRoundFn = (value: number) => value;
    //
    // const valueRawDisplayFn = (valueRaw: number): string => {
    //     const label = posProperties.label;
    //
    //     let precision = 1;
    //     if (label == "%" || label == "ms") precision = 0;
    //
    //     return `${valueRaw.toFixed(precision)}${posProperties.label}`;
    // };


    return (
        <div className={"w-full  h-full"}>
            <ToggleButton onClick={toggleMode} leftText={"Binaural"} rightText={"Stereo"}/>
        </div>
    );
};

export {Toggle};
