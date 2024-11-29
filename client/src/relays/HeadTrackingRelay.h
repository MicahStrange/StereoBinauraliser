#pragma once

#include "udp_receiver/UDPReceiver.h"

#include <juce_gui_extra/juce_gui_extra.h>

class HeadTrackingRelay : public OptionsBuilder<juce::WebBrowserComponent::Options>
{
public:
    HeadTrackingRelay (UDPReceiver & UDPReceiver);
    ~HeadTrackingRelay () override = default;

    juce::WebBrowserComponent::Options
    buildOptions (const juce::WebBrowserComponent::Options & initialOptions) override;

private:
    UDPReceiver & udp_receiver_;
};
