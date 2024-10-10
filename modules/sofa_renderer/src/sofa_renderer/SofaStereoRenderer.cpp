//
// Created by Micah Strange on 04/10/2024.
//

#include "SofaStereoRenderer.h"

SofaStereoRenderer::SofaStereoRenderer (ParameterTree & parameter_tree)
    : parameter_tree_ (parameter_tree)
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
        sofa_renderers_ [buffer_index].prepare (spec);
        sofa_renderers_ [buffer_index].SetFilter (hrir_buffers_ [buffer_index],
                                                  left_delays_ [buffer_index],
                                                  right_delays_ [buffer_index],
                                                  sample_rate_);
    }

    renderer_input_buffer_.setSize (1, (int) spec.maximumBlockSize);
    renderer_output_buffer_.setSize (2, (int) spec.maximumBlockSize);
}
void SofaStereoRenderer::process (
    const juce::dsp::ProcessContextNonReplacing<float> & processContext)
{
    auto input_block = processContext.getInputBlock ();
    auto output_block = processContext.getOutputBlock ();

    jassert (input_block.getNumChannels () == 2);
    jassert (output_block.getNumChannels () == 2);

    juce::dsp::AudioBlock<float> renderer_input_block {renderer_input_buffer_};
    renderer_input_block = renderer_input_block.getSubBlock (0, input_block.getNumSamples ());

    juce::dsp::AudioBlock<float> renderer_output_block {renderer_output_buffer_};
    renderer_output_block = renderer_output_block.getSubBlock (0, output_block.getNumSamples ());

    for (auto renderer_index = 0; renderer_index < sofa_renderers_.size (); renderer_index++)
    {
        renderer_input_block.clear ();

        renderer_input_block.add (input_block);

        juce::dsp::ProcessContextNonReplacing<float> renderer_context {renderer_input_block,
                                                                       renderer_output_block};
        sofa_renderers_ [renderer_index].process (renderer_context);

        output_block.add (renderer_output_block);
    }
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

    juce::dsp::AudioBlock<float> renderer_input_block {renderer_input_buffer_};
    renderer_input_block = renderer_input_block.getSubBlock (0, input_block.getNumSamples ());

    juce::dsp::AudioBlock<float> renderer_output_block {renderer_output_buffer_};
    renderer_output_block = renderer_output_block.getSubBlock (0, output_block.getNumSamples ());

    for (auto renderer_index = 0; renderer_index < sofa_renderers_.size (); renderer_index++)
    {
        renderer_input_block.clear ();

        renderer_input_block.add (input_block.getSingleChannelBlock (0)).multiplyBy (0.3f);

        juce::dsp::ProcessContextNonReplacing<float> renderer_context {renderer_input_block,
                                                                       renderer_output_block};
        sofa_renderers_ [renderer_index].process (renderer_context);

        output_block.add (renderer_output_block);
    }
}
