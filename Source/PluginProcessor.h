/*
  ==============================================================================
                        processor header
  ==============================================================================
*/

#pragma once
#define GAIN_ID "gain"
#define GAIN_NAME "Gain"
#include <JuceHeader.h>
#include "visualizer.h"

//==============================================================================
/**
*/
class VerbotestAudioProcessor  : public juce::AudioProcessor, public juce::Timer
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            

                            #endif

{
public:
    //==============================================================================
    VerbotestAudioProcessor();
    ~VerbotestAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;


   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    void process (juce::dsp::ProcessContextReplacing<float> context);
    void drawNextFrameOfSpectrum();
    static constexpr auto fftSize = 1 << 11;
    std::array<float, fftSize * 2> fftData;
    std::array<float, fftSize> fifo;
    void updateParams();
    void addToFifo(float sample);
    void frequalize();
    void timerCallback()override;
    bool nextFFTBlockReady = false;
    std::array<float,fftSize * 2> getFFTData();
    juce::AudioProcessorValueTreeState treeState;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::dsp::WindowingFunction<float> window;

    enum {
        scopeSize = 512
    };
    std::array<float,scopeSize> scopeData;


private:
    
    juce::dsp::Reverb verbo;
    juce::dsp::Reverb::Parameters paramets;
    juce::dsp::FFT freak;
    int fifoindex = 0;
    void reset() override;
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VerbotestAudioProcessor)
};
