/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
InterfaceTestAudioProcessor::InterfaceTestAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    treeState.addParameterListener("roomSize", this);
    treeState.addParameterListener("damping", this);
    treeState.addParameterListener("wetLevel", this);
}

/*
    float roomSize = false;
    float damping = false;
    float wetLevel = false;
*/

InterfaceTestAudioProcessor::~InterfaceTestAudioProcessor()
{
    treeState.removeParameterListener("roomSize", this);
    treeState.removeParameterListener("damping", this);
    treeState.removeParameterListener("wetLevel", this);
}

juce::AudioProcessorValueTreeState::ParameterLayout InterfaceTestAudioProcessor::createParameterLayout() {

    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;

    auto pRoomSize = std::make_unique<juce::AudioParameterFloat>("roomSize", "RoomSize", 0.0f, 1.0f, 0.5f);
    auto pDamping = std::make_unique<juce::AudioParameterFloat>("damping", "Damping", 0.0f, 1.0f, 0.5f);
    auto pWetLevel = std::make_unique<juce::AudioParameterFloat>("wetLevel", "WetLevel", 0.0f, 1.0f, 0.5f);
    
    params.push_back(std::move(pRoomSize));
    params.push_back(std::move(pDamping));
    params.push_back(std::move(pWetLevel));

    return { params.begin(), params.end() };
} 

void InterfaceTestAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    updateParameters();

    /*
    if (parameterID == "wetLevel")
    {
        wetLevel = newValue;
        DBG("wetLevel is: " << newValue);
    }
    */

    treeState.addParameterListener("roomSize", this);
    treeState.addParameterListener("damping", this);
    treeState.addParameterListener("wetLevel", this);
}

void InterfaceTestAudioProcessor::updateParameters()
{
    parameters.wetLevel = roomSize;
    parameters.wetLevel = damping;
    parameters.wetLevel = wetLevel;
    reverb.setParameters(parameters);
}

//==============================================================================
const juce::String InterfaceTestAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool InterfaceTestAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool InterfaceTestAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool InterfaceTestAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double InterfaceTestAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int InterfaceTestAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int InterfaceTestAudioProcessor::getCurrentProgram()
{
    return 0;
}

void InterfaceTestAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String InterfaceTestAudioProcessor::getProgramName (int index)
{
    return {};
}

void InterfaceTestAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void InterfaceTestAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();

    reverb.reset();
    reverb.prepare(spec);

    updateParameters();
}

void InterfaceTestAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool InterfaceTestAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void InterfaceTestAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    juce::dsp::AudioBlock<float> block(buffer);

    reverb.process(juce::dsp::ProcessContextReplacing<float>(block));

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool InterfaceTestAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* InterfaceTestAudioProcessor::createEditor()
{
    return new InterfaceTestAudioProcessorEditor (*this);
    //return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void InterfaceTestAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void InterfaceTestAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new InterfaceTestAudioProcessor();
}
