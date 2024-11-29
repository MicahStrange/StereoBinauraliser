#pragma once

#include "model/ParameterTree.h"

#include <juce_gui_extra/juce_gui_extra.h>

class ParameterAttachments;

class ParameterRelay : public OptionsBuilder<juce::WebBrowserComponent::Options>
{
public:
    friend class ParameterAttachments;

    explicit ParameterRelay (juce::WebBrowserComponent & web_browser_component);
    ~ParameterRelay () override = default;

    juce::WebBrowserComponent::Options
    buildOptions (const juce::WebBrowserComponent::Options & initialOptions) override;

private:
    juce::WebBrowserComponent & web_browser_component_;

    juce::WebSliderRelay input_gain_relay_ {web_browser_component_,
                                            ParameterTree::kInputGainParameterId};
    juce::WebSliderRelay binaural_relay_ {web_browser_component_,
                                          ParameterTree::kBinauralParameterId};
    juce::WebSliderRelay speaker_position_relay_ {web_browser_component_,
                                                  ParameterTree::kSpeakerPositionParameterId};
    juce::WebSliderRelay speaker_width_relay_ {web_browser_component_,
                                               ParameterTree::kSpeakerWidthParameterId};
};

/**
 * This is annoying that this is needed. It appears that the destruction order of this needs to
 * come before the web browser component. However relays should appear before the construction
 * of the browser.
 */
class ParameterAttachments
{
public:
    ParameterAttachments (ParameterRelay & parameter_relay,
                          juce::AudioProcessorValueTreeState & parameter_tree);

private:
    juce::WebSliderParameterAttachment input_gain_attachment_;
    juce::WebSliderParameterAttachment binaural_attachment_;
    juce::WebSliderParameterAttachment speaker_position_attachment_;
    juce::WebSliderParameterAttachment speaker_width_attachment_;
};
