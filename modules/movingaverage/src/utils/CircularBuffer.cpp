
#include "CircularBuffer.h"

CircularBuffer::CircularBuffer (juce::AudioBuffer<float> buffer)
    : circ_block_ (buffer)
{
}

void CircularBuffer::clear ()
{
    circ_block_.clear ();
}
float CircularBuffer::getSample (int delay, bool progress)
{
    int read_head = (write_head_ - delay) % circ_block_.getNumSamples ();
    return 0;
}

void CircularBuffer::add (juce::dsp::AudioBlock<float> new_samples)
{
    int num_input_samples = static_cast<int> (new_samples.getNumSamples ());
    int num_samples_before_wrap =
        std::min (num_input_samples, static_cast<int> (circ_block_.getNumSamples ()) - write_head_);
    int num_samples_after_wrap = num_input_samples - num_samples_before_wrap;

    circ_block_.getSubBlock (write_head_)
        .copyFrom (new_samples.getSubBlock (0, num_samples_before_wrap));
    write_head_ += num_samples_before_wrap % circ_block_.getNumSamples ();
    circ_block_.getSubBlock (write_head_)
        .copyFrom (new_samples.getSubBlock (num_samples_before_wrap, num_samples_after_wrap));
    write_head_ += num_samples_after_wrap % circ_block_.getNumSamples ();
}

void CircularBuffer::addSample (float sample_to_add)
{
}
