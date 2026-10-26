#pragma once


#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"

//==============================================================================
class Electribevst2AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    Electribevst2AudioProcessorEditor (Electribevst2AudioProcessor&);
    ~Electribevst2AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    Electribevst2AudioProcessor& audioProcessor;

    juce::Slider cutoffSlider;
    juce::Slider resonanceSlider;
    juce::ComboBox modeBox;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> resonanceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> modeAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Electribevst2AudioProcessorEditor)
};
