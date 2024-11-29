import {mapFrom01Linear, mapTo01Linear} from "@dsp-ts/math";
import {FC, useState} from "react";

import {KnobBase} from "@/components/knob/knob_base";
import {controlParameterIndexAnnotation} from "@/hooks/use_control_parameter_index_updater";
import {useSlider} from "@/hooks/use_slider";
import {TVKnobBase} from "@/components/tv_knob/tv_knob_base";
import {Button} from "@/components/ui/button";
import {ToggleButton} from "@/components/toggle_button/ToggleButton";

const mapTo01Skewed = (
    x: number,
    min: number,
    max: number,
    skew: number,
): number => {
    const linearMapped = mapTo01Linear(x, min, max);
    return Math.pow(linearMapped, skew);
};

const mapFrom01Skewed = (
    x: number,
    min: number,
    max: number,
    skew: number,
): number => {
    let skewedValue = Math.pow(x, 1 / skew);
    return mapFrom01Linear(skewedValue, min, max);
};


const TVKnob: FC<{
    posIdentifier: string;
    widthIdentifier: string;
    showMidpointIndicator?: boolean;
    trackFromMidpoint?: boolean;
}> = ({
          posIdentifier,
          widthIdentifier,
          showMidpointIndicator = true,
          trackFromMidpoint = false,
      }) => {
    const {
        properties: posProperties,
        valueScaled: posValueScaled,
        handleChange: posHandleChange,
        changeCommitted: posChangeCommitted,
        mouseDown: posMouseDown,
        toNormalised: posToNormalised,
    } = useSlider(posIdentifier);
    const {
        properties: widthProperties,
        valueScaled: widthValueScaled,
        handleChange: widthHandleChange,
        changeCommitted: widthChangeCommitted,
        mouseDown: widthMouseDown,
        toNormalised: widthToNormalised,
    } = useSlider(widthIdentifier);
    const [posEnabled, setMode] = useState(true);

    const toggleMode = () => setMode(!posEnabled);
    const stepFn = (valueRaw: number) => {
        return posProperties.interval;
    };

    const stepFnLarger = (valueRaw: number) => {
        return posProperties.interval * 10;
    };

    const setValueRaw = (valueRaw: number) => {

        if (posEnabled) {
            let value = valueRaw
            if (value === 360) {
                value = 0; // Wrap around when it hits 360
            } else if (value === 0) {
                value = 360; // Wrap around when it hits 0
            }
            posHandleChange(posToNormalised(value));
        } else {
            widthHandleChange(widthToNormalised(valueRaw));
        }

    };

    const rawValueCommit = (valueRaw: number) => {

        if (posEnabled) {
            let value = valueRaw
            if (value === 360) {
                value = 0; // Wrap around when it hits 360
            } else if (value === 0) {
                value = 360; // Wrap around when it hits 0
            }
            posChangeCommitted(posToNormalised(valueRaw));
        } else {
            widthChangeCommitted(widthToNormalised(valueRaw));
        }
    };

    const resetToDefault = () => {
        posMouseDown();
        widthMouseDown();
        posChangeCommitted(posProperties.defaultValue);
        widthChangeCommitted(widthProperties.defaultValue);
    };

    const valueRawRoundFn = (value: number) => value;

    const valueRawDisplayFn = (valueRaw: number): string => {
        const label = posProperties.label;

        let precision = 1;
        if (label == "%" || label == "ms") precision = 0;

        return `${valueRaw.toFixed(precision)}${posProperties.label}`;
    };

    const mapTo01 = (x: number, min: number, max: number) =>
        mapTo01Skewed(x, min, max, posProperties.skew);

    const mapFrom01 = (x: number, min: number, max: number) =>
        mapFrom01Skewed(x, min, max, posProperties.skew);


    return (
        <div className={"w-full  h-full flex flex-col p-2 py-2 justify-between  mt-0.5"}>
            <div className={"py-2"}>
                <ToggleButton onClick={toggleMode} leftText={"Position"} rightText={"Width"}/>
            </div>
            <TVKnobBase
                valueMin={0} valueMax={0} onRawValueCommit={rawValueCommit}
                posEnabled={posEnabled}
                onMouseDown={posMouseDown}
                name={posProperties.name}
                valuesRaw={[posValueScaled, widthValueScaled]}
                setPosValueRaw={setValueRaw}
                setWidthValueRaw={setValueRaw}
                stepFn={stepFn}
                stepLargerFn={stepFnLarger}
                posValueMin={posProperties.start}
                posValueMax={posProperties.end}
                widthValueMin={widthProperties.start}
                widthValueMax={widthProperties.end}
                valueRawRoundFn={valueRawRoundFn}
                valueRawDisplayFn={valueRawDisplayFn}
                mapTo01={mapTo01}
                mapFrom01={mapFrom01}
                midpoint={posProperties.defaultValue}
                showMidpointIndicator={showMidpointIndicator}
                onResetToDefault={resetToDefault}
                trackFromMidpoint={true}
                {...{
                    [controlParameterIndexAnnotation]: posProperties.parameterIndex,
                }}            />
        </div>
    );
};

export {TVKnob};
