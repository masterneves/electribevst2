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

const juce::String Electribe2HacktribeAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Electribe2HacktribeAudioProcessor::acceptsMidi() const { return true; }
bool Electribe2HacktribeAudioProcessor::producesMidi() const { return false; }
bool Electribe2HacktribeAudioProcessor::isMidiEffect() const { return false; }
double Electribe2HacktribeAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int Electribe2HacktribeAudioProcessor::getNumPrograms() { return 1; }
int Electribe2HacktribeAudioProcessor::getCurrentProgram() { return 0; }
void Electribe2HacktribeAudioProcessor::setCurrentProgram (int index) {}
const juce::String Electribe2HacktribeAudioProcessor::getProgramName (int index) { return {}; }
void Electribe2HacktribeAudioProcessor::changeProgramName (int index, const juce::String& newName) {}

void Electribe2HacktribeAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = (juce::uint32) samplesPerBlock;
    spec.numChannels = (juce::uint32) getTotalNumOutputChannels();

    oscillator.prepare(spec);

    filter.reset();
    filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    filter.prepare(spec);
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

void Electribe2HacktribeAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Processar mensagens MIDI (Note On / Note Off)
    for (const auto metadata : midiMessages)
    {
        auto message = metadata.getMessage();
        if (message.isNoteOn())
        {
            float frequency = (float) juce::MidiMessage::getMidiNoteInHertz(message.getNoteNumber());
            oscillator.setFrequency(frequency);
            isPlaying = true;
        }
        else if (message.isNoteOff())
        {
            isPlaying = false;
        }
    }

    juce::AudioBlock<float> block(buffer);

    // Se estivermos a tocar uma nota, geramos o som do oscilador
    if (isPlaying)
    {
        juce::dsp::ProcessContextReplacing<float> context(block);
        oscillator.process(context);
    }
    else
    {
        buffer.clear();
    }

    // Ler e aplicar os parâmetros do filtro (Cutoff, Resonance, Mode)
    float cutoff = apvts.getRawParameterValue("TRK_1_CUTOFF")->load();
    float resonance = apvts.getRawParameterValue("TRK_1_RESONANCE")->load();
    int mode = (int) apvts.getRawParameterValue("TRK_1_MODE")->load();

    if (mode == 0)
        filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    else if (mode == 1)
        filter.setType(juce::dsp::StateVariableTPTFilterType::highpass);

    filter.setCutoffFrequency(cutoff);
    filter.setResonance(resonance);

    juce::dsp::ProcessContextReplacing<float> filterContext(block);
    filter.process(filterContext);
}

bool Electribe2HacktribeAudioProcessor::hasEditor() const { return true; }
juce::AudioProcessorEditor* Electribe2HacktribeAudioProcessor::createEditor()
{
    return new Electribe2HacktribeAudioProcessorEditor (*this);
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

juce::AudioProcessorValueTreeState::ParameterLayout Electribe2HacktribeAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        "TRK_1_CUTOFF", "Track 1 Cutoff",
        juce::NormalisableRange<float> (20.0f, 20000.0f, 1.0f, 0.3f), 20000.0f));

    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        "TRK_1_RESONANCE", "Track 1 Resonance",
        juce::NormalisableRange<float> (0.1f, 10.0f, 0.05f), 0.707f));

    params.push_back (std::make_unique<juce::AudioParameterChoice> (
        "TRK_1_MODE", "Track 1 Mode",
        juce::StringArray { "Low Pass", "High Pass" }, 0));

    return { params.begin(), params.end() };
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Electribe2HacktribeAudioProcessor();
}
