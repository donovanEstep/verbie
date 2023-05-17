/*
  ==============================================================================

    visualizer.h
    Created: 3 Apr 2023 6:30:56pm
    Author:  Donovan Estep

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
//#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class visualizer  : public juce::Component, public juce::Timer
{
public:
    visualizer();
    ~visualizer() override;


    void paint (juce::Graphics&) override;
    void timerCallback() override;
    void resized() override;
    void drawFrame (juce::Graphics& g);

    juce::Rectangle<float> rect1;
    //VerbotestAudioProcessor audioProcessor;
    
    

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (visualizer)
};
