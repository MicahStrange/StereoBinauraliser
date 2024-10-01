
#include "MovingAverage.h"

MovingAverage::MovingAverage (ParameterTree & parameter_tree, bool high_pass)
    : parameter_tree_ (parameter_tree)
    , high_pass_ (high_pass)
{
}

void MovingAverage::prepare (const juce::dsp::ProcessSpec & spec)
{
    jassert (MaxDelaySamples > spec.maximumBlockSize);
    circ_buff_.setSize (spec.numChannels, MaxDelaySamples);
    circ_buff_.clear ();

    average_.resize (spec.numChannels);
    write_head_ = 0;
    reset ();
}

void MovingAverage::process (const juce::dsp::ProcessContextReplacing<float> & replacing)
{
    auto input_block = replacing.getInputBlock ();
    auto output_block = replacing.getOutputBlock ();
    auto circ_buff_size = circ_buff_.getNumSamples ();

    float parameter = high_pass_ ? *parameter_tree_.filter_size_high_parameter
                                 : *parameter_tree_.filter_size_low_parameter;
    filter_size = static_cast<int> (std::clamp (static_cast<int> (parameter), 1, circ_buff_size));

    for (auto sample_index = 0; sample_index < input_block.getNumSamples (); ++sample_index)
    {
        for (auto channel_index = 0; channel_index < input_block.getNumChannels (); ++channel_index)
        {
            // add sample to buffer;
            auto sample_to_add = input_block.getSample (channel_index, sample_index);
            circ_buff_.setSample (channel_index, write_head_, sample_to_add);

            // read old sample
            auto sample_to_remove = circ_buff_.getSample (channel_index, read_head_);

            // add new sample and remove old sample from total
            average_ [channel_index] = average_ [channel_index] + (sample_to_add / filter_size);
            average_ [channel_index] = average_ [channel_index] - (sample_to_remove / filter_size);

            // write total into output buffer
            auto output_sample =
                high_pass_ ? sample_to_add - average_ [channel_index] : average_ [channel_index];
            output_block.setSample (channel_index, sample_index, output_sample);
        }
        write_head_ = (write_head_ + 1) % circ_buff_size;
        read_head_ = ((write_head_ - filter_size) + circ_buff_size) % circ_buff_size;
    }
}

void MovingAverage::reset ()
{
    circ_buff_.clear ();
    for (auto i = 0; i < circ_buff_.getNumChannels (); ++i)
    {
        average_ [i] = 0;
    }
    write_head_ = 0;
    read_head_ =
        ((write_head_ - filter_size) + circ_buff_.getNumSamples ()) % circ_buff_.getNumSamples ();
}
