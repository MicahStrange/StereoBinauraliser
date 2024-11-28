//
// Created by Micah Strange on 04/10/2024.
//

#include "SofaStereoRenderer.h"

SofaStereoRenderer::SofaStereoRenderer (ParameterTree & parameter_tree, UDPReceiver & udp_receiver)
    : parameter_tree_ (parameter_tree)
    , udp_receiver_ (udp_receiver)
{
    for (int buffer_index = 0; buffer_index < hrir_buffers_.size (); buffer_index++)
    {
        hrir_buffers_ [buffer_index].setSize (2, sofa_filter_.GetFilterLength ());
        sofa_filter_.GetFilterForSphericalCoordinates (hrir_buffers_ [buffer_index],
                                                       left_delays_ [buffer_index],
                                                       right_delays_ [buffer_index],
                                                       kSphericalCoordinates [buffer_index]);
    }
}
SofaStereoRenderer::~SofaStereoRenderer ()
{
}

void SofaStereoRenderer::prepare (const juce::dsp::ProcessSpec & spec)
{
    sample_rate_ = spec.sampleRate;
    for (int buffer_index = 0; buffer_index < hrir_buffers_.size (); buffer_index++)
    {
        sofa_renderers_ [buffer_index].Prepare (spec, sofa_filter_.GetFilterLength ());
    }

    renderer_input_buffer_.setSize (2, (int) spec.maximumBlockSize);
    renderer_output_buffer_.setSize (2, (int) spec.maximumBlockSize);
}

void SofaStereoRenderer::reset ()
{
    for (auto & sofa_renderer : sofa_renderers_)
        sofa_renderer.reset ();
}

void SofaStereoRenderer::process (const juce::dsp::ProcessContextReplacing<float> & replacing)
{
    auto input_block = replacing.getInputBlock ();
    auto output_block = replacing.getOutputBlock ();

    jassert (input_block.getNumChannels () == 2);
    jassert (output_block.getNumChannels () == 2);

    if (*parameter_tree_.binaural_parameter > 0.5)
        return;

    //    float headpos_yaw = udp_receiver_.head_position_.yaw;
    //    float headpos_pitch = udp_receiver_.head_position_.pitch;
    auto [headpos_pitch, headpos_yaw] = udp_receiver_.GetHeadPosition ();

    for (int buffer_index = 0; buffer_index < hrir_buffers_.size (); buffer_index++)
    {
        auto width = buffer_index == 0 ? *parameter_tree_.speaker_width_parameter
                                       : -*parameter_tree_.speaker_width_parameter;
        auto azimuth =
            -1.f * (*parameter_tree_.speaker_position_parameter - headpos_yaw + (width / 2.f));
        SofaFilter::SphericalCoordinates coords {.azimuth_degrees = azimuth,
                                                 .elevation_degrees = -headpos_pitch};

        //        DBG ("buffer: " + juce::String (buffer_index) + " ->" + juce::String (azimuth) + "
        //        :" +
        //             juce::String (headpos_yaw) + "+" +
        //             juce::String (*parameter_tree_.speaker_position_parameter) + "+" +
        //             juce::String (width / 2));

        sofa_filter_.GetFilterForSphericalCoordinates (hrir_buffers_ [buffer_index],
                                                       left_delays_ [buffer_index],
                                                       right_delays_ [buffer_index],
                                                       coords);

        sofa_renderers_ [buffer_index].SetFilter (hrir_buffers_ [buffer_index],
                                                  left_delays_ [buffer_index],
                                                  right_delays_ [buffer_index],
                                                  sample_rate_);
    }

    juce::dsp::AudioBlock<float> renderer_input_block {renderer_input_buffer_};
    renderer_input_block = renderer_input_block.getSubBlock (0, input_block.getNumSamples ());

    juce::dsp::AudioBlock<float> renderer_output_block {renderer_output_buffer_};
    renderer_output_block = renderer_output_block.getSubBlock (0, output_block.getNumSamples ());

    renderer_input_block.clear ();

    renderer_input_block.add (input_block).multiplyBy (0.5f);
    output_block.clear ();

    jassert (sofa_renderers_.size () == hrir_buffers_.size ());
    for (auto renderer_index = 0; renderer_index < sofa_renderers_.size (); renderer_index++)
    {
        juce::dsp::ProcessContextNonReplacing<float> renderer_context {
            renderer_input_block.getSingleChannelBlock (renderer_index), renderer_output_block};

        sofa_renderers_ [renderer_index].Process (renderer_context, hrir_buffers_ [renderer_index]);

        output_block.add (renderer_output_block);
    }
}

// Currently unused until integrated with headtracking
bool SofaStereoRenderer::ParamDiff (ParameterTree & parameter_tree)
{
    bool result = true;
    if (*parameter_tree.speaker_width_parameter == saved_params_.speaker_width_parameter &&
        *parameter_tree.speaker_position_parameter == saved_params_.speaker_position_parameter)
        result = false;

    saved_params_.speaker_position_parameter = *parameter_tree.speaker_position_parameter;
    saved_params_.speaker_width_parameter = *parameter_tree.speaker_width_parameter;

    return result;
}
