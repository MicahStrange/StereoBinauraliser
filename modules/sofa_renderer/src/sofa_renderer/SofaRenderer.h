#pragma once

#include "BufferTransfer.h"
#include "TimeVaryingConvolver.h"

#include <juce_core/juce_core.h>
#include <juce_dsp/juce_dsp.h>
#include <zones_convolver/zones_convolver.h>

class SofaRenderer
{
public:
    SofaRenderer ();

    void SetFilter (juce::dsp::AudioBlock<float> hrir,
                    float left_delay,
                    float right_delay,
                    float sample_rate);

    void Prepare (const juce::dsp::ProcessSpec & spec, const int max_ir_num_samples);
    void Process (const juce::dsp::ProcessContextNonReplacing<float> & processContext,
                  const std::optional<juce::dsp::AudioBlock<float>> & ir);
    void reset ();

private:
    static constexpr int kLeftChannel = 0;
    static constexpr int kRightChannel = 1;

    float sample_rate_;

    BufferTransfer buffer_transfer_;

    juce::ThreadPool thread_pool_;
    //    zones::ConvolutionEngine convolver_ {thread_pool_};
    TimeVaryingConvolver convolver_;

    using SofaDelayLine = juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::None>;
    std::array<SofaDelayLine, 2> delay_lines_;
    float left_delay_;
    float right_delay_;

    juce::AudioBuffer<float> hrir_buffer_;
};