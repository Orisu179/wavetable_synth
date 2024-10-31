#include "WavetableOscillator.h"

WavetableOscillator::WavetableOscillator (const juce::AudioSampleBuffer& wavetableToUse)
    : waveTable(wavetableToUse)
{
    jassert(waveTable.getNumChannels() == 1);
}

void WavetableOscillator::setFrequency (float frequency, float sampleRate)
{
    auto tableSizeOverSampleRate = static_cast<float>(waveTable.getNumSamples() / sampleRate);
    tableDelta = frequency * tableSizeOverSampleRate;
}
