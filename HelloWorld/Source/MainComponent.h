#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void mouseDown (const MouseEvent& event) override;

private:
    //==============================================================================
    // Your private member variables go here...
    juce::String greetings[6] {"Hello, world!", "Hallo, wereld!", "Bonjour, monde!", "Hallo, Welt!", "Ciao, mondo!", "Hola, mundo!"};
    
    juce::String curGreeting = greetings[0];
    
    // First 3 are dark, last are light
    juce::Colour colours[6] {getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId), juce::Colours::darkblue, juce::Colours::black, juce::Colours::white, juce::Colours::antiquewhite, juce::Colours::lightpink};
    
    juce::Colour curColour = colours[0];
    
    juce::Colour fontColour = juce::Colours::white;
    
    juce::Point<int> mousePos = juce::Point<int>(600 / 2, 400 / 2);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
