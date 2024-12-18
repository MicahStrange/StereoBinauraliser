#include "SofaRenderer.h"

SofaRenderer::SofaRenderer ()
{
}

void SofaRenderer::Prepare (const juce::dsp::ProcessSpec & spec, const int max_ir_num_samples)
{
    sample_rate_ = spec.sampleRate;
    convolver_.Prepare (spec, max_ir_num_samples);
    auto mono_spec = spec;
    mono_spec.numChannels = 1;
    for (auto & delay_line : delay_lines_)
        delay_line.prepare (mono_spec);

    hrir_buffer_.setSize (2, max_ir_num_samples);
    hrir_buffer_.clear ();
}

void SofaRenderer::SetFilter (juce::dsp::AudioBlock<float> hrir,
                              float left_delay,
                              float right_delay,
                              float sample_rate)
{
    left_delay_ = left_delay;
    right_delay_ = right_delay;

    hrir_buffer_.clear ();
    hrir.copyTo (hrir_buffer_);

    auto left_delay_samples = juce::roundToInt (left_delay_ * sample_rate_);
    auto right_delay_samples = juce::roundToInt (right_delay_ * sample_rate_);

    delay_lines_ [kLeftChannel].setMaximumDelayInSamples (left_delay_samples);
    delay_lines_ [kRightChannel].setMaximumDelayInSamples (right_delay_samples);

    delay_lines_ [kLeftChannel].setDelay (left_delay_samples);
    delay_lines_ [kRightChannel].setDelay (right_delay_samples);
}

void SofaRenderer::Process (const juce::dsp::ProcessContextNonReplacing<float> & processContext,
                            const std::optional<juce::dsp::AudioBlock<float>> & ir)
{
    jassert (processContext.getInputBlock ().getNumChannels () == 1);
    jassert (processContext.getOutputBlock ().getNumChannels () == 2);

    auto input_block = processContext.getInputBlock ();

    const float * duplicated_input [] = {input_block.getChannelPointer (0),
                                         input_block.getChannelPointer (0)};
    juce::dsp::AudioBlock<const float> duplicated_input_block {
        duplicated_input, 2, input_block.getNumSamples ()};

    auto output_block = processContext.getOutputBlock ();

    output_block.copyFrom (duplicated_input_block);
    juce::dsp::ProcessContextReplacing<float> process_context {output_block};

    convolver_.Process (process_context, ir);

    auto left_block = output_block.getSingleChannelBlock (kLeftChannel);
    auto right_block = output_block.getSingleChannelBlock (kRightChannel);

    juce::dsp::ProcessContextReplacing<float> left_context (left_block);
    juce::dsp::ProcessContextReplacing<float> right_context (right_block);

    delay_lines_ [kLeftChannel].process (left_context);
    delay_lines_ [kRightChannel].process (right_context);
}

void SofaRenderer::reset ()
{
    convolver_.Reset ();
    for (auto & delay_line : delay_lines_)
        delay_line.reset ();
}
