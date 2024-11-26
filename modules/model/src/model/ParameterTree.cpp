#include "ParameterTree.h"

#include "ParameterUtils.h"

const juce::Identifier ParameterTree::kParameterTreeIdentifier {"zones_parameter_tree"};

const juce::String ParameterTree::kInputGainParameterId {"input_gain_parameter"};
const juce::String ParameterTree::kBinauralParameterId {"binaural_parameter"};
const juce::String ParameterTree::kSpeakerPositionParameterId {"speaker_position_parameter"};
const juce::String ParameterTree::kSpeakerWidthParameterId {"speaker_width_parameter"};

ParameterTree
ParameterTree::CreateParameterTree (juce::AudioProcessorValueTreeState::ParameterLayout & layout)
{
    return {

        .input_gain_parameter = ParameterUtils::BindLayoutParameter (
            std::make_unique<juce::AudioParameterFloat> (
                juce::ParameterID {kInputGainParameterId, 1},
                "Input",
                ParameterUtils::WithSkewedCentre (juce::NormalisableRange<float> (-100.0f, 10.f),
                                                  -6.f),
                0.f,
                juce::AudioParameterFloatAttributes ().withLabel ("dB")),
            layout),
        .binaural_parameter = ParameterUtils::BindLayoutParameter (
            std::make_unique<juce::AudioParameterFloat> (
                juce::ParameterID {kBinauralParameterId, 1},
                "Binaural",
                ParameterUtils::WithSkewedCentre (juce::NormalisableRange<float> (0, 1.f), 0.5f),
                0.f,
                juce::AudioParameterFloatAttributes ()),
            layout),
        .speaker_position_parameter = ParameterUtils::BindLayoutParameter (
            std::make_unique<juce::AudioParameterFloat> (
                juce::ParameterID {kSpeakerPositionParameterId, 1},
                "SpeakerPosition",
                ParameterUtils::WithSkewedCentre (juce::NormalisableRange<float> (0, 360.f), 180.f),
                0.f,
                juce::AudioParameterFloatAttributes ()),
            layout),
        .speaker_width_parameter = ParameterUtils::BindLayoutParameter (
            std::make_unique<juce::AudioParameterFloat> (
                juce::ParameterID {kSpeakerWidthParameterId, 1},
                "SpeakerWidth",
                ParameterUtils::WithSkewedCentre (juce::NormalisableRange<float> (0, 180.f), 90.f),
                90.f,
                juce::AudioParameterFloatAttributes ()),
            layout)};
}
