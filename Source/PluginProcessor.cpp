/*
  ==============================================================================
                            Processor Main
  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VerbotestAudioProcessor::VerbotestAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), treeState(*this, nullptr,"PARAMETER", createParameterLayout()), window (fftSize, juce::dsp::WindowingFunction<float>::flatTop),
freak (11)
{
}
#endif


VerbotestAudioProcessor::~VerbotestAudioProcessor()
{
    Timer::stopTimer();
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout VerbotestAudioProcessor::createParameterLayout(){
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    auto gainParam = std::make_unique<juce::AudioParameterFloat>("width","widths", 0.0f, 1.0f, 0.5f);
    params.push_back(std::move(gainParam));
    
    auto wetParam = std::make_unique<juce::AudioParameterFloat>("wetlevel","wet",0.0f,1.0f,0.5f);
    params.push_back(std::move(wetParam));
    
    auto sizeParam = std::make_unique<juce::AudioParameterFloat>("roomsize","size",0.0f,1.0f,0.5f);
    params.push_back(std::move(sizeParam));
    
    auto dampParam = std::make_unique<juce::AudioParameterFloat>("damping","damp",0.0f,1.0f,0.5f);
    params.push_back(std::move(dampParam));
    
    auto freezeParam = std::make_unique<juce::AudioParameterInt>("freezemode","freeze",0.0f,1.0f,0.0f);
    params.push_back(std::move(freezeParam));
    
    auto distParam = std::make_unique<juce::AudioParameterInt>("distortion","distort",0.0f,1.0f,0.0f);
    params.push_back(std::move(distParam));
    //auto widthParam = std::make_unique<juce::AudioParameterInt>("width","widths",0.0f,1.0f,0.0f);
    //params.push_back(std::move(freezeParam));
    
    return {params.begin(), params.end()};
}

const juce::String VerbotestAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool VerbotestAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool VerbotestAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool VerbotestAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double VerbotestAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int VerbotestAudioProcessor::getNumPrograms()
{
    return 1;
}

int VerbotestAudioProcessor::getCurrentProgram()
{
    return 0;
}

void VerbotestAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String VerbotestAudioProcessor::getProgramName (int index)
{
    return {};
}

void VerbotestAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}


//==============================================================================



void VerbotestAudioProcessor::frequalize(){
    
        window.multiplyWithWindowingTable (fftData.data(), fftSize);

        freak.performFrequencyOnlyForwardTransform (fftData.data());

        auto mindB = -100.0f;
        auto maxdB =    0.0f;

        for (int i = 0; i < scopeSize; ++i)                         // [3]
        {
            auto skewedProportionX = 1.0f - std::exp (std::log (1.0f - (float) i / (float) scopeSize) * 0.2f);
            auto fftDataIndex = juce::jlimit (0, fftSize / 2, (int) (skewedProportionX * (float) fftSize * 0.5f));
            auto level = juce::jmap (juce::jlimit (mindB, maxdB, juce::Decibels::gainToDecibels (fftData[fftDataIndex])
                                                               - juce::Decibels::gainToDecibels ((float) fftSize)),
                                     mindB, maxdB, 0.0f, 1.0f);

            scopeData[i] = level;
        }
    
}

std::array<float,VerbotestAudioProcessor::fftSize * 2> VerbotestAudioProcessor::getFFTData(){
    return fftData;
}

void VerbotestAudioProcessor::addToFifo(float sample){
    
    if (fifoindex == fftSize)
    {
        if (!nextFFTBlockReady)
        {
            juce::zeromem(fftData.data(), sizeof(fftData));
            std::memcpy (fftData.data(), fifo.data(), sizeof (fifo));
            nextFFTBlockReady = true;
        
        }

        fifoindex = 0;
    }

    fifo[fifoindex++] = sample;
}
    
void VerbotestAudioProcessor::timerCallback(){
    if (nextFFTBlockReady){
        frequalize();
        nextFFTBlockReady = false;
    }
    
}

void VerbotestAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    float dry = 1.0f - *treeState.getRawParameterValue("wetlevel");
    float wet = *treeState.getRawParameterValue("wetlevel");
    float size = *treeState.getRawParameterValue("roomsize");
    float damping = *treeState.getRawParameterValue("damping");
    float freeze = *treeState.getRawParameterValue("freezemode");
    float width = *treeState.getRawParameterValue("width");
    paramets.dryLevel = dry;
    paramets.wetLevel = wet;
    paramets.roomSize = size;
    paramets.damping = damping;
    paramets.freezeMode = freeze;
    paramets.width = width;


    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumInputChannels();


    verbo.prepare(spec);
    verbo.reset();
    verbo.setParameters(paramets);
    Timer::startTimerHz(60);
}


void VerbotestAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool VerbotestAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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


void VerbotestAudioProcessor::updateParams()
{
    float dry = 1.0f - *treeState.getRawParameterValue("wetlevel");
    float wet = *treeState.getRawParameterValue("wetlevel");
    float size = *treeState.getRawParameterValue("roomsize");
    float damping = *treeState.getRawParameterValue("damping");
    float freeze = *treeState.getRawParameterValue("freezemode");
    float width = *treeState.getRawParameterValue("width");
    paramets.dryLevel = dry;
    paramets.wetLevel = wet;
    paramets.roomSize = size;
    paramets.damping = damping;
    paramets.freezeMode = freeze;
    paramets.width = width;
    verbo.setParameters(paramets);
}

void VerbotestAudioProcessor::process(juce::dsp::ProcessContextReplacing<float> context)
{
    verbo.process(context);
}




void VerbotestAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    updateParams();
    juce::dsp::AudioBlock <float> block (buffer);
    process(juce::dsp::ProcessContextReplacing<float> (block));
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample){
            
            if (*treeState.getRawParameterValue("distortion") > 0){
                channelData[sample] = ((tanh(5*buffer.getSample(channel,sample)))*0.5)*0.5;
            }
            else{
                channelData[sample] = buffer.getSample(channel,sample)* 0.5;
            }
                VerbotestAudioProcessor::addToFifo(channelData[sample]);
        }
    }
}


//==============================================================================
bool VerbotestAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* VerbotestAudioProcessor::createEditor()
{
    return new VerbotestAudioProcessorEditor (*this);
}

//==============================================================================
void VerbotestAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = treeState.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    VerbotestAudioProcessor::copyXmlToBinary (*xml, destData);
    
}

void VerbotestAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (treeState.state.getType()))
            treeState.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VerbotestAudioProcessor();
}
void VerbotestAudioProcessor::reset()
{
    verbo.reset();
}
