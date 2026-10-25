#include "PluginProcessor.h"
#include "PluginEditor.h"

Electribe2HacktribeAudioProcessorEditor::Electribe2HacktribeAudioProcessorEditor (Electribe2HacktribeAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (600, 400);

    // Cutoff Slider (Corrigido para o ID exato do Processor)
    cutoffSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    cutoffSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible (cutoffSlider);
    cutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "TRK_1_CUTOFF", cutoffSlider);

    // Resonance Slider (Corrigido para o ID exato do Processor)
    resonanceSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    resonanceSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible (resonanceSlider);
    resonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "TRK_1_RESONANCE", resonanceSlider);

    // Mode Selector (Corrigido para o ID exato do Processor)
    modeSelector.addItem ("Synth Engine", 1);
    modeSelector.addItem ("User Sampler", 2);
    addAndMakeVisible (modeSelector);
    modeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (audioProcessor.apvts, "TRK_1_MODE", modeSelector);
}

Electribe2HacktribeAudioProcessorEditor::~Electribe2HacktribeAudioProcessorEditor()
{
}

void Electribe2HacktribeAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::darkgrey);

    g.setColour (juce::Colours::white);
    g.setFont (18.0f);
    g.drawFittedText ("Electribe 2 / Hacktribe VST", getLocalBounds().removeFromTop(40), juce::Justification::centred, 1);
}

void Electribe2HacktribeAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(20);
    area.removeFromTop(40); // Espaço para o título

    auto sliderArea = area.removeFromTop(150);
    cutoffSlider.setBounds (sliderArea.removeFromLeft(150));
    resonanceSlider.setBounds (sliderArea.removeFromLeft(150));

    modeSelector.setBounds (area.removeFromTop(30).withSizeKeepingCentre(150, 30));
}
