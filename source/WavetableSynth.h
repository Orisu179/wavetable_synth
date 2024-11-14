#pragma once
#include "WavetableOscillator.h"
#include "juce_audio_basics/juce_audio_basics.h"

class WavetableSynth
{
public:
    WavetableSynth();
    void setSampleRate (double sampleRate);
    void render(juce::AudioBuffer<float>& buffer, int curSample, int midiEventSample);
    void handleMidiEvent(juce::MidiMessage midiMessage);
    void initializeOscillators(int count);

private:
    static float midiNoteNumberToFreq(int);
    juce::OwnedArray<WavetableOscillator> oscillators;
    double sampleRate;

    juce::AudioSampleBuffer& generateSineWaveTable();
};
