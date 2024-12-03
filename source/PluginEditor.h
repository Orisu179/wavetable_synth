/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "PluginProcessor.h"

//==============================================================================
/**
*/
class WavetableSynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    WavetableSynthAudioProcessorEditor (WavetableSynthAudioProcessor&);
    ~WavetableSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainValue;
//    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackValue;
//    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayValue;
//    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainValue;
//    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseValue;

private:
    WavetableSynthAudioProcessor& audioProcessor;
    juce::Slider gainSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavetableSynthAudioProcessorEditor)
};
