#pragma once

#include "PluginProcessor.h"
#include "relays/HeadTrackingRelay.h"
#include "relays/ParameterRelay.h"
#include "relays/ResizeRelay.h"

#include <juce_gui_extra/juce_gui_extra.h>

//==============================================================================
class PluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor (PluginProcessor &);
    ~PluginEditor () override;

    //==============================================================================
    void resized () override;

private:
    PluginProcessor & processorRef;

    static const juce::String kLocalDevServerAddress;
    juce::File asset_directory_;
    std::optional<juce::WebBrowserComponent::Resource> GetResource (const juce::String & url);

    struct SinglePageBrowser : juce::WebBrowserComponent
    {
        using WebBrowserComponent::WebBrowserComponent;
        bool pageAboutToLoad (const juce::String & newURL) override;
    };

    const juce::WebBrowserComponent::Options kBaseWebOptions =
        juce::WebBrowserComponent::Options {}
            .withBackend (juce::WebBrowserComponent::Options::Backend::webview2)
            .withWinWebView2Options (
                juce::WebBrowserComponent::Options::WinWebView2 {}.withUserDataFolder (
                    juce::File::getSpecialLocation (
                        juce::File::SpecialLocationType::tempDirectory)))
            .withNativeIntegrationEnabled ()
            .withResourceProvider ([this] (const auto & url) { return GetResource (url); },
                                   juce::URL {kLocalDevServerAddress}.getOrigin ());

    static constexpr float kPreferredAspectRatio = 3.f / 4.f;
    static constexpr int kWindowMinimumWidth = 600;
    static constexpr int kWindowMinimumHeight = 700;
    static constexpr int kWindowMaxWidth = 1000;

    ParameterRelay parameter_relay_;
    HeadTrackingRelay head_tracking_relay_;
    ResizeRelay resize_relay_;
    SinglePageBrowser web_browser_component_;

    ParameterAttachments parameter_attachments_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
