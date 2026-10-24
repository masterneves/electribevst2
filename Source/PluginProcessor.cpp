#include "PluginProcessor.h"
#include "PluginEditor.h"

Electribe2HacktribeAudioProcessor::Electribe2HacktribeAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
       apvts(*this, nullptr, "Parameters", createParameterLayout())
#endif
{
}

Electribe2HacktribeAudioProcessor::~Electribe2HacktribeAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout Electribe2HacktribeAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    for (int i = 0; i < HacktribeParams::NUM_TRACKS; ++i)
    {
        juce::String prefix = "TRK_" + juce::String(i + 1) + "_";

        params.push_back(std::make_unique<juce::AudioParameterChoice>(
            prefix + "MODE", "Track " + juce::String(i + 1) + " Mode",
            juce::StringArray{ "Synth Engine", "User Sampler" }, 0));

        params.push_back(std::make_unique<juce::AudioParameterFloat>(prefix + "CUTOFF", "Cutoff", 20.0f, 20000.0f, 10000.0f));
        params.push_back(std::make_unique<juce::AudioParameterFloat>(prefix + "RESONANCE", "Resonance", 0.0f, 1.0f, 0.1f));
        params.push_back(std::make_unique<juce::AudioParameterFloat>(prefix + "ATTACK", "Attack", 0.001f, 5.0f, 0.01f));
        params.push_back(std::make_unique<juce::AudioParameterFloat>(prefix + "DECAY", "Decay", 0.01f, 10.0f, 0.5f));
        params.push_back(std::make_unique<juce::AudioParameterFloat>(prefix + "PAN", "Pan", -1.0f, 1.0f, 0.0f));
        params.push_back(std::make_unique<juce::AudioParameterFloat>(prefix + "LEVEL", "Level", 0.0f, 1.0f, 0.8f));
    }

    params.push_back(std::make_unique<juce::AudioParameterFloat>("MASTER_FX_X", "Master FX X", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MASTER_FX_Y", "Master FX Y", 0.0f, 1.0f, 0.5f));

    return { params.begin(), params.end() };
}

void Electribe2HacktribeAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    synthEngine.setCurrentPlaybackSampleRate(sampleRate);
    updateSequencerTiming(bpm);
}

void Electribe2HacktribeAudioProcessor::releaseResources()
{
}

bool Electribe2HacktribeAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    return true;
}

void Electribe2HacktribeAudioProcessor::updateSequencerTiming(double newBpm)
{
    bpm = newBpm;
    double secondsPerStep = (60.0 / bpm) / 4.0;
    samplesPerStep = static_cast<juce::int64>(secondsPerStep * currentSampleRate);
}

void Electribe2HacktribeAudioProcessor::triggerStep(int step, juce::MidiBuffer& midiOutput)
{
    juce::ignoreUnused(step, midiOutput);
}

void Electribe2HacktribeAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    if (auto* playHead = getPlayHead())
    {
        juce::AudioPlayHead::CurrentPositionInfo posInfo;
        if (playHead->getCurrentPosition(posInfo))
        {
            if (posInfo.bpm > 0 && posInfo.bpm != bpm)
                updateSequencerTiming(posInfo.bpm);
        }
    }

    int numSamples = buffer.getNumSamples();
    for (int sample = 0; sample < numSamples; ++sample)
    {
        stepSampleCounter++;
        if (stepSampleCounter >= samplesPerStep)
        {
            stepSampleCounter = 0;
            currentStep = (currentStep + 1) % HacktribeParams::NUM_STEPS;
            triggerStep(currentStep.load(), midiMessages);
        }
    }

    synthEngine.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

void Electribe2HacktribeAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void Electribe2HacktribeAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Electribe2HacktribeAudioProcessor();
}
