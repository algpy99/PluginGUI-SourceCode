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
        slider.setDoubleClickReturnValue(true, 0.0);
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

        dialTick.lineTo(centre.getPointOnCircumference(0.65 * radius, toAngle));
        g.strokePath(dialTick, juce::PathStrokeType(lineW * 0.6, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // Custom value display
        juce::String valueString = slider.getTextFromValue(slider.getValue());
        float fontSize = 0.2f * juce::jmin(width, height);
        if (juce::jmin(width, height) > 120)
        {
            fontSize = 0.1f * juce::jmin(width, height);
        }
        juce::Font font;
        font.setHeight(fontSize);
        g.setFont(font);
        int textWidth = font.getStringWidth(valueString);
        int centerX = x + (width - textWidth) / 2;
        int centerY = y + (height - font.getHeight()) / 2;
        g.setColour(fill);
        g.drawText(valueString, centerX, centerY, textWidth, font.getHeight(), juce::Justification::centred, false);

    }

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle style, juce::Slider& slider) override
    {
        if (style == juce::Slider::LinearHorizontal)
        {
            auto bounds = juce::Rectangle<float>(x, y, width, height);

            juce::Rectangle<float> sliderOuterPath{ bounds.getX(), bounds.getY() + 0.4f * bounds.getHeight(), bounds.getWidth(), 0.15f * bounds.getHeight() };
            g.setColour(MyColours::vitalMidGrey);
            g.fillRoundedRectangle(sliderOuterPath, 0.15f);


            juce::Rectangle<float> sliderPath{ bounds.getX(), bounds.getY() + 0.4f * bounds.getHeight(), sliderPos - bounds.getX(), 0.15f * bounds.getHeight() };
            g.setColour(MyColours::green);
            g.fillRoundedRectangle(sliderPath, 0.15f);


            juce::Rectangle<float> sliderThumb{ sliderPos, bounds.getY() + 0.6f * bounds.getHeight(), 0.01f * bounds.getWidth(), 0.3f * bounds.getHeight() };
            g.setColour(MyColours::green);            
            g.fillRoundedRectangle(sliderThumb, 0.15f);

            // Custom value display
            juce::String valueString = slider.getTextFromValue(slider.getValue());
            float fontSize = 0.4f * juce::jmin(width, height);
            juce::Font font;
            font.setHeight(fontSize);
            g.setFont(font);
            int textWidth = font.getStringWidth(valueString);
            g.setColour(MyColours::green);
            g.drawText(valueString, bounds.getX(), bounds.getY() - 0.1f * bounds.getHeight(), textWidth, font.getHeight(), juce::Justification::centred, false);

        }

        if (style == juce::Slider::TwoValueHorizontal)
        {
            auto bounds = juce::Rectangle<float>(x, y, width, height);

            juce::Rectangle<float> sliderOuterPath{ bounds.getX(), bounds.getY() + 0.4f * bounds.getHeight(), bounds.getWidth(), 0.15f * bounds.getHeight() };
            g.setColour(MyColours::vitalMidGrey);
            g.fillRoundedRectangle(sliderOuterPath, 0.2f);


            juce::Rectangle<float> sliderPath{ minSliderPos, bounds.getY() + 0.4f * bounds.getHeight(), maxSliderPos - minSliderPos, 0.15f * bounds.getHeight() };
            g.setColour(MyColours::orange);
            
            if ((maxSliderPos - minSliderPos) == bounds.getWidth())
            {
                g.setColour(MyColours::vitalMidGrey);
            }
            g.fillRoundedRectangle(sliderPath, 0.2f);

            juce::Rectangle<float> sliderMin { minSliderPos, bounds.getY() + 0.6f * bounds.getHeight(), 0.01f * bounds.getWidth(), 0.3f * bounds.getHeight() };
            g.setColour(MyColours::orange);            
            g.fillRoundedRectangle(sliderMin, 0.2f);

            juce::Rectangle<float> sliderMax{ maxSliderPos - 0.01f * bounds.getWidth(), bounds.getY() + 0.6f * bounds.getHeight(), 0.01f * bounds.getWidth(), 0.3f * bounds.getHeight() };
            g.setColour(MyColours::orange);
            
            g.fillRoundedRectangle(sliderMax, 0.2f);

            // Custom value display
            juce::String minValueString = slider.getTextFromValue(slider.getMinValue());
            float fontSize = 0.4f * juce::jmin(width, height);
            juce::Font font;
            font.setHeight(fontSize);
            g.setFont(font);
            int textWidth = font.getStringWidth(minValueString);
            g.setColour(MyColours::orange);
            g.drawText(minValueString, bounds.getX() - 0.01f * bounds.getWidth(), bounds.getY() - 0.1f * bounds.getHeight(), textWidth, font.getHeight(), juce::Justification::centred, false);
            juce::String maxValueString = slider.getTextFromValue(slider.getMaxValue());
            g.setColour(MyColours::orange);
            g.drawText(maxValueString, bounds.getRight() - 0.05f * bounds.getWidth(), bounds.getY() - 0.1f * bounds.getHeight(), textWidth, font.getHeight(), juce::Justification::centred, false);

        }
    }

    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour, bool, bool isButtonDown) override
    {
        auto buttonArea = button.getLocalBounds();
        button.setColour(juce::TextButton::ColourIds::buttonOnColourId, MyColours::vitalMidGrey);
        button.setColour(juce::TextButton::ColourIds::textColourOnId, MyColours::green);
        button.setColour(juce::TextButton::ColourIds::buttonColourId, MyColours::vitalMidGrey);
        button.setColour(juce::TextButton::ColourIds::textColourOffId, MyColours::vitalGrey);
        g.setColour(backgroundColour);
        g.fillRect(buttonArea);
    }
};

//==============================================================================
/**
*/
class InterfaceTestAudioProcessorEditor  : public juce::AudioProcessorEditor//, juce::AudioProcessorEditor::Listener
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

    std::vector<juce::Slider*> dials =
    {
        &reverbDial1,
        &reverbDial2,
        &DistoDial,
        &filterSlider,
        &LFODial,
        &doubleSlider,
    };

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> roomsizeAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dampingAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttach;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> frequencyAttach;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> sineAttach;
    //std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> sawAttach;
    //std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> squareAttach;

    std::unique_ptr<juce::ParameterAttachment> lowcutAttach;
    std::unique_ptr<juce::ParameterAttachment> highcutAttach;
    //std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highcutAttach;

    enum RadioButtonIds
    {
        Waves = 1001
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InterfaceTestAudioProcessorEditor)
};