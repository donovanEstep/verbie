/*
  ==============================================================================
                                Editor main
  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "visualizer.h"

//==============================================================================
VerbotestAudioProcessorEditor::VerbotestAudioProcessorEditor (VerbotestAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{

    sliderValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "width", slider1);
    
    sliderValue2 = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "wetlevel", slider2);
    sliderValue3 = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "roomsize", slider3);
    sliderValue4 = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "damping", slider4);
    buttonValue = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "freezemode", button1);
    button2Value = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "distortion", button2);
    
    
    setSize (647,400);

    slider1.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    slider1.setRange(0.0f, 1.0f);
    slider1.setValue(*audioProcessor.treeState.getRawParameterValue("width"));
    slider1.setAlpha(0.0);
    addAndMakeVisible(slider1);
    
    slider2.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    slider2.setRange(0.0f, 1.0f);
    slider2.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
    slider2.setValue(*audioProcessor.treeState.getRawParameterValue("wetlevel"));
    slider2.setAlpha(0.0);
    addAndMakeVisible(slider2);
    
    slider3.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    slider3.setRange(0.0f, 1.0f);
    slider3.setValue(*audioProcessor.treeState.getRawParameterValue("roomsize"));
    slider3.setAlpha(0.0);
    addAndMakeVisible(slider3);
    
    slider4.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    slider4.setRange(0.0f,1.0f);
    slider4.setValue(*audioProcessor.treeState.getRawParameterValue("damping"));
    slider4.setAlpha(0.0);
    addAndMakeVisible(slider4);
    graph.setBounds(100,100,100,100);

    button1.setAlpha(0.0);
    addAndMakeVisible(button1);
    button2.setAlpha(0.0);
    addAndMakeVisible(button2);
    Timer::startTimerHz(60);
    
}

VerbotestAudioProcessorEditor::~VerbotestAudioProcessorEditor()
{
    Timer::stopTimer();
}
void VerbotestAudioProcessorEditor::drawFrame (juce::Graphics& g)
{
    for (int i = 1; i < VerbotestAudioProcessor::scopeSize; ++i)
    {

        auto width = 235;
        auto height = 90;
        int ogx = juce::jmap (i-1, 0, VerbotestAudioProcessor::scopeSize - 1, 0, width);
        int ogy = juce::jmap (audioProcessor.scopeData[i - 1], 0.0f, 1.0f, (float) height, 0.0f);
        int newx =juce::jmap (i,     0, VerbotestAudioProcessor::scopeSize - 1, 0, width);
        int newy =juce::jmap (audioProcessor.scopeData[i],     0.0f, 1.0f, (float) height, 0.0f);
        
        g.drawLine (ogx+370,ogy+195,newx+370,newy+195);
    }
}

//==============================================================================

void VerbotestAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.fillAll(juce::Colour(209,199,174));
    juce::Image background = juce::ImageCache::getFromMemory (BinaryData::BACKGROUND_IMAGE_1_png, BinaryData::BACKGROUND_IMAGE_1_pngSize);
    g.drawImageAt (background, 0, 0);
    juce::Image damping = juce::ImageCache::getFromMemory (BinaryData::DAMPING_png,BinaryData::DAMPING_pngSize);
    g.drawImageAt(damping,37,347-(slider4.getValue()*199));
    
    
    juce::Image room_size = juce::ImageCache::getFromMemory (BinaryData::DAMPING_png,BinaryData::DAMPING_pngSize);
    g.drawImageAt(room_size,107,347-(slider3.getValue()*199));
    
    juce::Image width = juce::ImageCache::getFromMemory (BinaryData::DAMPING_png,BinaryData::DAMPING_pngSize);
    g.drawImageAt(width,177,347-(slider1.getValue()*199));
    
    int mixX = 0;
    int mixY = 0+(48*(round(30*(1.0f-slider2.getValue()))));
    juce::Image mix = juce::ImageCache::getFromMemory (BinaryData::knobreal_png,BinaryData::knobreal_pngSize);
    g.drawImage(mix,264,133,48,48,mixX,mixY,48,48,false);
    
    juce::Image distortion = juce::ImageCache::getFromMemory (BinaryData::DISTORTION_png,BinaryData::DISTORTION_pngSize);
    g.drawImageAt(distortion,267,222);
    
    juce::Image freeze = juce::ImageCache::getFromMemory (BinaryData::FREEZE_BUTTON_png,BinaryData::FREEZE_BUTTON_pngSize);
    g.drawImageAt(freeze,267,303);
    
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.setColour(juce::Colours::black);
    g.setColour(juce::Colour(155,169,214));
    VerbotestAudioProcessorEditor::drawFrame(g);

}
int inc = 0;
void VerbotestAudioProcessorEditor::timerCallback()
{
    scopey = audioProcessor.scopeData;

    repaint();
}

void VerbotestAudioProcessorEditor::resized()
{
    slider1.setBounds(172,142,39,225);
    slider2.setBounds(260,129,60,60);
    slider3.setBounds(102,142,39,225);
    slider4.setBounds(32,142,39,225);
    button1.setBounds(267,303,40,40);
    button2.setBounds(267, 222, 42, 42);
    rect1.setBounds(0, 0, 50, 200);
    rect2.setBounds(55, 0, 50, 200);
    rect3.setBounds(110, 0, 50, 200);
    rect4.setBounds(165, 0, 50, 200);
}
