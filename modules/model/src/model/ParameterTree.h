#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

class ParameterTree
{
public:
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    juce::AudioParameterFloat * input_gain_parameter;
    juce::AudioParameterFloat * filter_size_low_parameter;
    juce::AudioParameterFloat * filter_size_high_parameter;
    juce::AudioParameterFloat * binaural_parameter;
    juce::AudioParameterFloat * speaker_position_parameter;
    juce::AudioParameterFloat * speaker_width_parameter;

    static ParameterTree
    CreateParameterTree (juce::AudioProcessorValueTreeState::ParameterLayout & layout);

    static const juce::Identifier kParameterTreeIdentifier;
    static const juce::String kInputGainParameterId;
    static const juce::String kFilterSizeLowParameterId;
    static const juce::String kFilterSizeHighParameterId;
    static const juce::String kBinauralParameterId;
    static const juce::String kSpeakerPositionParameterId;
    static const juce::String kSpeakerWidthParameterId;
};
