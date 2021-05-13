//==============================================================================

#include "MainApplication.h"

#include "MainComponent.h"
#include "MainWindow.h"

//==============================================================================
// MainApplication members

MainApplication& MainApplication::getApp() {
    MainApplication* const app = dynamic_cast<MainApplication*>(JUCEApplication::getInstance());
    jassert(app != nullptr);
    return *app;
}

const File MainApplication::getRuntimeResourceDirectory() {
#if JUCE_MAC
    return File::getSpecialLocation(File::currentApplicationFile).getChildFile("Contents/Resources");
#endif
#if JUCE_WINDOWS
    return File::getSpecialLocation(File::currentApplicationFile).getParentDirectory().getChildFile("Resources");
#endif
#if JUCE_IOS
    return File::getSpecialLocation(File::currentApplicationFile);
#endif
#if JUCE_LINUX
    return File::getSpecialLocation(File::currentApplicationFile)
        .getParentDirectory()
        .getParentDirectory()
        .getParentDirectory()
        .getParentDirectory()
        .getChildFile("Resources");
#endif
    // else
    jassert(false);
}

AudioDeviceManager& MainApplication::getAudioDeviceManager() { return audioDeviceManager; }

void MainApplication::closeAllAlertAndDialogWindows() {}

//==============================================================================
// JUCEApplication overrides

MainApplication::MainApplication() {}

const String MainApplication::getApplicationName() { return ProjectInfo::projectName; }

const String MainApplication::getApplicationVersion() { return ProjectInfo::versionString; }

bool MainApplication::moreThanOneInstanceAllowed() { return false; }

void MainApplication::initialise(const String& commandLine) {
    auto errorMessage = audioDeviceManager.initialise(0, 2, nullptr, true);
    jassert(errorMessage.isEmpty());

    mainWindow = std::make_unique<MainWindow>(getApplicationName());
}

void MainApplication::shutdown() {
    // Delete our main window
    mainWindow = nullptr;
}

void MainApplication::systemRequestedQuit() {
    closeAllAlertAndDialogWindows();
    quit();
}

void MainApplication::anotherInstanceStarted(const String& commandLine) {
    // When another instance of the app is launched while this one is running,
    // this method is invoked, and the commandLine parameter tells you what
    // the other instance's command-line arguments were.
}

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION(MainApplication)
