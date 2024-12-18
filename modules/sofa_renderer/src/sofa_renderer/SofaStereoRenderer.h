#pragma once

#include "SofaFilter.h"
#include "SofaRenderer.h"
#include "model/ParameterTree.h"
#include "udp_receiver/UDPReceiver.h"

#include <juce_core/juce_core.h>
#include <juce_dsp/juce_dsp.h>

class SofaStereoRenderer : public juce::dsp::ProcessorBase
{
public:
    SofaStereoRenderer (ParameterTree & parameter_tree, UDPReceiver & udp_receiver);
    ~SofaStereoRenderer ();

    void prepare (const juce::dsp::ProcessSpec & spec);
    void reset ();
    void process (const juce::dsp::ProcessContextReplacing<float> & replacing) override;

private:
    float sample_rate_;

    std::array<SofaRenderer, 2> sofa_renderers_;
    SofaFilter sofa_filter_ {SofaFilter::OpenOptions {
        .hrtf_path = std::filesystem::path (
            "/Users/micahstrange/StereoBinauraliser/modules/sofa_renderer/src/my_hrtf/MIT_KEMAR_normal_pinna.sofa"),
        .sample_rate = 48000,
    }};
    std::array<juce::AudioBuffer<float>, 2> hrir_buffers_;
    std::array<float, 2> left_delays_;
    std::array<float, 2> right_delays_;

    juce::AudioBuffer<float> renderer_input_buffer_;
    juce::AudioBuffer<float> renderer_output_buffer_;

    static constexpr std::array<SofaFilter::SphericalCoordinates, 2> kSphericalCoordinates {
        SofaFilter::SphericalCoordinates {.azimuth_degrees = -45.f, .elevation_degrees = 0.f},
        SofaFilter::SphericalCoordinates {.azimuth_degrees = 45.f, .elevation_degrees = 0.f}

    };

    ParameterTree parameter_tree_;

    struct SavedParams
    {
        float speaker_position_parameter;
        float speaker_width_parameter;
    };

    SavedParams saved_params_;

    UDPReceiver & udp_receiver_;
};
