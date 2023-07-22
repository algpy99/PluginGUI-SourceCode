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

    buttonSine.setToggleState(true, juce::NotificationType::dontSendNotification);

    initDials();

}

void InterfaceTestAudioProcessorEditor::initWindow()
{
    auto width = 1500;
    auto height = 750;
    float ratio = 2;
    setResizable(true, true);
    setResizeLimits(0.6 * width, 0.6 * height, 0.75 * width, 0.75 * height);
    getConstrainer()->setFixedAspectRatio(ratio);
    setSize(0.5 * width, 0.5 * height);
}

void InterfaceTestAudioProcessorEditor::initDials()
{
    addAndMakeVisible(reverbDial1);
    reverbDial1.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    reverbDial1.setTextBoxStyle(juce::Slider::NoTextBox, false, 40, 20);
    reverbDial1.setRange(0.0, 100.0, 1.0);
    reverbDial1.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, MyColours::orange);
    reverbDial1.setLookAndFeel(&myLookAndFeel);
    reverbDial1.setColour(juce::Slider::textBoxOutlineColourId, MyColours::vitalMidGrey);

    roomsizeAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState , roomsizeID, reverbDial1);

    addAndMakeVisible(reverbDial2);
    reverbDial2.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    reverbDial2.setTextBoxStyle(juce::Slider::NoTextBox, false, 50, 20);
    reverbDial2.setRange(0.0, 100.0, 1.0);
    reverbDial2.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, MyColours::orange);
    reverbDial2.setLookAndFeel(&myLookAndFeel);
    reverbDial2.setColour(juce::Slider::textBoxOutlineColourId, MyColours::vitalMidGrey);

    dampingAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, dampingID, reverbDial2);

    addAndMakeVisible(DistoDial);
    DistoDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    DistoDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 50, 20);
    DistoDial.setRange(0.0, 100.0, 1.0);
    DistoDial.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, MyColours::orange);
    DistoDial.setLookAndFeel(&myLookAndFeel);
    DistoDial.setColour(juce::Slider::textBoxOutlineColourId, MyColours::vitalMidGrey);

    driveAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState , driveID, DistoDial);

    addAndMakeVisible(filterSlider);
    filterSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    filterSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 50, 20);
    filterSlider.setRange(0.0, 100.0, 1.0);
    filterSlider.setLookAndFeel(&myLookAndFeel);

    frequencyAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, frequencyID, filterSlider);

    addAndMakeVisible(doubleSlider);
    doubleSlider.setSliderStyle(juce::Slider::SliderStyle::TwoValueHorizontal);
    doubleSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    doubleSlider.setMinValue(0.0);
    doubleSlider.setMaxValue(100.0);
    doubleSlider.setLookAndFeel(&myLookAndFeel);

    addAndMakeVisible(LFODial);
    LFODial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    LFODial.setTextBoxStyle(juce::Slider::NoTextBox, false, 50, 20);
    LFODial.setRange(0.0, 100.0, 1.0);
    LFODial.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, MyColours::green);
    LFODial.setLookAndFeel(&myLookAndFeel);
    LFODial.setColour(juce::Slider::textBoxOutlineColourId, MyColours::vitalMidGrey);

    LFOAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, mixLFOID, LFODial);

    addAndMakeVisible(audioProcessor.waveViewerPost);
    audioProcessor.waveViewerPost.setColours(MyColours::keeperGrey, MyColours::green);

    combo.addItem("Sine", 1);
    combo.addItem("Saw", 2);
    combo.addItem("Square", 3);
    addAndMakeVisible(combo);
    combo.setColour(juce::ComboBox::ColourIds::textColourId, MyColours::green);
    combo.setLookAndFeel(&myLookAndFeel);
    //combo.setColour(juce::ComboBox::ColourIds::buttonColourId, MyColours::keeperGrey);
    comboAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.treeState, lfotypeID, combo);

    lowcutAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, highcutID, lowcutSlider);
    lowcutSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    lowcutSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 40, 20);
    lowcutSlider.setSkewFactor(10000.0);
    lowcutSlider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, MyColours::orange);
    lowcutSlider.setColour(juce::Slider::textBoxOutlineColourId, MyColours::vitalMidGrey);
    lowcutSlider.setLookAndFeel(&myLookAndFeel);
    addAndMakeVisible(lowcutSlider);

    addAndMakeVisible(highcutSlider);
    highcutSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    highcutSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 40, 20);
    highcutSlider.setRange(0.0, 100.0, 1.0);
    highcutSlider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, MyColours::orange);
    highcutSlider.setLookAndFeel(&myLookAndFeel);
    highcutSlider.setColour(juce::Slider::textBoxOutlineColourId, MyColours::vitalMidGrey);

    highcutAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, lowcutID, highcutSlider);
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

    // DIALS BOUNDS
    reverbDial1.setBounds(0.13067 * getWidth(), 0.2795 * getHeight(), 0.09763 * getWidth(), 0.09763 * getWidth());
    reverbDial2.setBounds(0.24505 * getWidth(), reverbDial1.getY(), reverbDial1.getWidth(), reverbDial1.getWidth());
    
    DistoDial.setBounds(0.4385 * getWidth(), reverbDial1.getY(), reverbDial1.getWidth(), reverbDial1.getWidth());
    
    lowcutSlider.setBounds(0.1208 * getWidth(), 0.6956 * getHeight(), 0.0614 * getWidth(), 0.0614 * getWidth());
    highcutSlider.setBounds(0.2032* getWidth(), 0.6956 * getHeight(), 0.0614 * getWidth(), 0.0614 * getWidth());

    filterSlider.setBounds(0.39 * getWidth(), 0.7758 * getHeight(), 0.173 * getWidth(), 0.05 * getHeight());
    LFODial.setBounds(0.678 * getWidth(), 0.2762 * getHeight(), 0.2222 * getWidth(), 0.2222 * getWidth());

    audioProcessor.waveViewerPost.setBounds(0.6795 * getWidth(), 0.765 * getHeight(), 0.238 * getWidth(), 0.081 * getHeight());

    combo.setBounds(0.404 * getWidth(), 0.6815 * getHeight(), 0.144 * getWidth(), 0.043 * getHeight());
}

void InterfaceTestAudioProcessorEditor::resized()
{
}