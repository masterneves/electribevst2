#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "PluginProcessor.h"

class Electribe2HacktribeAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    Electribe2HacktribeAudioProcessorEditor (Electribe2HacktribeAudioProcessor&);
    ~Electribe2HacktribeAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    Electribe2HacktribeAudioProcessor& audioProcessor;

    juce::Slider cutoffSlider;
    juce::Slider resonanceSlider;
    juce::ComboBox modeSelector;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> resonanceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> modeAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Electribe2HacktribeAudioProcessorEditor)
};
