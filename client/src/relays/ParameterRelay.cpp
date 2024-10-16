#include "ParameterRelay.h"

ParameterRelay::ParameterRelay (juce::WebBrowserComponent & web_browser_component)
    : web_browser_component_ (web_browser_component)
{
}

juce::WebBrowserComponent::Options
ParameterRelay::buildOptions (const juce::WebBrowserComponent::Options & initialOptions)
{
    return initialOptions.withOptionsFrom (input_gain_relay_)
        .withOptionsFrom (filter_size_low_relay_)
        .withOptionsFrom (filter_size_high_relay_)
        .withOptionsFrom (binaural_relay_)
        .withOptionsFrom (speaker_position_relay_)
        .withOptionsFrom (speaker_width_relay_);
}

ParameterAttachments::ParameterAttachments (ParameterRelay & parameter_relay,
                                            juce::AudioProcessorValueTreeState & parameter_tree)
    : input_gain_attachment_ (*parameter_tree.getParameter (ParameterTree::kInputGainParameterId),
                              parameter_relay.input_gain_relay_)
    , filter_size_low_attachment_ (
          *parameter_tree.getParameter (ParameterTree::kFilterSizeLowParameterId),
          parameter_relay.filter_size_low_relay_)
    , filter_size_high_attachment_ (
          *parameter_tree.getParameter (ParameterTree::kFilterSizeHighParameterId),
          parameter_relay.filter_size_high_relay_)
    , binaural_attachment_ (*parameter_tree.getParameter (ParameterTree::kBinauralParameterId),
                            parameter_relay.binaural_relay_)
    , speaker_position_attachment_ (
          *parameter_tree.getParameter (ParameterTree::kSpeakerPositionParameterId),
          parameter_relay.speaker_position_relay_)
    , speaker_width_attachment_ (
          *parameter_tree.getParameter (ParameterTree::kSpeakerWidthParameterId),
          parameter_relay.speaker_width_relay_)

{
}
