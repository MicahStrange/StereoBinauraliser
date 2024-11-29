add_module(
        MODULE udp_receiver
        SOURCES src/udp_receiver/UDPReceiver.cpp
        src/udp_receiver/UDPReceiver.h

        DEPS juce::juce_core
)