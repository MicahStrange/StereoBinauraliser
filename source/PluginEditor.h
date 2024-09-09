#pragma once

#include "BinaryData.h"
#include "PluginProcessor.h"
#include "melatonin_inspector/melatonin_inspector.h"

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
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PluginProcessor & processorRef;
    std::unique_ptr<melatonin::Inspector> inspector;
    juce::TextButton inspectButton {"Inspect the UI"};

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

    SinglePageBrowser web_browser_component_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
