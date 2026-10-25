#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Electribevst2AudioProcessorEditor::Electribevst2AudioProcessorEditor (Electribevst2AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Configuração do Slider de Cutoff
    cutoffSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    cutoffSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible (cutoffSlider);
    cutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.apvts, "CUTOFF", cutoffSlider);

    // Configuração do Slider de Resonance
    resonanceSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    resonanceSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible (resonanceSlider);
    resonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.apvts, "RESONANCE", resonanceSlider);

    // Configuração da Caixa de Modos do Filtro
    modeBox.addItem ("Lowpass", 1);
    modeBox.addItem ("Highpass", 2);
    modeBox.addItem ("Bandpass", 3);
    addAndMakeVisible (modeBox);
    modeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (
        audioProcessor.apvts, "MODE", modeBox);

    setSize (400, 300);
}

Electribevst2AudioProcessorEditor::~Electribevst2AudioProcessorEditor()
{
}

//==============================================================================
void Electribevst2AudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (16.0f);
    g.drawFittedText ("Electribe 2 Hacktribe", getLocalBounds().removeFromTop (40), juce::Justification::centred, 1);
}

void Electribevst2AudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced (20);
    area.removeFromTop (40);

    auto topRow = area.removeFromTop (120);
    cutoffSlider.setBounds (topRow.removeFromLeft (180));
    resonanceSlider.setBounds (topRow.removeFromRight (180));

    modeBox.setBounds (area.removeFromTop(40).reduced(40, 0));
}
