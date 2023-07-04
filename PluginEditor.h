/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MyColours.h"

class MyLookAndFeel : public juce::LookAndFeel_V4
{
public:
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override
    {
        auto outline = slider.findColour(juce::Slider::rotarySliderOutlineColourId);
        auto fill = slider.findColour(juce::Slider::rotarySliderFillColourId);

        auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat().reduced(10);

        auto radius = juce::jmin(width, height) / 2.0f;
        auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        auto lineW = 0.4f * juce::jmin(8.0f, radius * 0.5f);
        auto arcRadius = radius - lineW;

        juce::Path backgroundArc;
        backgroundArc.addCentredArc(bounds.getCentreX(),
            bounds.getCentreY(),
            arcRadius,
            arcRadius,
            0.0f,
            rotaryStartAngle,
            rotaryEndAngle,
            true);

        g.setColour(MyColours::bossMidGrey);
        g.strokePath(backgroundArc, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        if (slider.isEnabled())
        {
            juce::Path valueArc;
            valueArc.addCentredArc(bounds.getCentreX(),
                bounds.getCentreY(),
                arcRadius,
                arcRadius,
                0.0f,
                rotaryStartAngle,
                toAngle,
                true);

            g.setColour(fill);
            g.strokePath(valueArc, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        }

        auto thumbWidth = lineW * 2.0f;
        juce::Point<float> thumbPoint(  bounds.getCentreX() + (arcRadius - 10.0f) * std::cos(toAngle - juce::MathConstants<float>::halfPi),
                                        bounds.getCentreY() + (arcRadius - 10.0f) * std::sin(toAngle - juce::MathConstants<float>::halfPi));
        g.setColour(MyColours::orange);
        g.drawLine(backgroundArc.getBounds().getCentreX(), backgroundArc.getBounds().getCentreY(), thumbPoint.getX(), thumbPoint.getY(), lineW * 1.2);
    }

};

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

    MyLookAndFeel myLookAndFeel;

    void initWindow();
    void initDial();
    //void initDial2();
    //void initButton();

    juce::Slider dial;

    /*
    juce::Slider dial2;

    juce::TextButton button;

    //juce::Label dialLabel;
    */

    juce::GroupComponent sectionAmbience;
    juce::GroupComponent sectionDrive;
    juce::GroupComponent sectionFilter;

    juce::GroupComponent sectionLFO;
    juce::GroupComponent sectionGraph;
    

    //juce::Image background;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InterfaceTestAudioProcessorEditor)
};