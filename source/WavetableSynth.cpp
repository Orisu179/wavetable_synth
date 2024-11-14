#include "WavetableSynth.h"

void WavetableSynth::setSampleRate (double sampleRate)
{
    this->sampleRate = sampleRate;
}

void WavetableSynth::render (juce::AudioBuffer<float>& buffer, int curSample, int midiEventSample)
{

}

void WavetableSynth::handleMidiEvent (juce::MidiMessage midiMessage)
{
    if (midiMessage.isNoteOn())
    {
        const auto note = midiMessage.getNoteNumber();
        const auto freq = midiNoteNumberToFreq (note);
        oscillators[note]->setFrequency (note);
    }
    else if (midiMessage.isNoteOff())
    {
    }
    else if (midiMessage.isAllNotesOff())
    {
    }
}

float WavetableSynth::midiNoteNumberToFreq (const int noteNumber)
{
    constexpr auto A4_FREQ = 440.f;
    constexpr auto A4_NOTE = 69.f;
    constexpr auto SEMITONES_IN_AN_OCTAVE = 12.f;
    return A4_FREQ * std::pow (2.f, (noteNumber - A4_NOTE) / SEMITONES_IN_AN_OCTAVE);
}

juce::AudioSampleBuffer& WavetableSynth::generateSineWaveTable()
{
    constexpr auto SINE_WAVETABLE_SIZE = 64;
    constexpr auto PI = std::atan (1.f) * 4;
    juce::AudioSampleBuffer sineWaveTable;
    sineWaveTable.setSize (1, SINE_WAVETABLE_SIZE);
    float curSample = 0.f;
    for (int i = 0; i < SINE_WAVETABLE_SIZE; i++)
    {
        curSample = std::sin(2 * PI * static_cast<float>(i) / static_cast<float> (SINE_WAVETABLE_SIZE));
        sineWaveTable.setSample (0, i, curSample);
    }
    return sineWaveTable;
}

void WavetableSynth::initializeOscillators(int count)
{
    const auto waveTable = generateSineWaveTable();
    oscillators.clear ();
    for (int i = 0; i < count; i++)
    {
        WavetableOscillator osc(waveTable, sampleRate);
        oscillators.add (&osc);
    }

}
