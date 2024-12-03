/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WavetableSynthAudioProcessor::WavetableSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor (BusesProperties()
    #if !JucePlugin_IsMidiEffect
        #if !JucePlugin_IsSynth
              .withInput ("Input", juce::AudioChannelSet::stereo(), true)
        #endif
              .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
    #endif
              ),
      apvts (*this, nullptr, juce::Identifier ("Wavetable Synth"), createParameterLayout())
#endif
{
    apvts.state.addListener (this);
}

WavetableSynthAudioProcessor::~WavetableSynthAudioProcessor()
{
    apvts.state.removeListener (this);
}

//==============================================================================
const juce::String WavetableSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WavetableSynthAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool WavetableSynthAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool WavetableSynthAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double WavetableSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WavetableSynthAudioProcessor::getNumPrograms()
{
    return 1;
}

int WavetableSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WavetableSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String WavetableSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void WavetableSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void WavetableSynthAudioProcessor::prepareToPlay (double sampleRate, int)
{
    synth.prepareToPlay (sampleRate);
    parametersChanged.store (true);
}

void WavetableSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WavetableSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
    #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

        // This checks if the input layout matches the output layout
        #if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
        #endif

    return true;
    #endif
}
#endif

void WavetableSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    for (auto i = 0; i < buffer.getNumChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    bool expected = true;
    if (parametersChanged.compare_exchange_strong (expected, false))
    {
        synth.setGain (*apvts.getRawParameterValue ("gain"));
        float attack = *apvts.getRawParameterValue ("attack");
        float decay = *apvts.getRawParameterValue ("decay");
        float sustain = *apvts.getRawParameterValue ("sustain");
        float release = *apvts.getRawParameterValue ("release");
        synth.setADSR (attack, decay, sustain, release);
    }
    synth.processBlock (buffer, midiMessages);
}

//==============================================================================
bool WavetableSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* WavetableSynthAudioProcessor::createEditor()
{
    //    return new WavetableSynthAudioProcessorEditor (*this);
    auto editor = new juce::GenericAudioProcessorEditor (*this);
    editor->setSize (500, 1050);
    return editor;
}

//==============================================================================
void WavetableSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    copyXmlToBinary (*apvts.copyState().createXml(), destData);
}

void WavetableSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml (getXmlFromBinary (data, sizeInBytes));
    if (xml.get() != nullptr && xml->hasTagName (apvts.state.getType()))
    {
        apvts.replaceState (juce::ValueTree::fromXml (*xml));
        parametersChanged.store (true);
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout WavetableSynthAudioProcessor::createParameterLayout()
{
    return {
        std::make_unique<juce::AudioParameterFloat> (
            ParameterID::gain,
            "Gain",
            juce::NormalisableRange<float> (-48.0f, 0.0f, 0.1f),
            -15.0f,
            juce::AudioParameterFloatAttributes().withLabel ("db")),
        std::make_unique<juce::AudioParameterChoice> (
            ParameterID::type,
            "WaveTable Type",
            juce::StringArray { "Sine", "Sawtooth", "Triangle" },
            0),
        std::make_unique<juce::AudioParameterFloat> (
            ParameterID::attack,
            "Attack",
            juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
            0.1f),
        std::make_unique<juce::AudioParameterFloat> (
            ParameterID::decay,
            "Decay",
            juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
            0.5f),
        std::make_unique<juce::AudioParameterFloat> (
            ParameterID::sustain,
            "Sustain",
            juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
            1.0f),
        std::make_unique<juce::AudioParameterFloat> (
            ParameterID::release,
            "Release",
            juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
            0.3f),
    };
}
void WavetableSynthAudioProcessor::valueTreePropertyChanged (juce::ValueTree&, const juce::Identifier&)
{
    parametersChanged.store (true);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WavetableSynthAudioProcessor();
}
