import {mapFrom01Linear} from "@dsp-ts/math";
import {arc, DefaultArcObject, symbol, symbolTriangle} from "d3";
import {FC} from "react";

const angleMin = -180;
const angleMax = 180;
const innerRadius = 40;
const outerRadius = 50;

const arc_gen = arc().cornerRadius(0);
const arc_style: DefaultArcObject = {
    innerRadius: innerRadius,
    outerRadius: outerRadius,
    startAngle: (-180 / 180) * Math.PI,
    endAngle: (180 / 180) * Math.PI,
};

const track = arc_gen(arc_style)!;

const midpoint_indicator = symbol()
    .type(symbolTriangle)
    .size(10 * 10);

function getCoordinates(angle: number) {
    return {
        x: innerRadius * Math.cos((angle / 180) * Math.PI),
        y: innerRadius * Math.sin((angle / 180) * Math.PI),
    };
}

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
    const adjustedrightAngle = rightAngle < leftAngle ? rightAngle + 360 : rightAngle;


    // const left_track_fill = arc_gen({
    //     ...arc_style,
    //     ...(trackFromMidpoint && {
    //         startAngle: (midpointAngle / 180) * Math.PI,
    //     }),
    //     endAngle: (leftAngle / 180) * Math.PI,
    // })!;
    // const right_track_fill = arc_gen({
    //     ...arc_style,
    //     ...(trackFromMidpoint && {
    //         startAngle: (midpointAngle / 180) * Math.PI,
    //     }),
    //     endAngle: (rightAngle / 180) * Math.PI,
    // })!;

    const track_fill = arc_gen({
        innerRadius: innerRadius,
        outerRadius: outerRadius,
        startAngle: (leftAngle / 180) * Math.PI,
        endAngle: (adjustedrightAngle / 180) * Math.PI,
    })!;

    const left_point_fill = arc_gen({
        innerRadius: innerRadius,
        outerRadius: outerRadius,
        startAngle: (leftAngle / 180) * Math.PI,
        endAngle: ((leftAngle + 2) / 180) * Math.PI,
    })!;
    const right_point_fill = arc_gen({
        innerRadius: innerRadius,
        outerRadius: outerRadius,
        startAngle: ((adjustedrightAngle - 2) / 180) * Math.PI,
        endAngle: (adjustedrightAngle / 180) * Math.PI,
    })!;

    const drawTrackFill =
        () => {
            return (
                <>

                    {widthValue01 == 1 ? <path className="stroke-0 fill-primary" d={track}/> :
                        <path className="stroke-0 fill-primary" d={track_fill}/>}
                    <path className="stroke-0 fill-secondary"
                          d={left_point_fill}/>
                    <path className="stroke-0 fill-secondary"
                          d={right_point_fill}/>
                </>
            );
        }


    return (
        <div className=" relative w-full h-full">
            <svg
                className="w-full h-full relative "
                viewBox="-100 -100 100 100"
            >
                <g transform="translate(-50, -50)">
                    <path className="stroke-0 fill-ring" d={track}/>
                    {drawTrackFill()}
                </g>

            </svg>


        </div>
    );
};

export {
    TVKnobBaseThumb
};
