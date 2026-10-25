#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Electribevst2AudioProcessor::Electribevst2AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
       apvts (*this, nullptr, "Parameters", createParameterLayout())
#endif
{
    // Configuração da onda do oscilador (Onda dente de serra por defeito)
    osc.initialise ([] (float x) { return std::sin (x); });
}

Electribevst2AudioProcessor::~Electribevst2AudioProcessor()
{
}

//==============================================================================
const juce::String Electribevst2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Electribevst2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Electribevst2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Electribevst2AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Electribevst2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Electribevst2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts choose to think 0 means 1
}

int Electribevst2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Electribevst2AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Electribevst2AudioProcessor::getProgramName (int index)
{
    return {};
}

void Electribevst2AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Electribevst2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32> (samplesPerBlock);
    spec.numChannels = static_cast<juce::uint32> (getTotalNumOutputChannels());

    osc.prepare (spec);
    filter.prepare (spec);

    // Frequência de corte inicial do filtro TPT
    filter.setType (juce::dsp::StateVariableTPTFilterType::lowpass);
}

void Electribevst2AudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Electribevst2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Electribevst2AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Atualização dos parâmetros do APVTS
    auto cutoff = apvts.getRawParameterValue ("CUTOFF")->load();
    auto resonance = apvts.getRawParameterValue ("RESONANCE")->load();

    filter.setCutoffFrequency (cutoff);
    filter.setResonance (resonance);

    // Processamento DSP correto
    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> context (block);

    osc.process (context);
    filter.process (context);
}

//==============================================================================
bool Electribevst2AudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* Electribevst2AudioProcessor::createEditor()
{
    return new Electribevst2AudioProcessorEditor (*this);
}

//==============================================================================
void Electribevst2AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
}

void Electribevst2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

juce::AudioProcessorValueTreeState::ParameterLayout Electribevst2AudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { "CUTOFF", 1 },
        "Cutoff",
        juce::NormalisableRange<float> (20.0f, 20000.0f, 1.0f, 0.3f),
        1000.0f));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { "RESONANCE", 1 },
        "Resonance",
        juce::NormalisableRange<float> (0.1f, 10.0f, 0.05f),
        1.0f));

    layout.add (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID { "MODE", 1 },
        "Mode",
        juce::StringArray { "Lowpass", "Highpass", "Bandpass" },
        0));

    return layout;
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Electribevst2AudioProcessor();
}
