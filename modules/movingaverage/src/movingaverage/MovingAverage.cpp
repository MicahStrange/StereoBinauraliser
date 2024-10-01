
#include "MovingAverage.h"

void MovingAverage::prepare (const juce::dsp::ProcessSpec & spec)
{
    jassert (MaxDelaySamples > spec.maximumBlockSize);
    circ_buff_.setSize (spec.numChannels, MaxDelaySamples);
    circ_buff_.clear ();

    average_.resize (spec.numChannels);
}

void MovingAverage::process (const juce::dsp::ProcessContextReplacing<float> & replacing)
{
    auto input_block = replacing.getInputBlock ();
    auto output_block = replacing.getOutputBlock ();
    auto circ_buff_size = circ_buff_.getNumSamples ();

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
            output_block.setSample (channel_index, sample_index, average_ [channel_index]);
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
    read_head_ = (write_head_ - filter_size) % circ_buff_.getNumSamples ();
}
