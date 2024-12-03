/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "WavetableSynth.h"
#include "juce_audio_processors/juce_audio_processors.h"

namespace ParameterID
{
#define PARAMETER_ID(str) const juce::ParameterID str (#str, 1);
    PARAMETER_ID (gain)
    PARAMETER_ID (attack)
    PARAMETER_ID (decay)
    PARAMETER_ID (sustain)
    PARAMETER_ID (release)
    PARAMETER_ID (type)
#undef PARAMETER_ID
}
//==============================================================================
/**
*/
class WavetableSynthAudioProcessor : public juce::AudioProcessor,
                                     private juce::ValueTree::Listener
{
public:
    //==============================================================================
    WavetableSynthAudioProcessor();
    ~WavetableSynthAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState apvts;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavetableSynthAudioProcessor)
    WavetableSynth synth;
    std::atomic<bool> parametersChanged { false };
    juce::AudioParameterFloat* gainParam;
    juce::AudioParameterFloat* attackParam;
    juce::AudioParameterFloat* delayParam;
    juce::AudioParameterFloat* sustainParam;
    juce::AudioParameterFloat* releaseParam;
    juce::AudioParameterChoice* typeParam;

    void valueTreePropertyChanged(juce::ValueTree&, const juce::Identifier&) override;
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
};
