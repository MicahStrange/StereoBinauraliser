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
            DBG ("Listening on UDP port " + juce::String (port));

            while (! threadShouldExit ())
            {
                char buffer [110];
                int bytesReceived = socket.read (buffer, sizeof (buffer), false);

                if (bytesReceived > 0)
                {
                    // Successfully received data
                    DBG ("Received " + juce::String (bytesReceived));

                    // Print the received data (optional)
                    juce::String receivedMessage (buffer, bytesReceived);
                    auto tokens = juce::StringArray::fromTokens (receivedMessage, ",", "");

                    head_position_.yaw = static_cast<float> (tokens [0].getFloatValue ());
                    head_position_.pitch = static_cast<float> (tokens [1].getFloatValue ());
                    head_position_.roll = static_cast<float> (tokens [2].getFloatValue ());
                    DBG ("Message: " + receivedMessage);
                    //                    DBG ("yaw:" + juce::String (head_position_.yaw));
                    //                    DBG ("pitch:" + juce::String (head_position_.pitch));
                    //                    DBG ("roll:" + juce::String (head_position_.roll));
                }
                else
                {
                    DBG ("no message");
                }
                wait (10);
            }
        }
        else
        {
            DBG ("Failed to bind socket to port " + juce::String (port));
        }
    }
    HeadPosition head_position_;

private:
};