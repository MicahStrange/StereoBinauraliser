#include "PluginEditor.h"

const juce::String PluginEditor::kLocalDevServerAddress = "http://localhost:5173/";

bool PluginEditor::SinglePageBrowser::pageAboutToLoad (const juce::String & newURL)
{
    return newURL == PluginEditor::kLocalDevServerAddress || newURL == getResourceProviderRoot ();
}

std::vector<std::byte> StreamToVector (juce::InputStream & stream)
{
    // Workaround to make ssize_t work cross-platform.
    using namespace juce;
    const auto sizeInBytes = static_cast<size_t> (stream.getTotalLength ());
    std::vector<std::byte> result (sizeInBytes);
    stream.setPosition (0);
    [[maybe_unused]] const auto bytesRead = stream.read (result.data (), result.size ());
    jassert (bytesRead == static_cast<ssize_t> (sizeInBytes));
    return result;
}

static const char * GetMimeForExtension (const juce::String & extension)
{
    static const std::unordered_map<juce::String, const char *> mimeMap = {
        {{"htm"}, "text/html"},
        {{"html"}, "text/html"},
        {{"txt"}, "text/plain"},
        {{"jpg"}, "image/jpeg"},
        {{"jpeg"}, "image/jpeg"},
        {{"svg"}, "image/svg+xml"},
        {{"ico"}, "image/vnd.microsoft.icon"},
        {{"json"}, "application/json"},
        {{"png"}, "image/png"},
        {{"css"}, "text/css"},
        {{"map"}, "application/json"},
        {{"js"}, "text/javascript"},
        {{"woff2"}, "font/woff2"}};

    if (const auto it = mimeMap.find (extension.toLowerCase ()); it != mimeMap.end ())
        return it->second;

    jassertfalse;
    return "";
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

std::optional<juce::WebBrowserComponent::Resource>
PluginEditor::GetResource (const juce::String & url)
{
    std::cout << "ResourceProvider called with " << url << std::endl;

    static const auto resourceFilesRoot =
        juce::File::getSpecialLocation (juce::File::SpecialLocationType::currentApplicationFile)
            .getParentDirectory ()
            .getParentDirectory ()
            .getChildFile ("public");

    [[maybe_unused]] static auto printRootOnce = []
    {
        std::cout << "Resource files root is " << resourceFilesRoot.getFullPathName () << std::endl;
        return true;
    }();

    const auto resourceToRetrieve =
        url == "/" ? "index.html" : url.fromFirstOccurrenceOf ("/", false, false);

    const auto resource = resourceFilesRoot.getChildFile (resourceToRetrieve).createInputStream ();
    if (resource)
    {
        const auto extension = resourceToRetrieve.fromLastOccurrenceOf (".", false, false);
        return juce::WebBrowserComponent::Resource {StreamToVector (*resource),
                                                    GetMimeForExtension (extension)};
    }

    return std::nullopt;
}

PluginEditor::PluginEditor (PluginProcessor & p)
    : AudioProcessorEditor (&p)
    , processorRef (p)
    , web_browser_component_ (kBaseWebOptions)
{
    juce::ignoreUnused (processorRef);
    asset_directory_ = GetAssetsDirectory ();

    setSize (400, 300);

    // #if DEV_LOCALHOST
    //     web_browser_component_.goToURL (kLocalDevServerAddress);
    // #else
    //     web_browser_component_.goToURL (juce::WebBrowserComponent::getResourceProviderRoot ());
    // #endif
    web_browser_component_.goToURL (kLocalDevServerAddress);
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
