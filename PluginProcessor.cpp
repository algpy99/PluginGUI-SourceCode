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
    treeState.addParameterListener(roomsizeID, this);
    treeState.addParameterListener(dampingID, this);
    treeState.addParameterListener(driveID, this);
    treeState.addParameterListener(frequencyID, this);
    treeState.addParameterListener(lfotypeID, this);
    treeState.addParameterListener(lowcutID, this);
    treeState.addParameterListener(highcutID, this);
    treeState.addParameterListener(mixLFOID, this);

    waveViewerPost.setRepaintRate(30);
    waveViewerPost.setBufferSize(256);
}

InterfaceTestAudioProcessor::~InterfaceTestAudioProcessor()
{
    treeState.removeParameterListener(roomsizeID, this);
    treeState.removeParameterListener(dampingID, this);
    treeState.removeParameterListener(driveID, this);
    treeState.removeParameterListener(frequencyID, this);
    treeState.removeParameterListener(lfotypeID, this);
    treeState.removeParameterListener(lowcutID, this);
    treeState.removeParameterListener(highcutID, this);
    treeState.removeParameterListener(mixLFOID, this);
}

juce::AudioProcessorValueTreeState::ParameterLayout InterfaceTestAudioProcessor::createParameterLayout() {

    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;

    juce::StringArray lfoTypes = { "Sine", "Saw", "Square" };

    auto pRoomSize = std::make_unique<juce::AudioParameterFloat>(roomsizeID, roomsizeName, 0.0f, 1.0f, 0.0f);
    auto pDamping = std::make_unique<juce::AudioParameterFloat>(dampingID, dampingName, 0.0f, 1.0f, 0.0f);
    auto pDrive = std::make_unique<juce::AudioParameterFloat>(driveID, driveName, 0.0f, 24.0f, 0.0f);
    auto pFrequency = std::make_unique<juce::AudioParameterFloat>(frequencyID, frequencyName, 0.0f, 20.0f, 7.0f);
    auto pLFOType = std::make_unique<juce::AudioParameterChoice>(lfotypeID, lfotypeName, lfoTypes, 0);
    auto pLowcut = std::make_unique<juce::AudioParameterFloat>(lowcutID, lowcutName, juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.2f), 20000.0f);
    auto pHighcut = std::make_unique<juce::AudioParameterFloat>(highcutID, highcutName, juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.2f), 0.0f);
    auto pMixLFO = std::make_unique<juce::AudioParameterFloat>(mixLFOID, mixLFOName, 0.0f, 100.0f, 100.0f);

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

    treeState.addParameterListener(roomsizeID, this);
    treeState.addParameterListener(dampingID, this);
    treeState.addParameterListener(driveID, this);
    treeState.addParameterListener(frequencyID, this);
    treeState.addParameterListener(lfotypeID, this);
    treeState.addParameterListener(lowcutID, this);
    treeState.addParameterListener(highcutID, this);
    treeState.addParameterListener(mixLFOID, this);
}

void InterfaceTestAudioProcessor::updateParameters()
{
    parameters.damping = 0.5f;     /**< Damping, 0 to 1.0, where 0 is not damped, 1.0 is fully damped. */
    parameters.wetLevel = 0.33f;    /**< Wet level, 0 to 1.0 */
    parameters.dryLevel = 0.4f;     /**< Dry level, 0 to 1.0 */
    parameters.freezeMode = 0.0f;

    /*
    auto type = static_cast<int>(treeState.getRawParameterValue("lfoType")->load());
    switch (type)
    {
    case 0:
        lfo.setLFOType(alex_dsp::LFOGenerator::LFOType::kSine);
        break;
    case 1:
        lfo.setLFOType(alex_dsp::LFOGenerator::LFOType::kSaw);
        break;
    case 2:
        lfo.setLFOType(alex_dsp::LFOGenerator::LFOType::kSquare);
        break;
    }
    */

    reverb.setParameters(parameters);

    distortion.setMix(1.0);
    distortion.setDrive(treeState.getRawParameterValue(driveID)->load());

    lfo.setFrequency(treeState.getRawParameterValue(frequencyID)->load());
    lfo.setDepth(treeState.getRawParameterValue(mixLFOID)->load());

    LPfilter.setCutoffFrequency(treeState.getRawParameterValue(highcutID)->load());
    HPfilter.setCutoffFrequency(treeState.getRawParameterValue(lowcutID)->load());
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

    reverb.reset();
    reverb.prepare(spec);

    lfo.reset();
    lfo.prepare(spec);

    LPfilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    LPfilter.prepare(spec);

    LPfilter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
    HPfilter.prepare(spec);

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

    parameters.roomSize = treeState.getRawParameterValue(roomsizeID)->load();
    parameters.width = treeState.getRawParameterValue(dampingID)->load();

    //Parameters.m_frequency = 
        //treeState.getRawParameterValue(lfotypeID)->load());

    reverb.process(juce::dsp::ProcessContextReplacing<float>(block));

    distortion.process(juce::dsp::ProcessContextReplacing<float>(block));

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            //lfo.m_frequency.getNextValue();
            lfo.process();
            channelData[sample] = buffer.getSample(channel, sample) * lfo.getCurrentLFOValue();
        }
    }
    LPfilter.process(juce::dsp::ProcessContextReplacing<float>(block));
    HPfilter.process(juce::dsp::ProcessContextReplacing<float>(block));

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
