#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() {
    setSize(600, 400);
    using namespace juce;

    auto f = File();
}

MainComponent::~MainComponent() {}

//==============================================================================
void MainComponent::paint(juce::Graphics& g) {
    //     (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(curColour);

    auto font = juce::Font(16.0f);

    float x = mousePos.getX();
    float y = mousePos.getY();
    float height = font.getHeight();
    float width = font.getStringWidth(curGreeting);

    float topLeftX = juce::jlimit(0.0f, 600.f - width, x - width / 2);
    float topLeftY = juce::jlimit(0.0f, 400.f - height, y - height / 2);

    auto textbox = juce::Rectangle<float>(topLeftX, topLeftY, width, height);

    g.setFont(font);
    g.setColour(fontColour);
    g.drawText(curGreeting, textbox, true);
}

void MainComponent::resized() {
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

void MainComponent::mouseDown(const MouseEvent& event) {
    auto randNum = juce::Random::getSystemRandom().nextInt(6);

    mousePos = event.getPosition();

    curColour = colours[randNum];
    curGreeting = greetings[randNum];

    if (randNum < 3)
        fontColour = juce::Colours::white;
    else
        fontColour = juce::Colours::black;

    repaint();
}
