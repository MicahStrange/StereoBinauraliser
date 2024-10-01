#include "ParameterTree.h"

#include "ParameterUtils.h"

const juce::Identifier ParameterTree::kParameterTreeIdentifier {"zones_parameter_tree"};

const juce::String ParameterTree::kInputGainParameterId {"input_gain_parameter"};
const juce::String ParameterTree::kFilterSizeLowParameterId {"filter_size_low_parameter"};
const juce::String ParameterTree::kFilterSizeHighParameterId {"filter_size_high_parameter"};

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
        .filter_size_low_parameter = ParameterUtils::BindLayoutParameter (
            std::make_unique<juce::AudioParameterFloat> (
                juce::ParameterID {kFilterSizeLowParameterId, 1},
                "FilterSizeLow",
                ParameterUtils::WithSkewedCentre (juce::NormalisableRange<float> (0, 1000.f),
                                                  100.f),
                1.f,
                juce::AudioParameterFloatAttributes ()),
            layout),
        .filter_size_high_parameter = ParameterUtils::BindLayoutParameter (
            std::make_unique<juce::AudioParameterFloat> (
                juce::ParameterID {kFilterSizeHighParameterId, 1},
                "FilterSizeHigh",
                ParameterUtils::WithSkewedCentre (juce::NormalisableRange<float> (0, 1000.f),
                                                  100.f),
                1.f,
                juce::AudioParameterFloatAttributes ()),
            layout)};
}
