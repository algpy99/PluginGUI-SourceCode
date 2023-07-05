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
        auto lineW = 0.4f * juce::jmin(radius * 0.2f, radius * 0.5f);
        auto arcRadius = radius - lineW;


        juce::Path buttonBackground;
        buttonBackground.addCentredArc(bounds.getCentreX(),
            bounds.getCentreY(),
            arcRadius,
            arcRadius,
            0.0f,
            juce::MathConstants<float>::pi * 1.0f,
            juce::MathConstants<float>::pi * 3.0f,
            true);
        g.setColour(MyColours::vitalMidGrey);
        g.fillPath(buttonBackground);

        juce::Path backgroundArc;
        backgroundArc.addCentredArc(bounds.getCentreX(),
            bounds.getCentreY(),
            arcRadius - 2 * lineW,
            arcRadius - 2 * lineW,
            0.0f,
            rotaryStartAngle,
            rotaryEndAngle,
            true);

        g.setColour(MyColours::vitalGrey);
        g.strokePath(backgroundArc, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        if (slider.isEnabled())
        {
            juce::Path valueArc;
            valueArc.addCentredArc(bounds.getCentreX(),
                bounds.getCentreY(),
                arcRadius - 2 * lineW,
                arcRadius - 2 * lineW,
                0.0f,
                rotaryStartAngle,
                toAngle,
                true);

            g.setColour(fill);
            g.strokePath(valueArc, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        }

        juce::Point<float> thumbPoint(  bounds.getCentreX() + (arcRadius - 1.7 * lineW) * std::cos(toAngle - juce::MathConstants<float>::halfPi),
                                        bounds.getCentreY() + (arcRadius - 1.7 * lineW) * std::sin(toAngle - juce::MathConstants<float>::halfPi));
        
        g.setColour(MyColours::cream);
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
    void initDials();

    juce::Slider dial1;
    juce::Slider dial2;


    juce::GroupComponent sectionAtmosphere;
    juce::GroupComponent sectionDrive;
    juce::GroupComponent sectionFilter;

    juce::GroupComponent sectionLFO;
    juce::GroupComponent sectionGraph;   

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InterfaceTestAudioProcessorEditor)
};