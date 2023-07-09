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
}

void InterfaceTestAudioProcessorEditor::initWindow()
{
    auto width = 1500;
    auto height = 1000;
    float ratio = 3.0f/2.0f;
    setResizable(true, true);
    setResizeLimits(0.4 * width, 0.4 * height, 0.65 * width, 0.65 * height);
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
    reverbDial1.setColour(juce::Slider::textBoxOutlineColourId, MyColours::vitalMidGrey);

    addAndMakeVisible(reverbDial2);
    reverbDial2.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    reverbDial2.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    reverbDial2.setRange(0.0, 100.0, 0.1);
    reverbDial2.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, MyColours::orange);
    reverbDial2.setLookAndFeel(&myLookAndFeel);
    reverbDial2.setColour(juce::Slider::textBoxOutlineColourId, MyColours::vitalMidGrey);

    addAndMakeVisible(DistoDial);
    DistoDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    DistoDial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
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

    addAndMakeVisible(LFODial);
    LFODial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    LFODial.setTextBoxStyle(juce::Slider::NoTextBox, false, 50, 20);
    LFODial.setRange(0.0, 100.0, 0.1);
    LFODial.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, MyColours::green);
    LFODial.setLookAndFeel(&myLookAndFeel);
    LFODial.setColour(juce::Slider::textBoxOutlineColourId, MyColours::vitalMidGrey);

    addAndMakeVisible(audioProcessor.waveViewerPre);
    audioProcessor.waveViewerPre.setColours(MyColours::vitalGrey, MyColours::orange);
    addAndMakeVisible(audioProcessor.waveViewerPost);
    audioProcessor.waveViewerPost.setColours(MyColours::vitalGrey, MyColours::green);
}

InterfaceTestAudioProcessorEditor::~InterfaceTestAudioProcessorEditor()
{
}

//==============================================================================
void InterfaceTestAudioProcessorEditor::paint (juce::Graphics& g)
{
    // BACKGROUND
    background = juce::ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);
    g.drawImageWithin(background, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);

    // SECTIONS
    top = getHeight() * 0.1f;
    margin = getWidth() * 0.02f;
    separation = getWidth() * 0.01f;

    topBorder = { 1.0f * getX(), 1.0f * getY(),  1.0f * getWidth(), top};
    sectionFX = { margin, top + margin,  0.5f * getWidth(), 0.35f * getHeight() - margin };
    sectionLFO = { margin, sectionFX.getBottom() + separation, 0.5f * getWidth(), 0.47f * getHeight() - separation };
    sectionMix = { sectionFX.getRight() + separation, top + margin, 0.46f * getWidth() - separation, 0.82f * getHeight() - margin };
    bottomBorder = { 1.0f * getX(), sectionLFO.getBottom() + margin,  1.0f * getWidth(), 0.5f * top};

    // DIALS BOUNDS
    reverbDial1.setBounds(sectionFX.getX() + 0.05 * sectionFX.getWidth(), sectionFX.getY() + 0.2 * sectionFX.getHeight(), 0.28 * sectionFX.getWidth(), 0.28 * sectionFX.getWidth());
    reverbDial2.setBounds(reverbDial1.getRight() + 0.03 * sectionFX.getWidth(), sectionFX.getY() + 0.2 * sectionFX.getHeight(), 0.28 * sectionFX.getWidth(), 0.28 * sectionFX.getWidth());
    DistoDial.setBounds(reverbDial2.getRight() + 0.03 * sectionFX.getWidth(), sectionFX.getY() + 0.2 * sectionFX.getHeight(), 0.28 * sectionFX.getWidth(), 0.28 * sectionFX.getWidth());
    filterSlider.setBounds(sectionLFO.getX() + 0.05 * sectionLFO.getWidth(), sectionLFO.getY() + 0.5 * sectionLFO.getHeight(), 0.9 * sectionLFO.getWidth(), 0.1 * sectionLFO.getHeight());
    doubleSlider.setBounds(sectionLFO.getX() + 0.05 * sectionLFO.getWidth(), filterSlider.getY() + 0.25 * sectionLFO.getHeight(), 0.9 * sectionLFO.getWidth(), 0.1 * sectionLFO.getHeight());
    LFODial.setBounds(sectionMix.getX() + 0.1 * sectionMix.getWidth(), sectionMix.getY() + separation, 0.8 * sectionMix.getWidth(), 0.8 * sectionMix.getWidth());
    audioProcessor.waveViewerPre.setBounds(sectionMix.getX() + separation + margin, LFODial.getBottom() + separation, sectionMix.getWidth() - 2.0f * separation - margin, 0.12 * sectionMix.getHeight());
    audioProcessor.waveViewerPost.setBounds(sectionMix.getX() + separation + margin, audioProcessor.waveViewerPre.getBottom() + separation, sectionMix.getWidth() - 2.0f * separation - margin, 0.12 * sectionMix.getHeight());

    // SECTION LINES
    juce::Line<float> lineFX(juce::Point<float>(margin, sectionFX.getCentreY()), juce::Point<float>(margin, sectionFX.getY() + margin));
    g.setColour(MyColours::vitalGrey);
    if (reverbDial1.isMouseOverOrDragging(true))
    {
        g.setColour(MyColours::orange);
    }
    g.drawLine(lineFX, 2.5f);

    juce::Line<float> lineLFO(juce::Point<float>(margin, sectionLFO.getCentreY()), juce::Point<float>(margin, sectionLFO.getY() + margin));
    g.setColour(MyColours::green);
    g.drawLine(lineLFO, 2.5f);
    juce::Line<float> lineMIX(juce::Point<float>(getRight() - margin, sectionMix.getCentreY()), juce::Point<float>(getRight() - margin, sectionLFO.getBottom() - margin));
    g.setColour(MyColours::green);
    g.drawLine(lineMIX, 2.5f);
}

void InterfaceTestAudioProcessorEditor::resized()
{
}