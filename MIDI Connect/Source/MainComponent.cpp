//==============================================================================

#include "MainComponent.h"

#include "MainApplication.h"

MainContentComponent::MainContentComponent() : audioManager(MainApplication::getApp().getAudioDeviceManager()) {
    addAndMakeVisible(settingsButton);
    settingsButton.addListener(this);

    addAndMakeVisible(messageLogButton);
    messageLogButton.addListener(this);
    messageLogButton.setToggleState(true, dontSendNotification);
    messageLogButton.setClickingTogglesState(true);
    messageLogButton.setRadioGroupId(1);
    messageLogButton.setConnectedEdges(TextButton::ConnectedOnRight);

    addAndMakeVisible(pianoRollButton);
    pianoRollButton.addListener(this);
    messageLogButton.setToggleState(false, dontSendNotification);
    messageLogButton.setClickingTogglesState(true);
    messageLogButton.setRadioGroupId(1);
    pianoRollButton.setConnectedEdges(TextButton::ConnectedOnLeft);

    addAndMakeVisible(clearButton);
    clearButton.addListener(this);

    addAndMakeVisible(midiPianoRoll);
    addAndMakeVisible(midiMessageLog);

    keyboardState.addListener(this);
    midiKeyboard = std::make_unique<MidiKeyboardComponent>(keyboardState, MidiKeyboardComponent::horizontalKeyboard);
    midiKeyboard->setOctaveForMiddleC(4);
    addAndMakeVisible(*midiKeyboard);

    audioManager.addMidiInputCallback("", this);
    audioManager.addAudioCallback(&sfZeroPlayer);

    sfZeroAudioProcessor.reset(new sfzero::SFZeroAudioProcessor());
    sfZeroPlayer.setProcessor(sfZeroAudioProcessor.get());

    // Get and load soundFont:
    File soundFont = MainApplication::getApp().getRuntimeResourceDirectory().getChildFile("G800-A112-Piano1d-2-3f.sfz");
    loadSoundFont(soundFont);
    setVisible(true);
}

MainContentComponent::~MainContentComponent() {
    keyboardState.removeListener(this);
    audioManager.removeAudioCallback(&sfZeroPlayer);
    sfZeroPlayer.setProcessor(nullptr);
}

//==============================================================================
// Component overrides

void MainContentComponent::paint(Graphics& g) {
    g.fillAll(getLookAndFeel().findColour(DocumentWindow::backgroundColourId));

    g.setColour(Colours::red);

    for (int i = 0; i < getNumChildComponents(); i++) g.drawRect(getChildComponent(i)->getBounds(), 1);
}

void MainContentComponent::resized() {
    double compHeight{24.0};
    double padding{8.0};

    auto bounds = getBounds().reduced(8);

    auto topLine = bounds.removeFromTop(compHeight);
    auto copy = topLine;

    settingsButton.setBounds(topLine.removeFromLeft(120));
    clearButton.setBounds(topLine.removeFromRight(60));

    messageLogButton.setBounds(copy.getWidth() / 2 - 90, copy.getY(), 90, compHeight);
    pianoRollButton.setBounds(copy.getWidth() / 2, copy.getY(), 90, compHeight);

    bounds.removeFromTop(padding);
    copy = bounds.removeFromTop(64);
    midiKeyboard->setBounds(copy.getX(), copy.getY(), copy.getWidth(), copy.getHeight());

    bounds.removeFromTop(padding);
    midiMessageLog.setBounds(bounds);
    midiPianoRoll.setBounds(bounds);
}

//==============================================================================
// Listener overrides

void MainContentComponent::buttonClicked(Button* button) {
    if (button == &settingsButton) {
        openAudioSettings();
    }

    if (button == &messageLogButton) {
        midiMessageLog.clear();
        addChildComponent(&midiMessageLog);
        addAndMakeVisible(&midiMessageLog);
        removeChildComponent(&midiPianoRoll);
    }

    if (button == &pianoRollButton) {
        midiPianoRoll.clear();
        addChildComponent(&midiPianoRoll);
        addAndMakeVisible(&midiPianoRoll);
        removeChildComponent(&midiMessageLog);
    }

    if (button == &clearButton) {
        midiMessageLog.clear();
        midiPianoRoll.clear();
    }
}

void MainContentComponent::showMidiMessage(const MidiMessage& message) {
    if (messageLogButton.getToggleState())
        midiMessageLog.addMidiMessage(message);
    else
        midiPianoRoll.addMidiMessage(message);
}

void MainContentComponent::playMidiMessage(const MidiMessage& message) {
    sfZeroPlayer.getMidiMessageCollector().addMessageToQueue(message);
}

void MainContentComponent::handleNoteOn(MidiKeyboardState*, int chan, int note, float vel) {
    MidiMessage msg(MidiMessage::noteOn(chan, note, vel));
    msg.setTimeStamp(Time::getMillisecondCounterHiRes() * 0.001);
    showMidiMessage(msg);
    playMidiMessage(msg);
}

void MainContentComponent::handleNoteOff(MidiKeyboardState*, int chan, int note, float vel) {
    MidiMessage msg(MidiMessage::noteOff(chan, note, vel));
    msg.setTimeStamp(Time::getMillisecondCounterHiRes() * 0.001);
    showMidiMessage(msg);
    playMidiMessage(msg);
}

//==============================================================================
// MidiCallback overrides

// NOTE: this callback is not called by the main message thread so it should not
// modify any GUI components directly without locking the main message thread.
void MainContentComponent::handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message) {
    if (message.isActiveSense()) {
        return;
    } else if (quitting) {
        return;
    }

    if (message.isNoteOnOrOff()) {
        keyboardState.processNextMidiEvent(message);
    } else {
        playMidiMessage(message);
        showMidiMessage(message);
    }
}

//==============================================================================
// Internal Synth code
//

void MainContentComponent::loadSoundFont(juce::File& fontFile) {
    jassert(fontFile.existsAsFile());  // Check if the lib exists

    sfZeroAudioProcessor->setSfzFileThreaded(&fontFile);
    DBG("Sound font loaded!");
}

void MainContentComponent::openAudioSettings() {
    auto ptr = std::unique_ptr<AudioDeviceSelectorComponent>(
        new AudioDeviceSelectorComponent(audioManager, 0, 2, 0, 2, true, false, true, false));

    ptr.get()->setSize(500, 270);

    DialogWindow::LaunchOptions dw;
    dw.useNativeTitleBar = true;
    dw.resizable = false;
    dw.dialogTitle = "Audio Settings";
    dw.dialogBackgroundColour = getLookAndFeel().findColour(DocumentWindow::backgroundColourId);

    dw.content.setOwned(ptr.get());
    ptr.release();
    dw.launchAsync();
}
