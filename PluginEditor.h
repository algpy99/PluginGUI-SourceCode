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
    void initDial2();
    void initButton();

    juce::Slider dial;
    juce::Slider dial2;

    juce::TextButton button;

    //juce::Label dialLabel;

    juce::GroupComponent border1;
    juce::GroupComponent border2;

    juce::GroupComponent border3;
    juce::GroupComponent border4;

    juce::Image background;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InterfaceTestAudioProcessorEditor)
};
