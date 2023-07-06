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

    addAndMakeVisible(audioProcessor.waveViewerPre);
    audioProcessor.waveViewerPre.setColours(MyColours::vitalGrey, MyColours::vitalCream);

    addAndMakeVisible(audioProcessor.waveViewerPost);
    audioProcessor.waveViewerPost.setColours(MyColours::vitalGrey, MyColours::green);
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
    addAndMakeVisible(reverbDial1);
    reverbDial1.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    reverbDial1.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 20);
    reverbDial1.setRange(0.0, 100.0, 0.1);
    reverbDial1.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, MyColours::orange);
    reverbDial1.setLookAndFeel(&myLookAndFeel);
    reverbDial1.setColour(juce::Slider::textBoxOutlineColourId, MyColours::vitalGrey);

    addAndMakeVisible(reverbDial2);
    reverbDial2.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    reverbDial2.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    reverbDial2.setRange(0.0, 100.0, 0.1);
    reverbDial2.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, MyColours::orange);
    reverbDial2.setLookAndFeel(&myLookAndFeel);
    reverbDial2.setColour(juce::Slider::textBoxOutlineColourId, MyColours::vitalGrey);

    addAndMakeVisible(DistoDial);
    DistoDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    DistoDial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    DistoDial.setRange(0.0, 100.0, 0.1);
    DistoDial.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, MyColours::orange);
    DistoDial.setLookAndFeel(&myLookAndFeel);
    DistoDial.setColour(juce::Slider::textBoxOutlineColourId, MyColours::vitalGrey);

    addAndMakeVisible(filterDial1);
    filterDial1.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    filterDial1.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    filterDial1.setRange(0.0, 100.0, 0.1);
    filterDial1.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, MyColours::orange);
    filterDial1.setLookAndFeel(&myLookAndFeel);
    filterDial1.setColour(juce::Slider::textBoxOutlineColourId, MyColours::vitalGrey);

    addAndMakeVisible(filterDial2);
    filterDial2.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    filterDial2.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    filterDial2.setRange(0.0, 100.0, 0.1);
    filterDial2.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, MyColours::orange);
    filterDial2.setLookAndFeel(&myLookAndFeel);
    filterDial2.setColour(juce::Slider::textBoxOutlineColourId, MyColours::vitalGrey);

    addAndMakeVisible(LFODial);
    LFODial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    LFODial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    LFODial.setRange(0.0, 100.0, 0.1);
    LFODial.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, MyColours::green);
    LFODial.setLookAndFeel(&myLookAndFeel);
    LFODial.setColour(juce::Slider::textBoxOutlineColourId, MyColours::vitalGrey);
}

InterfaceTestAudioProcessorEditor::~InterfaceTestAudioProcessorEditor()
{
}

//==============================================================================
void InterfaceTestAudioProcessorEditor::paint (juce::Graphics& g)
{    
    changeSectionColor();

    addAndMakeVisible(audioProcessor.waveViewerPost);

    g.fillAll (MyColours::vitalGrey);
}

void InterfaceTestAudioProcessorEditor::changeSectionColor()
{
    if (reverbDial1.isMouseOverOrDragging(true) || reverbDial2.isMouseOverOrDragging(true))
    {
        sectionAtmosphere.setColour(juce::GroupComponent::ColourIds::textColourId, MyColours::orange);
    }
    else
    {
        sectionAtmosphere.setColour(juce::GroupComponent::ColourIds::textColourId, MyColours::cream);
    }

    if (DistoDial.isMouseOverOrDragging(true))
    {
        sectionDrive.setColour(juce::GroupComponent::ColourIds::textColourId, MyColours::orange);
    }
    else
    {
        sectionDrive.setColour(juce::GroupComponent::ColourIds::textColourId, MyColours::cream);
    }

    if (filterDial1.isMouseOverOrDragging(true) || filterDial2.isMouseOverOrDragging(true))
    {
        sectionFilter.setColour(juce::GroupComponent::ColourIds::textColourId, MyColours::orange);
    }
    else
    {
        sectionFilter.setColour(juce::GroupComponent::ColourIds::textColourId, MyColours::cream);
    }

    if (LFODial.isMouseOverOrDragging(true))
    {
        sectionLFO.setColour(juce::GroupComponent::ColourIds::textColourId, MyColours::green);
    }
    else
    {
        sectionLFO.setColour(juce::GroupComponent::ColourIds::textColourId, MyColours::cream);
    }
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

    reverbDial1.setBounds(margin + 0.5f * (blockW - dialSize), top + 0.5f * (blockH - dialSize), dialSize, dialSize);
    reverbDial2.setBounds(margin + blockW + blockSep + 0.5f * (blockW - dialSize), top + 0.5f * (blockH - dialSize), dialSize, dialSize);

    DistoDial.setBounds(margin + 2 * blockW + 2 * blockSep + 0.5f * (blockW - dialSize), top + 0.5f * (blockH - dialSize), dialSize, dialSize);
    
    filterDial1.setBounds(margin + 0.5f * (blockW - 2.0f * dialSizeSmall), top + blockH + margin, dialSizeSmall, dialSizeSmall);
    filterDial2.setBounds(margin + 0.5f * (blockW - 2.0f * dialSizeSmall) + dialSizeSmall, top + blockH + margin, dialSizeSmall, dialSizeSmall);

    LFODial.setBounds(margin + 3 * blockW + 3 * blockSep + 0.5f * (blockW - dialSize), top + 0.5f * (blockH - dialSize), dialSize, dialSize);

    audioProcessor.waveViewerPre.setBounds(1.1 * sectionGraph.getX(), 1.1 * sectionGraph.getY(), 0.88 * sectionGraph.getWidth(), 0.3f * sectionGraph.getHeight());
    audioProcessor.waveViewerPost.setBounds(1.1 * sectionGraph.getX(), 1.1 * sectionGraph.getY() + 0.4f * sectionGraph.getHeight(), audioProcessor.waveViewerPre.getWidth(), audioProcessor.waveViewerPre.getHeight());

}