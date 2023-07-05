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
    initDials();

    addAndMakeVisible(sectionAtmosphere);
    sectionAtmosphere.setText("Atmosphere");
    sectionAtmosphere.setColour(juce::GroupComponent::ColourIds::outlineColourId, MyColours::vitalMidGrey);
    
    addAndMakeVisible(sectionDrive);
    sectionDrive.setText("Drive");
    sectionDrive.setColour(juce::GroupComponent::ColourIds::outlineColourId, MyColours::vitalMidGrey);

    addAndMakeVisible(sectionFilter);
    sectionFilter.setText("Filter");
    sectionFilter.setColour(juce::GroupComponent::ColourIds::outlineColourId, MyColours::vitalMidGrey);

    addAndMakeVisible(sectionLFO);
    sectionLFO.setText("LFO");
    sectionLFO.setColour(juce::GroupComponent::ColourIds::outlineColourId, MyColours::vitalMidGrey);

    addAndMakeVisible(sectionGraph);
    sectionGraph.setText("Output");
    sectionGraph.setColour(juce::GroupComponent::ColourIds::outlineColourId, MyColours::vitalMidGrey);
}

void InterfaceTestAudioProcessorEditor::initWindow()
{
    auto width = 1500;
    auto height = 750;
    float ratio = 2;
    setResizable(true, true);
    setResizeLimits(0.4 * width, 0.4 * height, 0.8 * width, 0.8 * height);
    getConstrainer()->setFixedAspectRatio(ratio);
    setSize(0.4 * width, 0.4 * height);
}

void InterfaceTestAudioProcessorEditor::initDials()
{
    addAndMakeVisible(dial1);
    dial1.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    dial1.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 20);
    dial1.setRange(0.0, 100.0, 0.1);
    dial1.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, MyColours::orange);
    dial1.setLookAndFeel(&myLookAndFeel);
    dial1.setColour(juce::Slider::textBoxOutlineColourId, MyColours::vitalGrey);

    addAndMakeVisible(dial2);
    dial2.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    dial2.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    dial2.setRange(0.0, 100.0, 0.1);
    dial2.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, MyColours::orange);
    dial2.setLookAndFeel(&myLookAndFeel);
    dial2.setColour(juce::Slider::textBoxOutlineColourId, MyColours::vitalGrey);

    addAndMakeVisible(dial3);
    dial3.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    dial3.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    dial3.setRange(0.0, 100.0, 0.1);
    dial3.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, MyColours::orange);
    dial3.setLookAndFeel(&myLookAndFeel);
    dial3.setColour(juce::Slider::textBoxOutlineColourId, MyColours::vitalGrey);

    addAndMakeVisible(dial4);
    dial4.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    dial4.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    dial4.setRange(0.0, 100.0, 0.1);
    dial4.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, MyColours::orange);
    dial4.setLookAndFeel(&myLookAndFeel);
    dial4.setColour(juce::Slider::textBoxOutlineColourId, MyColours::vitalGrey);

    addAndMakeVisible(dial5);
    dial5.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    dial5.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    dial5.setRange(0.0, 100.0, 0.1);
    dial5.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, MyColours::orange);
    dial5.setLookAndFeel(&myLookAndFeel);
    dial5.setColour(juce::Slider::textBoxOutlineColourId, MyColours::vitalGrey);

}

InterfaceTestAudioProcessorEditor::~InterfaceTestAudioProcessorEditor()
{
}

//==============================================================================
void InterfaceTestAudioProcessorEditor::paint (juce::Graphics& g)
{    
    if (dial1.isMouseOverOrDragging(true) || dial2.isMouseOverOrDragging(true))
    {
        sectionAtmosphere.setColour(juce::GroupComponent::ColourIds::textColourId, MyColours::orange);
    }    
    else
    {
        sectionAtmosphere.setColour(juce::GroupComponent::ColourIds::textColourId, MyColours::cream);
    }

    g.fillAll (MyColours::vitalGrey);
}

void InterfaceTestAudioProcessorEditor::resized()
{
    
    auto top = getHeight() * 0.15;
    auto margin = getWidth() * 0.05;
    
    auto blockW = getWidth() * 0.21;
    auto blockH = getHeight() * 0.42;

    auto blockSep = getWidth() * 0.02;

    auto LFOH = getHeight() * 0.74;
    auto FilterH = getHeight() * 0.3;

    auto dialSize = 0.6 * blockW;
    auto dialSizeSmall = 0.4 * blockW;

    sectionAtmosphere.setBounds(  margin, 
                                top,
                                2 * blockW + blockSep,
                                blockH);

    sectionDrive.setBounds(     margin + 2 * blockW + 2 * blockSep,
                                top,
                                blockW,
                                blockH);

    sectionLFO.setBounds(       margin + 3 * blockW + 3 * blockSep,
                                top, 
                                blockW, 
                                LFOH);

    sectionFilter.setBounds(    margin,
                                top + blockH + 0.5 * blockSep,
                                1 * blockW,
                                FilterH);

    sectionGraph.setBounds(    margin + blockW + blockSep,
                                top + blockH + 0.5 * blockSep,
                             2 * blockW + blockSep,
                            FilterH);

    dial1.setBounds(margin + 0.5f * (blockW - dialSize), top + 0.5f * (blockH - dialSize), dialSize, dialSize);
    dial2.setBounds(margin + blockW + blockSep + 0.5f * (blockW - dialSize), top + 0.5f * (blockH - dialSize), dialSize, dialSize);

    dial3.setBounds(margin + 2 * blockW + 2 * blockSep + 0.5f * (blockW - dialSize), top + 0.5f * (blockH - dialSize), dialSize, dialSize);
    
    dial4.setBounds(margin + 0.5f * (blockW - 2.0f * dialSizeSmall), top + blockH + margin, dialSizeSmall, dialSizeSmall);
    dial5.setBounds(margin + 0.5f * (blockW - 2.0f * dialSizeSmall) + dialSizeSmall, top + blockH + margin, dialSizeSmall, dialSizeSmall);

    //n + 0.5f * (blockW - 2.0f *  0.7f * dialSize)
}