import {mapFrom01Linear} from "@dsp-ts/math";
import {arc, DefaultArcObject, symbol, symbolTriangle} from "d3";
import {FC} from "react";

const angleMin = -180;
const angleMax = 180;

const arc_gen = arc();
const arc_style: DefaultArcObject = {
    innerRadius: 110,
    outerRadius: 120,
    startAngle: (angleMin / 180) * Math.PI,
    endAngle: (angleMax / 180) * Math.PI,
};

const track = arc_gen(arc_style)!;

const midpoint_indicator = symbol()
    .type(symbolTriangle)
    .size(10 * 10);

const TVKnobBaseThumb: FC<{
    posValue01: number;
    widthValue01: number;
    midpoint: number;
    showMidpointIndicator: boolean;
    trackFromMidpoint: boolean;
}> = ({posValue01, widthValue01, midpoint, showMidpointIndicator, trackFromMidpoint}) => {
    const leftValue = (posValue01 - (0.5 * widthValue01) + 0.5) % 1;
    const rightValue = (posValue01 + (0.5 * widthValue01) + 0.5) % 1;
    const midValue = (posValue01 + 0.5) % 1;
    const leftAngle = mapFrom01Linear(leftValue, angleMin, angleMax);
    const rightAngle = mapFrom01Linear(rightValue, angleMin, angleMax);
    const midpointAngle = mapFrom01Linear(midValue, angleMin, angleMax);

    const left_track_fill = arc_gen({
        ...arc_style,
        ...(trackFromMidpoint && {
            startAngle: (midpointAngle / 180) * Math.PI,
        }),
        endAngle: (leftAngle / 180) * Math.PI,
    })!;
    const right_track_fill = arc_gen({
        ...arc_style,
        ...(trackFromMidpoint && {
            startAngle: (midpointAngle / 180) * Math.PI,
        }),
        endAngle: (rightAngle / 180) * Math.PI,
    })!;

    return (
        <div className="relative w-16 h-16">
            {showMidpointIndicator && (
                <svg
                    className="w-16 h-16 absolute"
                    viewBox="-120 -120 240 240"
                    style={{
                        rotate: `${midpoint * 290 - 145}deg`,
                    }}
                >
                    <path
                        className="stroke-0 fill-primary"
                        d={midpoint_indicator()!}
                        transform="rotate(180) translate(0 115)"
                    />
                </svg>
            )}
            <svg
                className="w-12 h-12 absolute"
                viewBox="-120 -120 240 240"
                style={{
                    top: "50%",
                    left: "50%",
                    transform: "translate(-50%, -50%)",
                }}
            >
                <path className="stroke-0 fill-background" d={track}/>
                <path className="stroke-0 fill-primary" d={left_track_fill}/>
                <path className="stroke-0 fill-primary" d={right_track_fill}/>
            </svg>
            <h1 style={{color: 'green'}}>{leftValue}</h1>
            <h1 style={{color: 'green'}}>{rightValue}</h1>
            <h1 style={{color: 'green'}}>{midValue}</h1>
            <h1 style={{color: 'green'}}>{leftAngle}</h1>
            <h1 style={{color: 'green'}}>{rightAngle}</h1>
            <h1 style={{color: 'green'}}>{midpointAngle}</h1>

        </div>
    );
};

export {TVKnobBaseThumb};
