#pragma once

#include "zones_convolver/util/CircularBuffer.h"
#include "zones_convolver/util/ComplexBuffer.h"
#include "zones_convolver/util/FrequencyDelayLine.h"

#include <juce_dsp/juce_dsp.h>

class TimeVaryingConvolver
{
public:
    TimeVaryingConvolver ();
    void Prepare (const juce::dsp::ProcessSpec & spec, const int & max_ir_num_samples);
    void Process (const juce::dsp::ProcessContextReplacing<float> & replacing,
                  const std::optional<juce::dsp::AudioBlock<float>> & ir);
    void Reset ();

private:
    void InitialiseFilterPartitions (std::vector<zones::ComplexBuffer> & filter);
    void PrepareIr (const juce::dsp::AudioBlock<float> & ir,
                    std::vector<zones::ComplexBuffer> & filter_to_fill);

    void BeginFade ();
    void ResetFade ();

    int fft_size_;
    std::unique_ptr<juce::dsp::FFT> fft_;
    int num_samples_to_discard_ = 0;
    int num_samples_collected_ = 0;
    int partition_size_ = 0;
    int num_partitions_ = 0;
    juce::dsp::ProcessSpec spec_;

    std::unique_ptr<zones::FrequencyDelayLine> frequency_delay_line_;
    std::unique_ptr<zones::ComplexBuffer> main_fdl_convolved_output_;
    std::unique_ptr<zones::ComplexBuffer> fade_fdl_convolved_output_;
    std::unique_ptr<zones::ComplexBuffer> main_ifft_buffer_;
    std::unique_ptr<zones::ComplexBuffer> fade_ifft_buffer_;

    std::vector<zones::ComplexBuffer> main_filter_partitions_;
    std::vector<zones::ComplexBuffer> fade_filter_partitions_;
    std::vector<zones::ComplexBuffer> pending_filter_partitions_;

    juce::LinearSmoothedValue<float> smoothed_value_in_;
    juce::LinearSmoothedValue<float> smoothed_value_out_;

    bool main_filled_ = false;
    bool fade_filled_ = false;
    bool pending_filled_ = false;

    juce::AudioBuffer<float> saved_inputs_;
    zones::CircularBuffer circular_buffer_ {saved_inputs_};
};
