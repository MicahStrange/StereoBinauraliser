#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

class ParameterTree
{
public:
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    juce::AudioParameterFloat * input_gain_parameter;
    juce::AudioParameterFloat * filter_size_parameter;

    static ParameterTree
    CreateParameterTree (juce::AudioProcessorValueTreeState::ParameterLayout & layout);

    static const juce::Identifier kParameterTreeIdentifier;
    static const juce::String kInputGainParameterId;
    static const juce::String kFilterSizeParameterId;
};
