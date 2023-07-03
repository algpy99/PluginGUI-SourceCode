/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
InterfaceTestAudioProcessorEditor::InterfaceTestAudioProcessorEditor (InterfaceTestAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    initWindow();
    initDial();
    initButton();
}

void InterfaceTestAudioProcessorEditor::initWindow()
{
    setResizable(true, true);
    setResizeLimits(800, 400, 1200, 600);
    getConstrainer()->setFixedAspectRatio(2.0);
    setSize(1000, 500);
}

void InterfaceTestAudioProcessorEditor::initDial()
{
    addAndMakeVisible(dial);
    dial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    dial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 32, 16);
    dial.setRange(-24.0, 24.0, 0.1);
    dial.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colour(0xfff7941d));
    dial.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colour(0xffe4e2dd));
    dial.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour(0xffe4e2dd));
}

void InterfaceTestAudioProcessorEditor::initButton()
{
    addAndMakeVisible(button);
    button.setButtonText("Hi");
    button.setClickingTogglesState(true);
    button.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colour(0xff0e8f51));
}

InterfaceTestAudioProcessorEditor::~InterfaceTestAudioProcessorEditor()
{
}

//==============================================================================
void InterfaceTestAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colour(0xff2c2c2c));

    g.setColour(juce::Colour(0xffe4e2dd));
    g.setFont (36.0f);
    g.drawFittedText ("Stutter Plugin", getLocalBounds(), juce::Justification::centred, 1);
}

void InterfaceTestAudioProcessorEditor::resized()
{
    auto leftMargin = getWidth() * 0.02;
    auto topMargin = getHeight() * 0.2;
    auto dialSize = getWidth() * 0.2;
    auto buttonWidth = getWidth() * 0.1;
    auto buttonHeighht = buttonWidth * 0.5;

    dial.setBounds(leftMargin, topMargin, dialSize, dialSize);
    button.setBounds(dial.getX() + dial.getWidth() + leftMargin, topMargin, buttonWidth, buttonHeighht);
}
