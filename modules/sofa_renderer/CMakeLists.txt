add_module(
        MODULE sofa_renderer
        SOURCES src/sofa_renderer/SofaFilter.h
        src/sofa_renderer/SofaFilter.cpp
        src/sofa_renderer/SofaRenderer.h
        src/sofa_renderer/SofaRenderer.cpp
        src/sofa_renderer/BufferTransfer.h
        src/sofa_renderer/SofaStereoRenderer.h
        src/sofa_renderer/SofaStereoRenderer.cpp
        src/sofa_renderer/TimeVaryingConvolver.h
        src/sofa_renderer/TimeVaryingConvolver.cpp
        DEPS juce::juce_core juce::juce_dsp model zones_convolver udp_receiver
)