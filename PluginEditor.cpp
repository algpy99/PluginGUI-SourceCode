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

    
    //initDial2();
    //initButton();

    addAndMakeVisible(sectionAmbience);
    sectionAmbience.setText("Ambience");
    sectionAmbience.setColour(juce::GroupComponent::ColourIds::outlineColourId,MyColours::bossMidGrey);
    sectionAmbience.setColour(juce::GroupComponent::ColourIds::textColourId , MyColours::bossMidGrey);

    addAndMakeVisible(sectionDrive);
    sectionDrive.setText("Drive");
    sectionDrive.setColour(juce::GroupComponent::ColourIds::outlineColourId, MyColours::bossMidGrey);

    addAndMakeVisible(sectionFilter);
    sectionFilter.setText("Filter");
    sectionFilter.setColour(juce::GroupComponent::ColourIds::outlineColourId, MyColours::bossMidGrey);

    addAndMakeVisible(sectionLFO);
    sectionLFO.setText("LFO");
    sectionLFO.setColour(juce::GroupComponent::ColourIds::outlineColourId, MyColours::green);

    addAndMakeVisible(sectionGraph);
    sectionGraph.setText("Output");
    sectionGraph.setColour(juce::GroupComponent::ColourIds::outlineColourId, MyColours::green);
    
}

void InterfaceTestAudioProcessorEditor::initWindow()
{
    auto width = 1500;
    auto height = 750;
    float ratio = 2;
    setResizable(true, true);
    setResizeLimits(0.5 * width, 0.5 * height, 0.8 * width, 0.8 * height);
    getConstrainer()->setFixedAspectRatio(ratio);
    setSize(0.5 * width, 0.5 * height);
}

void InterfaceTestAudioProcessorEditor::initDial()
{
    addAndMakeVisible(dial);
    dial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    dial.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    //dial.setRange(0.0, 100.0, 0.1);
    dial.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, MyColours::orange);

    dial.setLookAndFeel(&myLookAndFeel);

    //juce::GlowEffect glowEffect;
    //glowEffect.setGlowProperties(30, juce::Colour(0xffe4e2dd), juce::Point<int>(2, 2));
    //dial.setComponentEffect(&glowEffect);

    //dial.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, MyColours::cream);
    //dial.setColour(juce::Slider::ColourIds::thumbColourId,MyColours::cream);
    //dial.setLookAndFeel(new MyLookAndFeel());

    /*
    addAndMakeVisible(dialLabel);
    dialLabel.setText("Ambience", juce::dontSendNotification);
    dialLabel.attachToComponent(&dial, false);
    dialLabel.setJustificationType(juce::Justification::centred);
    dialLabel.setFont(juce::Font(dialLabel.getFont().withHeight(24.0f)));
    */
}

/*
void InterfaceTestAudioProcessorEditor::initButton()
{
    addAndMakeVisible(button);
    button.setButtonText("Hi");
    button.setClickingTogglesState(true);
    button.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colour(0xff0e8f51));
}
*/

InterfaceTestAudioProcessorEditor::~InterfaceTestAudioProcessorEditor()
{
}

//==============================================================================
void InterfaceTestAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //background = juce::ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);
    //g.drawImageWithin(background, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);


    g.fillAll (MyColours::bossBlack);

    //g.setColour(juce::Colour(0xffe4e2dd));
    //g.setFont (36.0f);
    //g.drawFittedText ("Stutter Plugin", getLocalBounds(), juce::Justification::centred, 1);
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

    auto dialSize = blockW * 0.6;

    sectionAmbience.setBounds(  margin, 
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

    dial.setBounds(margin + 0.2 * (blockW), top + margin, dialSize, dialSize);
    //dial2.setBounds(border1.getX() + leftMargin + border1.getWidth() * 0.5, topMargin + 0.12 * border1.getHeight(), dialSize, dialSize);
    

    //dial.setBounds(getLocalBounds());
}
