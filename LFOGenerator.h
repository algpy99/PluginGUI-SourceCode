/*
  ==============================================================================

    LFOGenerator.h
    Created: 27 Jun 2023 3:40:37pm
    Author:  goupy

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace alex_dsp
{
class LFOGenerator
{
public:

    void prepare(const juce::dsp::ProcessSpec& spec);

    void reset();

    struct Parameters
    {
        //juce::SmoothedValue<float> m_frequency;
        //float m_depth = 100.0f;
    };

    void setFrequency(float newFrequency);

    void setDepth(float newDepth);

    void process();

    void processSample();

    void processSine();

    void processSaw();

    void processSquare();

    float getCurrentLFOValue();

    void setParameter(Parameters parameter, float parameterValue);

    enum class LFOType
    {
        kSine,
        kSaw,
        kSquare
    };

    void setLFOType(LFOType newType);



private:

    float sampleRate;

    juce::SmoothedValue<float> m_frequency;
    juce::SmoothedValue<float> m_depth;
    //juce::SmoothedValue<float> m_LFOValue;

    float frequency;
    float depth;
    float a;
    float offset;

    float m_time;
    float m_deltaTime;
    float m_GlobalBypass{ false };

    float m_LFOValue;
    float targetFrequency = 0.0f;

    LFOType _type = LFOType::kSaw;
};

}

