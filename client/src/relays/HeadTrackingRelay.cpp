#include "HeadTrackingRelay.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

static void to_json (json & data, const std::pair<float, float> float_pair)
{
    data = json {
        {"yaw", float_pair.first},
        {"pitch", float_pair.second},
    };
}

static void from_json (const json & data, std::pair<float, float> float_pair)
{
    data.at ("yaw").get_to (float_pair.first);
    data.at ("pitch").get_to (float_pair.second);
}

HeadTrackingRelay::HeadTrackingRelay (UDPReceiver & udp_receiver)
    : udp_receiver_ (udp_receiver)
{
}

juce::WebBrowserComponent::Options
HeadTrackingRelay::buildOptions (const juce::WebBrowserComponent::Options & initialOptions)
{
    return initialOptions.withNativeFunction ("get_head_tracking_native",
                                              [this] (auto & var, auto complete)
                                              {
                                                  JUCE_ASSERT_MESSAGE_THREAD;

                                                  auto a = udp_receiver_.GetHeadPosition ();
                                                  json data = a;
                           
                                                  complete ({data.dump ()});
                                              });
}
