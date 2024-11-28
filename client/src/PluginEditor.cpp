#include "PluginEditor.h"

const juce::String PluginEditor::kLocalDevServerAddress = "http://localhost:5173/";

bool PluginEditor::SinglePageBrowser::pageAboutToLoad (const juce::String & newURL)
{
    return newURL == PluginEditor::kLocalDevServerAddress || newURL == getResourceProviderRoot ();
}

static juce::File GetAssetsDirectory ()
{
#if JUCE_MAC
    auto assets_dir =
        juce::File::getSpecialLocation (juce::File::SpecialLocationType::currentApplicationFile)
            .getChildFile ("Contents/Resources/dist");
#elif JUCE_WINDOWS
    auto assets_dir = juce::File::getSpecialLocation (
                          juce::File::SpecialLocationType::
                              currentExecutableFile) // Plugin.vst3/Contents/<arch>/Plugin.vst3
                          .getParentDirectory ()     // Plugin.vst3/Contents/<arch>/
                          .getParentDirectory ()     // Plugin.vst3/Contents/
                          .getChildFile ("Resources/dist");
#else
    #error "We only support Mac and Windows here yet."
#endif

    return assets_dir;
}

static const char * GetMimeForExtension (const juce::String & extension)
{
    static const std::unordered_map<juce::String, const char *> mime_map = {
        {{".htm"}, "text/html"},
        {{".html"}, "text/html"},
        {{".txt"}, "text/plain"},
        {{".jpg"}, "image/jpeg"},
        {{".jpeg"}, "image/jpeg"},
        {{".svg"}, "image/svg+xml"},
        {{".ico"}, "image/vnd.microsoft.icon"},
        {{".json"}, "application/json"},
        {{".png"}, "image/png"},
        {{".css"}, "text/css"},
        {{".map"}, "application/json"},
        {{".js"}, "text/javascript"},
        {{".woff2"}, "font/woff2"}};

    if (const auto it = mime_map.find (extension.toLowerCase ()); it != mime_map.end ())
        return it->second;

    jassertfalse;
    return "";
}

static auto StreamToVector (juce::InputStream & stream)
{
    std::vector<std::byte> result ((size_t) stream.getTotalLength ());
    stream.setPosition (0);
    [[maybe_unused]] const auto kBytesRead = stream.read (result.data (), result.size ());
    jassert (kBytesRead == (ssize_t) result.size ());
    return result;
}

std::optional<juce::WebBrowserComponent::Resource>
PluginEditor::GetResource (const juce::String & url)
{
    auto rel_path = "." + (url == "/" ? "/index.html" : url);
    auto asset_file = asset_directory_.getChildFile (rel_path);

    if (! asset_file.existsAsFile ())
        return std::nullopt;

    auto file_stream = asset_file.createInputStream ();
    return juce::WebBrowserComponent::Resource {
        StreamToVector (*file_stream), GetMimeForExtension (asset_file.getFileExtension ())};
}

PluginEditor::PluginEditor (PluginProcessor & p)
    : AudioProcessorEditor (&p)
    , processorRef (p)
    , parameter_relay_ (web_browser_component_)
    , head_tracking_relay_ (p.udp_receiver_)
    , web_browser_component_ (kBaseWebOptions.withOptionsFrom (parameter_relay_)
                                  .withOptionsFrom (head_tracking_relay_)
                                  .withOptionsFrom (resize_relay_))
    , parameter_attachments_ (parameter_relay_, p.parameter_tree_state_)
{
    juce::ignoreUnused (processorRef);
    asset_directory_ = GetAssetsDirectory ();

    setSize (600, 800);
    setResizable (true, true);
    setResizeLimits (kWindowMinimumWidth,
                     kWindowMinimumHeight,
                     kWindowMaxWidth,
                     static_cast<int> (kWindowMaxWidth * kPreferredAspectRatio));
    resize_relay_.Setup (this, getConstrainer ());

#if DEV_LOCALHOST
    web_browser_component_.goToURL (kLocalDevServerAddress);
#else
    web_browser_component_.goToURL (juce::WebBrowserComponent::getResourceProviderRoot ());
#endif

    addAndMakeVisible (web_browser_component_);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
}

PluginEditor::~PluginEditor ()
{
}

void PluginEditor::resized ()
{
    web_browser_component_.setBounds (getLocalBounds ());
}
