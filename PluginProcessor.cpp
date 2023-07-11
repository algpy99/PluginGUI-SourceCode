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
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ), treeState(*this, nullptr, "PARAMETERS", createParameterLayout()), waveViewerPost(1)
#endif
{
    treeState.addParameterListener("roomSize", this);
    treeState.addParameterListener("damping", this);
    treeState.addParameterListener("drive", this);
    treeState.addParameterListener("frequency", this);
    treeState.addParameterListener("lfotype", this);
    treeState.addParameterListener("lowcut", this);
    treeState.addParameterListener("highcut", this);
    treeState.addParameterListener("mixLFO", this);

    waveViewerPost.setRepaintRate(30);
    waveViewerPost.setBufferSize(256);
}

InterfaceTestAudioProcessor::~InterfaceTestAudioProcessor()
{
    treeState.removeParameterListener("roomSize", this);
    treeState.removeParameterListener("damping", this);
    treeState.removeParameterListener("drive", this);
    treeState.removeParameterListener("frequency", this);
    treeState.removeParameterListener("lfotype", this);
    treeState.removeParameterListener("lowcut", this);
    treeState.removeParameterListener("highcut", this);
    treeState.removeParameterListener("mixLFO", this);
}

juce::AudioProcessorValueTreeState::ParameterLayout InterfaceTestAudioProcessor::createParameterLayout() {

    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;

    juce::StringArray lfoTypes = { "Sine", "Saw", "Square" };

    auto pRoomSize = std::make_unique<juce::AudioParameterFloat>("roomSize", "RoomSize", 0.0f, 1.0f, 0.0f);
    auto pDamping = std::make_unique<juce::AudioParameterFloat>("damping", "Damping", 0.0f, 1.0f, 0.0f);
    auto pDrive = std::make_unique<juce::AudioParameterFloat>("drive", "Drive", 0.0f, 1.0f, 0.0f);
    auto pFrequency = std::make_unique<juce::AudioParameterFloat>("frequency", "Frequency", 0.0f, 500.0f, 20.0f);
    auto pLFOType = std::make_unique<juce::AudioParameterChoice>("lfoType", "LFO Type", lfoTypes, 0);
    auto pLowcut = std::make_unique<juce::AudioParameterFloat>("lowcut", "Lowcut", 22.0f, 22000.0f, 0.0f);
    auto pHighcut = std::make_unique<juce::AudioParameterFloat>("highcut", "Highcut", 22.0f, 22000.0f, 22000.0f);
    auto pMixLFO = std::make_unique<juce::AudioParameterFloat>("mixLFO", "Mix", 0.0f, 100.0f, 100.0f);

    params.push_back(std::move(pRoomSize));
    params.push_back(std::move(pDamping));
    params.push_back(std::move(pDrive));
    params.push_back(std::move(pFrequency));
    params.push_back(std::move(pLFOType));
    params.push_back(std::move(pLowcut));
    params.push_back(std::move(pHighcut));
    params.push_back(std::move(pMixLFO));


    return { params.begin(), params.end() };
} 

void InterfaceTestAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    updateParameters();

    treeState.addParameterListener("roomSize", this);
    treeState.addParameterListener("damping", this);
    treeState.addParameterListener("drive", this);
    treeState.addParameterListener("frequency", this);
    treeState.addParameterListener("lfotype", this);
    treeState.addParameterListener("lowcut", this);
    treeState.addParameterListener("highcut", this);
    treeState.addParameterListener("mixLFO", this);
}

void InterfaceTestAudioProcessor::updateParameters()
{
    //reverb.setParameters(parameters);

    distortion.setMix(treeState.getRawParameterValue("mix")->load());
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

    distortion.reset();
    distortion.prepare(spec);

    //reverb.reset();
    //reverb.prepare(spec);

    lfo.prepare(spec);

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

    //reverb.process(juce::dsp::ProcessContextReplacing<float>(block));

    distortion.process(juce::dsp::ProcessContextReplacing<float>(block));

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] = buffer.getSample(channel, sample) * 3.0f;
        }
    }
    waveViewerPost.pushBuffer(buffer);
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
    //juce::MemoryOutputStream stream(destData, false);
    //treeState.state.writeToStream(stream);
}

void InterfaceTestAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    /*
    auto tree = juce::ValueTree::readFromData(data, size_t(sizeInBytes));

    if (tree.isValid())
    {
        treeState.state = tree;
    }
    */
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new InterfaceTestAudioProcessor();
}
