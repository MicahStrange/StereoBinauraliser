#pragma once
#include "juce_core/juce_core.h"

struct HeadPosition
{
    std::atomic<float> yaw = 0.f;
    std::atomic<float> pitch = 0.f;
    std::atomic<float> roll = 0.f;
};

class UDPReceiver : public juce::Thread
{
public:
    UDPReceiver ()
        : Thread ("UDPReceiverThread")
    {
    }

    void run () override
    {
        juce::DatagramSocket socket;
        int port = 5555;

        if (socket.bindToPort (port))
        {
            while (! threadShouldExit ())
            {
                char buffer [110];
                int bytesReceived = socket.read (buffer, sizeof (buffer), false);

                if (bytesReceived > 0)
                {
                    juce::String receivedMessage (buffer, bytesReceived);
                    auto tokens = juce::StringArray::fromTokens (receivedMessage, ",", "");

                    head_position_.yaw = static_cast<float> (tokens [0].getFloatValue ());
                    head_position_.pitch = static_cast<float> (tokens [1].getFloatValue ());
                    head_position_.roll = static_cast<float> (tokens [2].getFloatValue ());
                }
                else
                {
                    empty_call_counter_ += 1;
                    if (empty_call_counter_ >= kMaxEmptyCallsBeforeReset)
                    {
                        head_position_.yaw = 0.f;
                        head_position_.pitch = 0.f;
                        head_position_.roll = 0.f;

                        empty_call_counter_ = 0;
                    }
                }
                wait (20);
            }
        }
        else
        {
            DBG ("Failed to bind socket to port " + juce::String (port));
            empty_call_counter_ = 0;
        }
    }

    std::pair<float, float> GetHeadPosition ()
    {
        return {head_position_.yaw.load (), head_position_.pitch.load ()};
    }

private:
    HeadPosition head_position_;
    const int kMaxEmptyCallsBeforeReset = 100;
    int empty_call_counter_ = 0;
};