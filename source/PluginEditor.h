#include "PluginProcessor.h"
#include <juce_audio_processors/juce_audio_processors.h>

//==============================================================================
class PluginEditor : public juce::AudioProcessorEditor
{
public:
  explicit PluginEditor (PluginProcessor&);
  ~PluginEditor() override;

  //==============================================================================
  void paint (juce::Graphics&) override;
  void resized() override;

private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  PluginProcessor& processorRef;
  juce::Label cpuUsageLabel;
  juce::Label cpuUsageText;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};