#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
class WavetableOscillator
{
public:
    WavetableOscillator(const juce::AudioSampleBuffer& wavetableToUse, double sampleRate);

    void setFrequency(float frequency);
    float getNextSample();

private:
    float sampleRate;
    const juce::AudioSampleBuffer& waveTable;
    float currentIndex = 0.0f, tableDelta = 0.0f;

    float interpolateLinearly();
    void stop();
    bool isPlaying();
};