#include "PluginProcessor.h"

#include "PluginEditor.h"

//==============================================================================
PluginProcessor::PluginProcessor ()
    : AudioProcessor (BusesProperties ()
                          .withInput ("Input", juce::AudioChannelSet::stereo (), true)
                          .withOutput ("Output", juce::AudioChannelSet::stereo (), true))
    , parameter_tree_state_ (*this,
                             nullptr,
                             ParameterTree::kParameterTreeIdentifier,
                             CreateParameterLayout ())
    , sofa_stereo_renderer_ (parameter_tree_, udp_receiver_)
{
    udp_receiver_.startThread ();
}

PluginProcessor::~PluginProcessor ()
{
    udp_receiver_.stopThread (100);
}

//==============================================================================
const juce::String PluginProcessor::getName () const
{
    return JucePlugin_Name;
}

bool PluginProcessor::acceptsMidi () const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool PluginProcessor::producesMidi () const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool PluginProcessor::isMidiEffect () const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double PluginProcessor::getTailLengthSeconds () const
{
    return 0.0;
}

int PluginProcessor::getNumPrograms ()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int PluginProcessor::getCurrentProgram ()
{
    return 0;
}

void PluginProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String PluginProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void PluginProcessor::changeProgramName (int index, const juce::String & newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void PluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused (sampleRate, samplesPerBlock);

    auto spec = juce::dsp::ProcessSpec {sampleRate,
                                        static_cast<unsigned int> (samplesPerBlock),
                                        static_cast<unsigned int> (getTotalNumInputChannels ())};

    smoothed_input_gain_.reset (spec.sampleRate, 0.1f);

    sofa_stereo_renderer_.prepare (spec);
}

void PluginProcessor::releaseResources ()
{
    sofa_stereo_renderer_.reset ();
}

bool PluginProcessor::isBusesLayoutSupported (const BusesLayout & layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet () != juce::AudioChannelSet::mono () &&
        layouts.getMainOutputChannelSet () != juce::AudioChannelSet::stereo ())
        return false;

        // This checks if the input layout matches the output layout
    #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet () != layouts.getMainInputChannelSet ())
        return false;
    #endif

    return true;
#endif
}

void PluginProcessor::processBlock (juce::AudioBuffer<float> & buffer,
                                    juce::MidiBuffer & midiMessages)
{
    UpdateParameters ();

    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;

    auto block = juce::dsp::AudioBlock<float> (buffer);
    auto context_replacing = juce::dsp::ProcessContextReplacing<float> (block);

    context_replacing.getOutputBlock ().multiplyBy (smoothed_input_gain_);
    sofa_stereo_renderer_.process (context_replacing);
}

//==============================================================================
bool PluginProcessor::hasEditor () const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor * PluginProcessor::createEditor ()
{
    return new PluginEditor (*this);
}

//==============================================================================
void PluginProcessor::getStateInformation (juce::MemoryBlock & destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (destData);
}

void PluginProcessor::setStateInformation (const void * data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused (data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor * JUCE_CALLTYPE createPluginFilter ()
{
    return new PluginProcessor ();
}

juce::AudioProcessorValueTreeState::ParameterLayout PluginProcessor::CreateParameterLayout ()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    parameter_tree_ = ParameterTree::CreateParameterTree (layout);
    return layout;
}

void PluginProcessor::UpdateParameters ()
{
    smoothed_input_gain_.setTargetValue (
        juce::Decibels::decibelsToGain<float> (*parameter_tree_.input_gain_parameter));
}