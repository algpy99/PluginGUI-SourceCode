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
        auto rotaryParams = slider.getRotaryParameters();
        rotaryParams.startAngleRadians = juce::MathConstants<float>::pi * 1.15f;
        rotaryParams.endAngleRadians = juce::MathConstants<float>::pi * 2.85f;
        slider.setRotaryParameters(rotaryParams);

        auto outline = slider.findColour(juce::Slider::rotarySliderOutlineColourId);
        auto fill = slider.findColour(juce::Slider::rotarySliderFillColourId);

        auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat();

        auto centre = bounds.getCentre();

        auto radius = juce::jmin(width, height) / 2.0f;
        auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        auto lineW = 0.4f * juce::jmin(radius * 0.2f, radius * 0.5f);
        auto arcRadius = radius;


        juce::Path buttonBackground;
        buttonBackground.addCentredArc(bounds.getCentreX(),
            bounds.getCentreY(),
            arcRadius,
            arcRadius,
            0.0f,
            rotaryStartAngle,
            rotaryEndAngle,
            true);
        g.setColour(MyColours::vitalMidGrey);
        //g.fillPath(buttonBackground);

        juce::Path backgroundArc;
        backgroundArc.addCentredArc(bounds.getCentreX(),
            bounds.getCentreY(),
            arcRadius - 2 * lineW,
            arcRadius - 2 * lineW,
            0.0f,
            rotaryStartAngle,
            rotaryEndAngle,
            true);

        g.setColour(MyColours::vitalMidGrey);
        g.strokePath(backgroundArc, juce::PathStrokeType(lineW));

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
            g.strokePath(valueArc, juce::PathStrokeType(lineW));
        }

        juce::Path dialTick;
        dialTick.startNewSubPath(centre.getPointOnCircumference(0.45 * radius, toAngle));
        g.setColour(fill);
        
        if (toAngle == rotaryStartAngle)
        {
            g.setColour(MyColours::vitalMidGrey);
        }
        
        /** Dial tick length*/
        dialTick.lineTo(centre.getPointOnCircumference(0.65 * radius, toAngle));
        
        /** Dial tick thickness*/
        g.strokePath(dialTick, juce::PathStrokeType(lineW * 0.6, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));


    }

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle style, juce::Slider& slider) override
    {
        if (style == juce::Slider::LinearHorizontal)
        {
            auto bounds = juce::Rectangle<float>(x, y, width, height);

            juce::Rectangle<float> sliderOuterPath{ bounds.getX(), bounds.getY(), bounds.getWidth(), 0.4f * bounds.getHeight() };
            g.setColour(MyColours::vitalMidGrey);
            g.fillRoundedRectangle(sliderOuterPath, 0.2f);


            juce::Rectangle<float> sliderPath{ bounds.getX(), bounds.getY(), sliderPos - bounds.getX(), 0.4f * bounds.getHeight() };
            g.setColour(MyColours::green);
            g.fillRoundedRectangle(sliderPath, 0.2f);


            juce::Rectangle<float> sliderThumb{ sliderPos, bounds.getY() + 0.5f * bounds.getHeight(), 0.01f * bounds.getWidth(), 0.5f * bounds.getHeight() };
            g.setColour(MyColours::green);

            if (sliderPos == bounds.getX())
            {
                g.setColour(MyColours::vitalMidGrey);
            }
            g.fillRoundedRectangle(sliderThumb, 0.2f);
        }

        if (style == juce::Slider::TwoValueHorizontal)
        {
            auto bounds = juce::Rectangle<float>(x, y, width, height);

            juce::Rectangle<float> sliderOuterPath{ bounds.getX(), bounds.getY(), bounds.getWidth(), 0.4f * bounds.getHeight() };
            g.setColour(MyColours::vitalMidGrey);
            g.fillRoundedRectangle(sliderOuterPath, 0.2f);


            juce::Rectangle<float> sliderPath{ minSliderPos, bounds.getY(), maxSliderPos - minSliderPos, 0.4f * bounds.getHeight()};
            g.setColour(MyColours::orange);
            g.fillRoundedRectangle(sliderPath, 0.2f);


            juce::Rectangle<float> sliderMin { minSliderPos, bounds.getY() + 0.5f * bounds.getHeight(), 0.01f * bounds.getWidth(), 0.5f * bounds.getHeight() };
            g.setColour(MyColours::orange);

            if (minSliderPos == bounds.getX())
            {
                g.setColour(MyColours::vitalMidGrey);
            }
            g.fillRoundedRectangle(sliderMin, 0.2f);

            juce::Rectangle<float> sliderMax{ maxSliderPos - 0.01f * bounds.getWidth(), bounds.getY() + 0.5f * bounds.getHeight(), 0.01f * bounds.getWidth(), 0.5f * bounds.getHeight() };
            g.setColour(MyColours::orange);

            if (sliderMax.getRight() == bounds.getRight())
            {
                g.setColour(MyColours::vitalGrey);
            }
            g.fillRoundedRectangle(sliderMax, 0.2f);
        }
    }

    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour, bool, bool isButtonDown) override
    {
        auto buttonArea = button.getLocalBounds();
        //auto buttonArea = juce::Rectangle<float> (1.0f * button.getX(), 1.0f * button.getY(), 1.0f * button.getWidth(), 1.0f * button.getHeight());
        g.setColour(backgroundColour);
        //g.setCornerRadius
        g.fillRect(buttonArea);
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

    juce::Image background;

    void initWindow();
    void initDials();

    float top;
    float margin;
    float separation;

    juce::Rectangle<float> topBorder;
    juce::Rectangle<float> sectionFX;
    juce::Rectangle<float> sectionLFO;
    juce::Rectangle<float> sectionMix;
    juce::Rectangle<float> bottomBorder;

    juce::Rectangle<float> lineFX;

    juce::Slider reverbDial1;
    juce::Slider reverbDial2;
    juce::Slider DistoDial;
    juce::Slider filterSlider;
    juce::Slider LFODial;
    juce::Slider doubleSlider;

    juce::TextButton buttonSine;
    juce::TextButton buttonSaw;
    juce::TextButton buttonSquare;

    enum RadioButtonIds
    {
        Waves = 1001
    };

    /*
    juce::TextButton buttonSine;
    juce::TextButton buttonSaw;
    juce::TextButton buttonSquare;
    */

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InterfaceTestAudioProcessorEditor)
};