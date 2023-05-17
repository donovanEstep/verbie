/*
  ==============================================================================

    visualizer.cpp
    Created: 3 Apr 2023 6:30:56pm
    Author:  Donovan Estep
    rgb for visualizer: R:155 G:169 B:214

  ==============================================================================
*/

#include <JuceHeader.h>
#include "visualizer.h"

//==============================================================================
visualizer::visualizer()
{
    setSize(200,75);
    Timer::startTimerHz(60);

}

visualizer::~visualizer()
{
}
void visualizer::drawFrame (juce::Graphics& g)
{
    for (int i = 1; i < VerbotestAudioProcessor::scopeSize; ++i)
    {
        //auto width  = getLocalBounds().getWidth();
        //auto height = getLocalBounds().getHeight();
        //auto width = 100;
        //auto height = 300;
        //auto topleft = 0;
        //auto topright = 200;
        //auto bottomleft = 100;
        //auto bottomright = 300;
        //auto beebo = VerbotestAudioProcessor::scopeData.data();
        

        //g.drawLine ({ (float) juce::jmap (i - 1, 0, VerbotestAudioProcessor::scopeSize - 1, 0, width),
                              //juce::jmap (VerbotestAudioProcessor::scopeData[i - 1], 0.0f, 1.0f, (float) height, 0.0f),
                      //(float) juce::jmap (i,     0, VerbotestAudioProcessor::scopeSize - 1, 0, width),
                              //juce::jmap (VerbotestAudioProcessor::scopeData[i],     0.0f, 1.0f, (float) height, 0.0f) });
    }
}

void visualizer::paint (juce::Graphics& g)
{
    
    g.fillAll(juce::Colours::white);
    g.setColour(juce::Colours::black);
    visualizer::drawFrame(g);

    

   

}
void visualizer::timerCallback(){
    repaint();
}
void visualizer::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
