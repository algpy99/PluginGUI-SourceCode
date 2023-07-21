/*
  ==============================================================================

    LFOGenerator.cpp
    Created: 27 Jun 2023 3:40:37pm
    Author:  goupy

  ==============================================================================
*/

#include "LFOGenerator.h"

void alex_dsp::LFOGenerator::prepare(const juce::dsp::ProcessSpec &spec)
{
    m_time = 0.0;
    sampleRate = spec.sampleRate;
    m_deltaTime = 1 / sampleRate;
}

void alex_dsp::LFOGenerator::reset()
{
    if (sampleRate <= 0) return;

    m_frequency.reset(sampleRate, 0.05);
    //m_frequency.setTargetValue(7.0);

    m_depth.reset(sampleRate, 0.05);
    //m_depth.setTargetValue(100.0);
}

void alex_dsp::LFOGenerator::setFrequency(float newFrequency)
{
    m_frequency.setTargetValue(newFrequency);
    targetFrequency = newFrequency;
}

void alex_dsp::LFOGenerator::setDepth(float newDepth)
{
    m_depth.setTargetValue(newDepth);
    depth = m_depth.getTargetValue();
    a = depth / 200.0f;
    offset = a - 1;
}

void alex_dsp::LFOGenerator::setLFOType(alex_dsp::LFOGenerator::LFOType newType)
{
    switch (newType)
    {
    case LFOType::kSine:
    {
        _type = newType;
        break;
    }
    case LFOType::kSaw:
    {
        _type = newType;
        break;
    }
    case LFOType::kSquare:
    {
        _type = newType;
        break;
    }
    }
}

void alex_dsp::LFOGenerator::process()
{      
    if (m_time >= std::numeric_limits<float>::max())
    {
        m_time = 0.0;
    }

    processSample();
}
 
void alex_dsp::LFOGenerator::processSample()
{
    switch (_type)
    {
    case LFOType::kSine:
    {
        return processSine();
        break;
    }
    case LFOType::kSaw:
    {
        return processSaw();
        break;
    }
    case LFOType::kSquare:
    {
        return processSquare();
        break;
    }
    }
}

void alex_dsp::LFOGenerator::processSine()
{
    m_LFOValue = a * sin(2 * juce::double_Pi * m_frequency.getNextValue() * m_time) + offset;
    m_time += m_deltaTime;
}

void alex_dsp::LFOGenerator::processSaw()
{
    m_LFOValue = a * ((acos(sin(2 * juce::double_Pi * m_frequency.getNextValue() * m_time)) / 1.5708f) + offset);
    m_time += m_deltaTime;
}

void alex_dsp::LFOGenerator::processSquare()
{
    m_LFOValue = a * ((sin(2 * juce::double_Pi * m_frequency.getNextValue() * m_time) >= 0.0f ? 1.0f : 0.0f)) + offset;
    m_time += m_deltaTime;
}

float alex_dsp::LFOGenerator::getCurrentLFOValue()
{
    return m_LFOValue;
}