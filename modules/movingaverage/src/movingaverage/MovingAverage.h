#pragma once
#include <juce_dsp/juce_dsp.h>

class MovingAverage : public juce::dsp::ProcessorBase
{
public:
    void prepare (const juce::dsp::ProcessSpec & spec) override;
    void process (const juce::dsp::ProcessContextReplacing<float> & replacing) override;
    void reset () override;

private:
    const int MaxDelaySamples = 1024;
    int filter_size = 100;

    juce::AudioBuffer<float> circ_buff_;
    std::vector<float> average_;
    int read_head_;
    int write_head_;
};
