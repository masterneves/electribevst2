#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class Electribe2HacktribeAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                                  private juce::Timer
{
public:
    Electribe2HacktribeAudioProcessorEditor (Electribe2HacktribeAudioProcessor&);
    ~Electribe2HacktribeAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    Electribe2HacktribeAudioProcessor& audioProcessor;

    juce::TextButton pads[16];
    juce::Slider cutoffSlider;
    juce::Slider resonanceSlider;
    juce::ComboBox trackModeSelector;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> resonanceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> modeAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Electribe2HacktribeAudioProcessorEditor)
};
