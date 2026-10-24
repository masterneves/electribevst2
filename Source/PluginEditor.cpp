#include "PluginProcessor.h"
#include "PluginEditor.h"

Electribe2HacktribeAudioProcessorEditor::Electribe2HacktribeAudioProcessorEditor (Electribe2HacktribeAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (900, 550);

    for (int i = 0; i < 16; ++i)
    {
        pads[i].setButtonText(juce::String(i + 1));
        pads[i].setColour(juce::TextButton::buttonColourId, juce::Colour(0xff2a2a2a));
        pads[i].setColour(juce::TextButton::textColourOffId, juce::Colours::white);
        addAndMakeVisible(pads[i]);
    }

    cutoffSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    cutoffSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(cutoffSlider);

    resonanceSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    resonanceSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(resonanceSlider);

    trackModeSelector.addItem("Synth Engine", 1);
    trackModeSelector.addItem("User Sampler", 2);
    addAndMakeVisible(trackModeSelector);

    cutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "TRK_1_CUTOFF", cutoffSlider);
    resonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "TRK_1_RESONANCE", resonanceSlider);
    modeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.getAPVTS(), "TRK_1_MODE", trackModeSelector);

    startTimerHz(30);
}

Electribe2HacktribeAudioProcessorEditor::~Electribe2HacktribeAudioProcessorEditor()
{
    stopTimer();
}

void Electribe2HacktribeAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff1e1e24));

    g.setColour (juce::Colours::cyan);
    g.setFont (22.0f);
    g.drawFittedText ("KORG ELECTRIBE 2 - HACKTRIBE OS VST", getLocalBounds().removeFromTop(40), juce::Justification::centred, 1);
}

void Electribe2HacktribeAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(20);
    area.removeFromTop(40);

    auto controlArea = area.removeFromTop(120);
    trackModeSelector.setBounds(controlArea.removeFromLeft(150).withHeight(30));
    cutoffSlider.setBounds(controlArea.removeFromLeft(100).withHeight(100));
    resonanceSlider.setBounds(controlArea.removeFromLeft(100).withHeight(100));

    area.removeFromTop(20);

    auto padArea = area.removeFromTop(200);
    int padWidth = padArea.getWidth() / 8;
    int padHeight = 80;

    for (int i = 0; i < 16; ++i)
    {
        int row = i / 8;
        int col = i % 8;
        pads[i].setBounds(padArea.getX() + col * padWidth + 4, 
                           padArea.getY() + row * (padHeight + 10), 
                           padWidth - 8, padHeight);
    }
}

void Electribe2HacktribeAudioProcessorEditor::timerCallback()
{
    int currentStep = audioProcessor.getCurrentStep();
    
    for (int i = 0; i < 16; ++i)
    {
        if (i == (currentStep % 16))
        {
            pads[i].setColour(juce::TextButton::buttonColourId, juce::Colours::red);
        }
        else
        {
            pads[i].setColour(juce::TextButton::buttonColourId, juce::Colour(0xff2a2a2a));
        }
    }
}
