/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WavetableSynthAudioProcessorEditor::WavetableSynthAudioProcessorEditor (WavetableSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    gainValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "gain", gainSlider);
    setSize (600, 400);
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, 100, 25);
    gainSlider.setRange(-48.0f, 0.0f);
    gainSlider.setValue(-15.0f);
}

WavetableSynthAudioProcessorEditor::~WavetableSynthAudioProcessorEditor()
{

}

//==============================================================================
void WavetableSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Wavetable Synth", getLocalBounds().removeFromTop(10.f), juce::Justification::centred, 1);
}

void WavetableSynthAudioProcessorEditor::resized()
{
    addAndMakeVisible(gainSlider);
}
