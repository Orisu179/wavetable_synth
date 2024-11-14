#include "WavetableOscillator.h"

WavetableOscillator::WavetableOscillator (const juce::AudioSampleBuffer& wavetableToUse, double sampleRate)
    : sampleRate (sampleRate), waveTable(wavetableToUse)
{
}

void WavetableOscillator::setFrequency (const float frequency)
{
    const auto tableSizeOverSampleRate = static_cast<float> (waveTable.getNumSamples() / sampleRate);
    tableDelta = frequency * tableSizeOverSampleRate;
}
float WavetableOscillator::getNextSample()
{
    auto tableSize = static_cast<unsigned int>(waveTable.getNumSamples());
    const auto sample = interpolateLinearly();
    currentIndex += tableDelta;
    currentIndex = std::fmod (currentIndex, tableSize);
    return sample;
}

float WavetableOscillator::interpolateLinearly()
{
    const auto intIndex = static_cast<int> (currentIndex);
    const auto nextIndex = (intIndex + 1) % waveTable.getNumSamples();

    const auto nextIndexWeight = currentIndex - static_cast<float>(intIndex);
    const auto intIndexWeight = 1.f - nextIndexWeight;

    return intIndexWeight * waveTable.getSample (0, intIndex)
        + nextIndexWeight * waveTable.getSample (0, nextIndex);
}

void WavetableOscillator::stop()
{
    currentIndex = 0.f;
    tableDelta = 0.f;
}

bool WavetableOscillator::isPlaying() const
{
    return tableDelta != 0.f;
}

