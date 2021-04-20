//==============================================================================
/// @file MainComponent.h
/// Defines the class representing our main window content component. See the
/// cpp file comments for specific implementation details.

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiMessageLog.h"
#include "MidiPianoRoll.h"
#include "SFZeroAudioProcessor.h"

/// Inherits from several Listener classes so it can respond
/// to users manipulating subcomponent buttons, menus etc. It also inherits from
/// MidiInputCallback so that it receives MidiMessages whenever they arrive.
struct MainContentComponent : public Component,
                              public MidiInputCallback,
                              public Button::Listener,
                              public MidiKeyboardStateListener {
    /// Constructor.  Your method should take the following actions:
    /// * Add the messageLogButton and pianoRollButton to our component, then
    /// configure them to be toggle and radio buttons and share connected
    /// edges. Set the initial toggle state of messageLogButton to be "on" without
    /// triggering the button's callback.
    /// * Add and configure the clearButton and settingsButton.
    /// * Assign the midiKeyboard member a new MidiKeyboardComponent, then add and
    /// make it visible. Set its Middle C octave to be the 4th octave.
    /// * Add our component as the keyboardState's listener.
    /// * Add and make visible the midiMessageLog.
    /// * Set the midiPianoRoll to be visible but do NOT add it.
    /// * Call the audio manager's addMidiInputCallback() method
    /// and add out component as its callback.
    /// * Add the sfZeroPlayer as the callback for our audio device manager.
    /// * Create a new sfzero::SFZeroAudioProcessor and assign it to
    /// the sfZeroAudioProcessor unique_ptr.
    /// * Add the sfZeroAudioProcessor as the processor for the sfZeroPlayer.
    /// * Call loadSoundFont() to load the our sound font file
    /// "G800-A112-Piano1d-2-3f.sfz". This file is stored in the the app's
    /// resource directory. See: MainApplication::getRuntimeResourceDirectory().
    /// * Call the loadSoundFont() function to load the sound font resource.
    MainContentComponent();

    /// Destructor. Your method should take the following actions:
    /// * Remove this component as keyboard states listener.
    /// * Remove sfzeroplayer as the audio device managers's callback.
    /// audioManager.removeMidiInputCallback("", this);
    /// * Set sfZeroPlayer's processor to be nullptr;.
    ~MainContentComponent();

    //==============================================================================
    // Component overrides

    /// Called by the graphics system to paint the component. Your method should
    /// completely fill the background of this component with the colour for
    /// ResizableWindow::backgroundColourId.
    void paint(Graphics&) override;

    /// Called when the MainContentComponent is resized. Your method should take
    /// the following actions:
    /// * All child components are indented 8 pixels from the edge of the component.
    /// * The height of the top line is 24.
    /// * The settings button is 120 pixels wide.
    /// * The clear button should be right justified in the top line with a
    /// width of 60.
    /// * The messageLogButton and pianoRollbutton should be centered as a unit,
    /// (abutted together) each with a height of 24 and a width of 90.
    /// * There should be an 8 pixel space between the top line and the
    /// midiKeyboardComponent.
    /// * The midiKeyboard is 64 pixels high and occupies the width of the
    /// MainComponent.
    /// * The midiMessageLog and midiPianoRoll viewers take the remainder of
    /// the component's area beginning 8 pixels below the keyboard.
    void resized() override;

    //==============================================================================
    // MidiInputCallback overrides

    /// The callback that responds whenever a MIDI message arrives at the
    /// midiInputDevice. Note: this function does not run in the main message thread
    /// and must never attempt to update GUI components with taking steps to insure
    /// that both threads do not access components at the same time.  Your method
    /// should take the following action:
    /// * If the quitting member is true or if the incoming midi message
    /// is an Active Sensing message this method should immediately return.
    /// * Otherwise the main message thread should first be blocked using a
    /// MessageThreadLock and then the incoming midi message routed to components
    /// according to its message type:
    ///   - If the incoming message is a noteOn or noteOff then add it to the
    ///     midiKeyboardState so the keyboardComponent will display its arrival.
    ///   - Otherwise the message should be passed to showMidiMessage()
    ///     and playMidiMessage().
    void handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message) override;

    //==============================================================================
    // Listener overrides

    /// The callback that responds to Button clicks. Your method should perform
    /// the following actions:
    /// * If the messageLogButton was clicked (and it was not already toggled on) then
    /// perform the following steps:
    ///   - Call removeChildComponent on the midiPianoRoll to remove it from this component.
    ///   - Call midiMessageLog's clear method to remove any existing display.
    ///   - Call the keyboardState's reset() method to zero out any existing data.
    ///   - Call addChildComponent on the midiMessageLog.
    /// * If the pianoRollButton was clicked (and it was not already toggled on) then
    /// take analogous steps as outlined for the messageLogButton.
    /// * If the clearButton was clicked then clear the message log or
    /// the piano roll, whichever is currently active. See: getToggleState().
    /// * If the settingsButton was clicked then open the audio settings.
    void buttonClicked(Button* button) override;

    /// The MidiKeyboardComponent callback that responds to NoteOns. When
    /// triggered this method will create a NoteOn and route it to the
    /// sfZeroAudioProcessor for audio playback and to either the midiMessageLog
    /// or the midiPianoRoll, whichever is currently active.
    void handleNoteOn(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;

    /// The MidiKeyboardComponent callback that responds to NoteOffs. When
    /// triggered this method will create a NoteOff and route it to
    /// showMidiMessage() and playMidiMessage().
    void handleNoteOff(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;

    //==============================================================================
    // MainContentComponent members

    /// Loads the sfzero synth with the specified sound font file.
    void loadSoundFont(juce::File& fontFile);

    /// Adds the message to either the MidiMessageLog or the MidiPianoRoll,
    /// whichever is currently visible.
    void showMidiMessage(const MidiMessage& message);

    /// Adds the message to the internal syth for playback.
    void playMidiMessage(const MidiMessage& message);

    /// If true the midi input callback will return without adding input. This
    /// member should be set to true by MainApplication::systemRequestedQuit()
    /// when the user quits the app.
    bool quitting{false};

   private:
    /// Open the audio settings dialog. The dialog should not
    /// show audio inputs and outputs because our device manager
    /// does not enable them. See the WaveLab app for more information.
    void openAudioSettings();

    /// A button that displays "Audio Settings...".
    TextButton settingsButton{"Audio Settings..."};

    /// A toggle button that displays "Message Log".
    TextButton messageLogButton{"Message Log"};

    /// A toggle button that displays "Piano Roll".
    TextButton pianoRollButton{"Piano Roll"};

    /// A button that displays "Clear".
    TextButton clearButton{"Clear"};

    /// An object that manages a MidiKeyboardComponent
    MidiKeyboardState keyboardState;

    /// A juce component that displays an interactive midi keyboard.
    std::unique_ptr<MidiKeyboardComponent> midiKeyboard;

    /// A graphical component that displays incoming MIDI messages.
    MidiMessageLog midiMessageLog;

    /// A component that displays an animation of incoming MIDI messages.
    MidiPianoRoll midiPianoRoll;

    /// Reference to the application's device manager.
    AudioDeviceManager& audioManager;

    /// The SFZero soundfont synth.
    std::unique_ptr<sfzero::SFZeroAudioProcessor> sfZeroAudioProcessor;

    /// An audio processor player to play the SFZeroAudioProcessor
    AudioProcessorPlayer sfZeroPlayer;

    /// The currently open MIDI output device. Currently null;
    std::unique_ptr<MidiOutput> midiOutputDevice;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};
