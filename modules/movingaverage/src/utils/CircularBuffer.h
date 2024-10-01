#pragma once
#include <juce_dsp/juce_dsp.h>

class CircularBuffer
{
public:
    CircularBuffer (juce::AudioBuffer<float> buffer);

    void add (juce::dsp::AudioBlock<float> new_samples);
    void addSample (float sample_to_add);
    float getSample (int delay, bool progress = true);

    void clear ();

private:
    juce::dsp::AudioBlock<float> circ_block_;
    int write_head_;
};
