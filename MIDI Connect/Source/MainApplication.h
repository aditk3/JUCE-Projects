//==============================================================================
// @file MainApplication.h
// Defines the class representing our application instance.

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

// Forward declarations (see below)
class MainWindow;
class MainMenu;

/// Our Application Instance
class MainApplication : public JUCEApplication {
   public:
    MainApplication();

    //==============================================================================
    // JUCEApplication overrides

    const String getApplicationName() override;

    const String getApplicationVersion() override;

    bool moreThanOneInstanceAllowed() override;

    /// The application's initialization code. Your method should take
    /// the following actions:
    /// * Initialize the audio device manager with no audio inputs and
    ///   two audio outputs.
    /// * Create the application window.
    void initialise(const String& commandLine) override;

    /// The application's shutdown code. Your method can set the mainWidow
    /// member to nullptr (this is not technically necessary but it is
    /// a good idea to make your app's cleanup steps explicit.)
    void shutdown() override;

    /// Called when the app is being asked to quit. Your method should take
    /// the following actions:
    /// * Set MainComponent's 'quitting' member to true to prevent any
    /// additional midi input from being processed.
    /// * Close any open alert or dialog windows. See: closeAllAlertAndDialogWindows().
    /// * Quit the app.
    void systemRequestedQuit() override;

    void anotherInstanceStarted(const String& commandLine) override;

    //==============================================================================
    // MainApplication members

    /// Returns our application instance. See: Wave Lab app.
    static MainApplication& getApp();

    /// Returns the app's runtime resource directory using an OS-specific relative
    /// path from the location of the application. The application's location (pathname)
    /// can be found by calling juce::File::getSpecialLocation() and passing it the
    /// identifier juce::File::currentApplicationFile. Given the returned pathname
    /// {APP}, the app's resource directory can be found by appending relative paths:
    /// * JUCE_MAC: {APP}/Contents/Resources
    /// * JUCE_WINDOWS: {APP}/../Resources
    /// * JUCE_IOS: {APP}
    /// * JUCE_LINUX: ???
    const File getRuntimeResourceDirectory();

    /// Returns our AudioDeviceManager
    AudioDeviceManager& getAudioDeviceManager();

   private:
    /// Manages Audio/MIDI system settings.
    AudioDeviceManager audioDeviceManager;

    /// Close any open alert or dialog windows. See: Wave Lab.app
    void closeAllAlertAndDialogWindows();

    /// Pointer to the main window of the app.
    std::unique_ptr<MainWindow> mainWindow;
};
