import {mapFrom01Linear, mapTo01Linear} from "@dsp-ts/math";
import {ComponentProps, useId} from "react";
import {
    KnobHeadless,
    KnobHeadlessLabel,
    KnobHeadlessOutput,
    useKnobKeyboardControls,
} from "react-knob-headless";

import {KnobBaseThumb} from "@/components/knob/knob_base_thumb";
import {cn} from "@/lib/utils";
import {TVKnobBaseThumb} from "@/components/tv_knob/tv_knob_base_thumb";

type KnobHeadlessProps = ComponentProps<typeof KnobHeadless>;

export type TVKnobBaseProps = Pick<
    KnobHeadlessProps,
    | "valueMin"
    | "valueMax"
    | "valueRawRoundFn"
    | "valueRawDisplayFn"
    | "orientation"
    | "mapTo01"
    | "mapFrom01"
> & {
    readonly onRawValueCommit: (valueRaw: number) => void;
    readonly onMouseDown: () => void;
    readonly valuesRaw: [number, number];
    readonly setPosValueRaw: (valuesRaw: number) => void;
    readonly setWidthValueRaw: (valuesRaw: number) => void;
    readonly name: string;
    readonly posEnabled: boolean;
    readonly stepFn: (valuesRaw: number) => number;
    readonly stepLargerFn: (valuesRaw: number) => number;
    readonly midpoint: number;
    readonly showMidpointIndicator: boolean;
    readonly trackFromMidpoint: boolean;
    readonly onResetToDefault: () => void;
    readonly posValueMin: number;
    readonly widthValueMin: number;
    readonly posValueMax: number;
    readonly widthValueMax: number;
};

const TVKnobBase = ({
                        name,
                        posEnabled,
                        valuesRaw,
                        setPosValueRaw,
                        setWidthValueRaw,
                        posValueMin,
                        widthValueMin,
                        posValueMax,
                        widthValueMax,
                        valueRawRoundFn,
                        valueRawDisplayFn,
                        orientation,
                        stepFn,
                        stepLargerFn,
                        mapTo01 = mapTo01Linear,
                        mapFrom01 = mapFrom01Linear,
                        onMouseDown,
                        onRawValueCommit,
                        midpoint,
                        showMidpointIndicator,
                        trackFromMidpoint,
                        onResetToDefault,
                    }: TVKnobBaseProps) => {
    const PosknobId = useId();
    const WidthknobId = useId();
    const labelId = useId();

    const values01 = [mapTo01(valuesRaw[0], posValueMin, posValueMax), mapTo01(valuesRaw[1], widthValueMin, widthValueMax)];
    const step = stepFn(valuesRaw[0]);
    const stepLarger = stepLargerFn(valuesRaw[0]);
    const dragSensitivity = 0.006;

    const PosKeyboardControlHandlers = useKnobKeyboardControls({
        valueRaw: valuesRaw[0],
        valueMin: posValueMin,
        valueMax: posValueMax,
        step,
        stepLarger,
        onValueRawChange: setPosValueRaw,
    });

    const WidthKeyboardControlHandlers = useKnobKeyboardControls({
        valueRaw: valuesRaw[1],
        valueMin: widthValueMin,
        valueMax: widthValueMax,
        step,
        stepLarger,
        onValueRawChange: setWidthValueRaw,
    });

    return (

        <div
            className={cn(
                "w-fit flex flex-col justify-center items-center text-xs select-none"
            )}
        >
            <KnobHeadlessLabel id={labelId} className="text-primary mb-2">
                {name}
            </KnobHeadlessLabel>
            <div>
                <div>
                    {posEnabled ? (
                        <KnobHeadless
                            id={PosknobId}
                            aria-labelledby={labelId}
                            className="outline-none w-fit"
                            valueMin={posValueMin}
                            valueMax={posValueMax}
                            valueRaw={valuesRaw[0]}
                            valueRawRoundFn={valueRawRoundFn}
                            valueRawDisplayFn={valueRawDisplayFn}
                            dragSensitivity={dragSensitivity}
                            orientation={orientation}
                            mapTo01={mapTo01}
                            mapFrom01={mapFrom01}
                            onValueRawChange={setPosValueRaw}
                            onMouseDown={onMouseDown}
                            onMouseUp={() => {
                                onRawValueCommit(valuesRaw[0]);
                            }}
                            onClick={(e) => {
                                if (e.altKey) {
                                    onResetToDefault();
                                }
                            }}
                            {...PosKeyboardControlHandlers}
                        >
                            <TVKnobBaseThumb
                                posValue01={values01[0]}
                                widthValue01={values01[1]}
                                midpoint={midpoint}
                                showMidpointIndicator={showMidpointIndicator}
                                trackFromMidpoint={trackFromMidpoint}
                            />
                        </KnobHeadless>) : (

                        <KnobHeadless
                            id={WidthknobId}
                            aria-labelledby={labelId}
                            className="outline-none w-fit"
                            valueMin={widthValueMin}
                            valueMax={widthValueMax}
                            valueRaw={valuesRaw[1]}
                            valueRawRoundFn={valueRawRoundFn}
                            valueRawDisplayFn={valueRawDisplayFn}
                            dragSensitivity={dragSensitivity}
                            orientation={orientation}
                            mapTo01={mapTo01}
                            mapFrom01={mapFrom01}
                            onValueRawChange={setPosValueRaw}
                            onMouseDown={onMouseDown}
                            onMouseUp={() => {
                                onRawValueCommit(valuesRaw[1]);
                            }}
                            onClick={(e) => {
                                if (e.altKey) {
                                    onResetToDefault();
                                }
                            }}
                            {...PosKeyboardControlHandlers} >
                            <TVKnobBaseThumb
                                posValue01={values01[0]}
                                widthValue01={values01[1]}
                                midpoint={midpoint}
                                showMidpointIndicator={showMidpointIndicator}
                                trackFromMidpoint={trackFromMidpoint}
                            />
                        </KnobHeadless>

                    )}
                </div>
            </div>
            <KnobHeadlessOutput htmlFor={PosknobId} className="mt-0.5">
                {valueRawDisplayFn(valuesRaw[0])}
            </KnobHeadlessOutput>
            <KnobHeadlessOutput htmlFor={WidthknobId} className="mt-0.5">
                {valueRawDisplayFn(valuesRaw[1])}
            </KnobHeadlessOutput>

        </div>
    )
        ;
};

export {
    TVKnobBase
};
