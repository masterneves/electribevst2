#pragma once

#include <JuceHeader.h>

namespace HacktribeParams
{
    static constexpr int NUM_TRACKS = 16;
    static constexpr int NUM_STEPS = 64;
}

enum class TrackMode
{
    Synth = 0,
    Sample
};

class Electribe2HacktribeAudioProcessor  : public juce::AudioProcessor
{
public:
    Electribe2HacktribeAudioProcessor();
    ~Electribe2HacktribeAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return JucePlugin_Name; }

    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return true; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return "Default"; }
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState& getAPVTS() { return apvts; }
    int getCurrentStep() const { return currentStep.load(); }

private:
    juce::AudioProcessorValueTreeState apvts;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    juce::Synthesiser synthEngine;
    
    std::atomic<int> currentStep { 0 };
    double currentSampleRate { 44100.0 };
    double bpm { 120.0 };
    juce::int64 samplesPerStep { 0 };
    juce::int64 stepSampleCounter { 0 };

    void updateSequencerTiming(double newBpm);
    void triggerStep(int step, juce::MidiBuffer& midiOutput);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Electribe2HacktribeAudioProcessor)
};
