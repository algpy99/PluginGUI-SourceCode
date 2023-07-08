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
    //sectionAtmosphere.setText("Atmosphere");
    sectionAtmosphere.setColour(juce::GroupComponent::ColourIds::outlineColourId, MyColours::vitalGrey);
    
    addAndMakeVisible(sectionDrive);
    //sectionDrive.setText("Drive");
    sectionDrive.setColour(juce::GroupComponent::ColourIds::outlineColourId, MyColours::vitalGrey);

    addAndMakeVisible(sectionFilter);
    //sectionFilter.setText("Filter");
    sectionFilter.setColour(juce::GroupComponent::ColourIds::outlineColourId, MyColours::vitalGrey);

    addAndMakeVisible(sectionLFO);
    //sectionLFO.setText("LFO");
    sectionLFO.setColour(juce::GroupComponent::ColourIds::outlineColourId, MyColours::vitalGrey);

    addAndMakeVisible(sectionGraph);
    //sectionGraph.setText("Output");
    sectionGraph.setColour(juce::GroupComponent::ColourIds::outlineColourId, MyColours::vitalGrey);

    addAndMakeVisible(audioProcessor.waveViewerPre);
    audioProcessor.waveViewerPre.setColours(MyColours::orange, MyColours::orange);

    addAndMakeVisible(audioProcessor.waveViewerPost);
    audioProcessor.waveViewerPost.setColours(MyColours::green, MyColours::green);

    addAndMakeVisible(button);
    button.setButtonText("X");
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
    reverbDial1.setTextBoxStyle(juce::Slider::NoTextBox, false, 40, 20);
    reverbDial1.setRange(0.0, 100.0, 0.1);
    reverbDial1.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, MyColours::orange);
    reverbDial1.setLookAndFeel(&myLookAndFeel);
    reverbDial1.setColour(juce::Slider::textBoxOutlineColourId, MyColours::vitalMidGrey);

    addAndMakeVisible(reverbDial2);
    reverbDial2.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    reverbDial2.setTextBoxStyle(juce::Slider::NoTextBox, false, 50, 20);
    reverbDial2.setRange(0.0, 100.0, 0.1);
    reverbDial2.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, MyColours::orange);
    reverbDial2.setLookAndFeel(&myLookAndFeel);
    reverbDial2.setColour(juce::Slider::textBoxOutlineColourId, MyColours::vitalMidGrey);

    addAndMakeVisible(DistoDial);
    DistoDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    DistoDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 50, 20);
    DistoDial.setRange(0.0, 100.0, 0.1);
    DistoDial.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, MyColours::orange);
    DistoDial.setLookAndFeel(&myLookAndFeel);
    DistoDial.setColour(juce::Slider::textBoxOutlineColourId, MyColours::vitalMidGrey);

    addAndMakeVisible(filterSlider);
    filterSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    filterSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 50, 20);
    filterSlider.setRange(0.0, 100.0, 0.1);
    filterSlider.setLookAndFeel(&myLookAndFeel);

    addAndMakeVisible(doubleSlider);
    doubleSlider.setSliderStyle(juce::Slider::SliderStyle::TwoValueHorizontal);
    doubleSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    doubleSlider.setMinValue(0.0f, juce::NotificationType::dontSendNotification, false);
    doubleSlider.setMaxValue(100.0f, juce::NotificationType::dontSendNotification, false);
    doubleSlider.setLookAndFeel(&myLookAndFeel);

    /*
    addAndMakeVisible(filterSlider);
    filterSlider.setSliderStyle(juce::Slider::SliderStyle::TwoValueHorizontal);
    filterSlider.setRange(22.0, 22000.0, 0.1);
    filterSlider.setMinAndMaxValues(22.0, 22000.0, juce::NotificationType::dontSendNotification);
    DistoDial.setLookAndFeel(&myLookAndFeel);
    */
    
    //filterSlider.setMinValue(20.0f, juce::NotificationType::dontSendNotification, false);
    //filterSlider.setMaxValue(22000.0f, juce::NotificationType::dontSendNotification, false);


    addAndMakeVisible(LFODial);
    LFODial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    LFODial.setTextBoxStyle(juce::Slider::NoTextBox, false, 50, 20);
    LFODial.setRange(0.0, 100.0, 0.1);
    LFODial.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, MyColours::green);
    LFODial.setLookAndFeel(&myLookAndFeel);
    LFODial.setColour(juce::Slider::textBoxOutlineColourId, MyColours::vitalMidGrey);
}

InterfaceTestAudioProcessorEditor::~InterfaceTestAudioProcessorEditor()
{
}

//==============================================================================
void InterfaceTestAudioProcessorEditor::paint (juce::Graphics& g)
{    
    changeSectionColor();

    addAndMakeVisible(audioProcessor.waveViewerPre);
    addAndMakeVisible(audioProcessor.waveViewerPost);

    g.fillAll (MyColours::vitalMidGrey);
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

    sectionAtmosphere.setBounds(margin, top, getWidth() * 0.3, getHeight() * 0.45);
    sectionDrive.setBounds(sectionAtmosphere.getRight(), top, getWidth() * 0.2, getHeight() * 0.45);
    sectionLFO.setBounds(sectionDrive.getRight(), top, getWidth() * 0.4, getHeight() * 0.75);
    sectionFilter.setBounds(margin, sectionAtmosphere.getBottom(), getWidth() * 0.5, getHeight() * 0.3);

    reverbDial1.setBounds(sectionAtmosphere.getX() + 0.05 * sectionAtmosphere.getWidth(), sectionAtmosphere.getY() + margin, 0.44 * sectionAtmosphere.getWidth(), 0.44 * sectionAtmosphere.getWidth());
    reverbDial2.setBounds(reverbDial1.getRight() + 0.02 * sectionAtmosphere.getWidth(), sectionAtmosphere.getY() + margin, 0.44 * sectionAtmosphere.getWidth(), 0.44 * sectionAtmosphere.getWidth());

    DistoDial.setBounds(sectionDrive.getX() + 0.07 * sectionDrive.getWidth(), sectionDrive.getY() + 0.16 * sectionDrive.getHeight(), 0.86 * sectionDrive.getWidth(), 0.86 * sectionDrive.getWidth());

    LFODial.setBounds(sectionLFO.getX() + 0.22 * sectionLFO.getWidth(), sectionLFO.getY() + 0.1 * sectionLFO.getHeight(), 0.56 * sectionLFO.getWidth(), 0.56 * sectionLFO.getWidth());

    //audioProcessor.waveViewerPre.setBounds(sectionLFO.getX() + 0.1 * sectionLFO.getWidth(), LFODial.getBottom() + 0.1 * sectionLFO.getHeight(), 0.8 * sectionLFO.getWidth(), 0.2 * sectionLFO.getHeight());

    audioProcessor.waveViewerPost.setBounds(sectionLFO.getX() + 0.1 * sectionLFO.getWidth(), LFODial.getBottom() + 0.1 * sectionLFO.getHeight(), 0.8 * sectionLFO.getWidth(), 0.15 * sectionLFO.getHeight());
    //audioProcessor.waveViewerPost.setTransform(juce::AffineTransform::rotation(juce::MathConstants<float>::pi / 2.0f));
    
    //filterSlider.setBounds(sectionFilter.getX() + 0.2 * sectionFilter.getWidth(), sectionFilter.getY() + 0.25 * sectionFilter.getHeight(), 0.6 * sectionFilter.getWidth(), 0.5 * sectionFilter.getHeight());

    doubleSlider.setBounds(sectionFilter.getX() + 0.2 * sectionFilter.getWidth(), sectionFilter.getY() + 0.25 * sectionFilter.getHeight(), 0.6 * sectionFilter.getWidth(), 0.2 * sectionFilter.getHeight());

    button.setBounds(1.3f * sectionAtmosphere.getX(), 1.3f * sectionAtmosphere.getY(), 0.1 * sectionAtmosphere.getWidth(), 0.1 * sectionAtmosphere.getWidth());
}