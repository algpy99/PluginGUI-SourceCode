/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class InterfaceTestAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    InterfaceTestAudioProcessorEditor (InterfaceTestAudioProcessor&);
    ~InterfaceTestAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    InterfaceTestAudioProcessor& audioProcessor;

    void initWindow();
    void initDial();
    void initButton();

    juce::Slider dial;

    juce::TextButton button;

    juce::Label dialLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InterfaceTestAudioProcessorEditor)
};
