#pragma once
#include "juce_audio_basics/juce_audio_basics.h"

#include "WavetableOscillator.h"

class WavetableSynth
{
public:
    void prepareToPlay(double sampleRate);
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    void setADSR(float attack, float decay, float sustain, float release);
    void setGain(float gain);
    enum oscType{
        Sine,
        Sawtooth,
        Triangle
    };
    void setType(juce::String type);

private:
    static std::vector<float> generateSineWaveTable();
    static std::vector<float> genereateSawWaveTable();
    static std::vector<float> genereateSquareWaveTable();
    static float midiNoteNumberToFrequency(int midiNoteNumber);
    void initializeOscillators();
    void handleMidiEvent(const juce::MidiMessage& midiMessage);
    void render(juce::AudioBuffer<float>& buffer, int beginSample, int endSample);

    double curSampleRate;
    float gain;
    std::vector<WavetableOscillator> oscillators;

    juce::ADSR adsr;
    juce::ADSR::Parameters AdsrParams;
    oscType type;
};

