/*
  ==============================================================================
                            Editor Header
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class VerbotestAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Timer

{
public:
    VerbotestAudioProcessorEditor (VerbotestAudioProcessor&);
    ~VerbotestAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback()override;
    int posx = 100;
    int posy = 100;
    void setvals(VerbotestAudioProcessor&);
    void drawFrame (juce::Graphics& g);



private:

    juce::Slider slider1;
    juce::Slider slider2;
    juce::Slider slider3;
    juce::Slider slider4;
    juce::ToggleButton button1;
    juce::ToggleButton button2;
    juce::Rectangle<float> rect1;
    juce::Rectangle<float> rect2;
    juce::Rectangle<float> rect3;
    juce::Rectangle<float> rect4;
    enum {
        scopeSize = 512
    };
    std::array<float,scopeSize> scopey;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    VerbotestAudioProcessor& audioProcessor;
    visualizer graph;
public:
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> sliderValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> sliderValue2;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> sliderValue3;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> sliderValue4;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> buttonValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> button2Value;
    

    
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VerbotestAudioProcessorEditor)
};
