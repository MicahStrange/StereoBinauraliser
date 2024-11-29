#include "TimeVaryingConvolver.h"

#include "zones_convolver/PartitionSchemes.h"

TimeVaryingConvolver::TimeVaryingConvolver ()
{
}

void TimeVaryingConvolver::Prepare (const juce::dsp::ProcessSpec & spec,
                                    const int & max_ir_num_samples)
{
    spec_ = spec;
    const auto num_channels = static_cast<int> (spec.numChannels);

    partition_size_ = static_cast<int> (spec.maximumBlockSize);

    fft_size_ = juce::nextPowerOfTwo (partition_size_ * 2);

    auto fft_order = std::log2 (fft_size_);
    fft_ = std::make_unique<juce::dsp::FFT> (fft_order);

    num_samples_to_discard_ = fft_size_ - partition_size_;

    jassert (max_ir_num_samples > 0);
    num_partitions_ =
        zones::GetNumPartitionsRequiredForSegment (partition_size_, max_ir_num_samples);

    saved_inputs_.setSize (num_channels, fft_size_);

    frequency_delay_line_ =
        std::make_unique<zones::FrequencyDelayLine> (num_channels, num_partitions_, fft_size_);
    main_fdl_convolved_output_ = std::make_unique<zones::ComplexBuffer> (fft_size_, num_channels);
    fade_fdl_convolved_output_ = std::make_unique<zones::ComplexBuffer> (fft_size_, num_channels);

    main_ifft_buffer_ = std::make_unique<zones::ComplexBuffer> (fft_size_, num_channels);
    fade_ifft_buffer_ = std::make_unique<zones::ComplexBuffer> (fft_size_, num_channels);

    InitialiseFilterPartitions (main_filter_partitions_);
    InitialiseFilterPartitions (fade_filter_partitions_);
    InitialiseFilterPartitions (pending_filter_partitions_);

    Reset ();

    static constexpr auto kSmoothingTime = 0.1f;
    smoothed_value_in_.reset (spec.sampleRate, kSmoothingTime);
    smoothed_value_out_.reset (spec.sampleRate, kSmoothingTime);
}

void TimeVaryingConvolver::Process (const juce::dsp::ProcessContextReplacing<float> & replacing,
                                    const std::optional<juce::dsp::AudioBlock<float>> & ir)
{
    if (ir != std::nullopt)
    {
        if (! main_filled_)
        {
            PrepareIr (ir.value (), main_filter_partitions_);
            main_filled_ = true;
        }

        if (fade_filled_)
        {
            PrepareIr (ir.value (), pending_filter_partitions_);
            pending_filled_ = true;
        }
        else
        {
            PrepareIr (ir.value (), fade_filter_partitions_);
            fade_filled_ = true;
            BeginFade ();
        }
    }

    if (! main_filled_)
        return;

    auto output_block = replacing.getOutputBlock ();
    auto num_channels = static_cast<int> (output_block.getNumChannels ());
    auto num_partitions = static_cast<int> (main_filter_partitions_.size ());
    auto block_size = static_cast<int> (output_block.getNumSamples ());

    auto num_samples_processed = 0;
    while (num_samples_processed < block_size)
    {
        if (num_samples_collected_ == 0)
        {
            frequency_delay_line_->GetNextBlock ();
            main_fdl_convolved_output_->Clear ();
            fade_fdl_convolved_output_->Clear ();

            for (auto partition_index = 1; partition_index < num_partitions; ++partition_index)
            {
                auto & previous_fdl_block =
                    frequency_delay_line_->GetBlockWithOffset (partition_index);
                auto & partition = main_filter_partitions_ [partition_index];

                main_fdl_convolved_output_->ComplexMultiplyAccumulateFrom (previous_fdl_block,
                                                                           partition);

                if (fade_filled_)
                {
                    partition = fade_filter_partitions_ [partition_index];
                    fade_fdl_convolved_output_->ComplexMultiplyAccumulateFrom (previous_fdl_block,
                                                                               partition);
                }
            }

            circular_buffer_.GetNext (partition_size_, true);
        }

        auto num_samples_to_process =
            std::min (block_size - num_samples_processed, partition_size_ - num_samples_collected_);

        auto & first_fdl_block = frequency_delay_line_->GetBlockWithOffset (0);
        first_fdl_block.Clear ();
        auto continuous_fdl_block = first_fdl_block.GetContinuousBlock ();

        auto input_to_copy =
            output_block.getSubBlock (num_samples_processed, num_samples_to_process);

        circular_buffer_.GetNext (num_samples_to_discard_ + num_samples_collected_, false)
            .CopyFrom (input_to_copy);
        circular_buffer_.GetNext (0, false).CopyTo (continuous_fdl_block);

        for (auto channel_index = 0u; channel_index < num_channels; ++channel_index)
            fft_->performRealOnlyForwardTransform (
                continuous_fdl_block.getChannelPointer (channel_index), true);

        auto & first_partition = main_filter_partitions_ [0];

        main_ifft_buffer_->ComplexMultiplyFrom (first_fdl_block, first_partition);
        main_ifft_buffer_->AddFrom (*main_fdl_convolved_output_);

        auto continuous_ifft_block = main_ifft_buffer_->GetContinuousBlock ();
        for (auto channel_index = 0u; channel_index < num_channels; ++channel_index)
            fft_->performRealOnlyInverseTransform (
                continuous_ifft_block.getChannelPointer (channel_index));

        auto offset_output_block =
            output_block.getSubBlock (num_samples_processed, num_samples_to_process);
        auto ifft_valid_block = continuous_ifft_block
                                    .getSubBlock (num_samples_to_discard_ + num_samples_collected_,
                                                  num_samples_to_process)
                                    .multiplyBy (smoothed_value_out_);

        offset_output_block.copyFrom (ifft_valid_block);

        if (fade_filled_)
        {
            auto & first_fade_partition = fade_filter_partitions_ [0];
            fade_ifft_buffer_->ComplexMultiplyFrom (first_fdl_block, first_fade_partition);
            fade_ifft_buffer_->AddFrom (*fade_fdl_convolved_output_);

            auto fade_continuous_ifft_block = fade_ifft_buffer_->GetContinuousBlock ();
            for (auto channel_index = 0u; channel_index < num_channels; ++channel_index)
                fft_->performRealOnlyInverseTransform (
                    fade_continuous_ifft_block.getChannelPointer (channel_index));

            auto fade_ifft_valid_block =
                fade_continuous_ifft_block
                    .getSubBlock (num_samples_to_discard_ + num_samples_collected_,
                                  num_samples_to_process)
                    .multiplyBy (smoothed_value_in_);

            offset_output_block.add (fade_ifft_valid_block);
        }

        num_samples_processed += num_samples_to_process;
        num_samples_collected_ =
            (num_samples_collected_ + num_samples_to_process) % partition_size_;
    }

    if (! smoothed_value_in_.isSmoothing ())
    {
        if (fade_filled_)
        {
            main_filter_partitions_ = fade_filter_partitions_;
            fade_filled_ = false;
            main_filled_ = true;
            ResetFade ();

            if (pending_filled_)
            {
                fade_filter_partitions_ = pending_filter_partitions_;
                pending_filled_ = false;
                fade_filled_ = true;
                BeginFade ();
            }
        }
    }
}

void TimeVaryingConvolver::Reset ()
{
    saved_inputs_.clear ();

    num_samples_collected_ = 0;

    frequency_delay_line_->Clear ();
    main_fdl_convolved_output_->Clear ();
    fade_fdl_convolved_output_->Clear ();
    main_ifft_buffer_->Clear ();
    fade_ifft_buffer_->Clear ();

    ResetFade ();
}

void TimeVaryingConvolver::PrepareIr (const juce::dsp::AudioBlock<float> & ir,
                                      std::vector<zones::ComplexBuffer> & filter_to_fill)
{
    const auto ir_num_channels = static_cast<int> (ir.getNumChannels ());
    const auto ir_num_samples = static_cast<int> (ir.getNumSamples ());

    for (auto partition_index = 0; partition_index < num_partitions_; ++partition_index)
    {
        auto partition_offset = partition_index * partition_size_;
        auto partition_length = std::min (partition_size_, ir_num_samples - partition_offset);

        auto filter_partition = filter_to_fill [partition_index];
        filter_partition.Clear ();

        if (partition_offset < ir_num_samples)
        {
            auto partition_block = ir.getSubBlock (partition_offset, partition_length);
            auto filter_block = filter_partition.GetContinuousBlock ();
            for (auto channel_index = 0; channel_index < ir_num_channels; ++channel_index)
            {
                filter_block.getSingleChannelBlock (channel_index)
                    .copyFrom (
                        partition_block.getSingleChannelBlock (channel_index % ir_num_channels));
                fft_->performRealOnlyForwardTransform (
                    filter_block.getChannelPointer (channel_index), true);
            }
        }
    }
}
void TimeVaryingConvolver::InitialiseFilterPartitions (std::vector<zones::ComplexBuffer> & filter)
{
    for (auto partition_index = 0; partition_index < num_partitions_; ++partition_index)
    {
        filter.emplace_back (zones::ComplexBuffer {
            static_cast<size_t> (juce::nextPowerOfTwo (partition_size_ * 2)), spec_.numChannels});
    }
}

void TimeVaryingConvolver::BeginFade ()
{
    smoothed_value_in_.setTargetValue (1.f);
    smoothed_value_out_.setTargetValue (0.f);
}
void TimeVaryingConvolver::ResetFade ()
{
    smoothed_value_in_.setCurrentAndTargetValue (0.0f);
    smoothed_value_out_.setCurrentAndTargetValue (1.0f);
}
