#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
class WavetableOscillator
{
public:
    WavetableOscillator(const juce::AudioSampleBuffer& wavetableToUse);

    void setFrequency(float frequency, float sampleRate);

private:
    const juce::AudioSampleBuffer& waveTable;
    float currentIndex = 0.0f, tableDelta = 0.0f;
};