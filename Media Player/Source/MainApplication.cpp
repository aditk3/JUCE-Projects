//==============================================================================

#include "MainApplication.h"

#include "MainComponent.h"
#include "MainWindow.h"

//==============================================================================
// MainApplication members

MainApplication& MainApplication::getApp() {
    MainApplication* const app = dynamic_cast<MainApplication*>(JUCEApplication::getInstance());
    assert(app != nullptr);
    return *app;
}

void MainApplication::closeAllAlertAndDialogWindows() {}

//==============================================================================
// JUCEApplication overrides

MainApplication::MainApplication() {}

const String MainApplication::getApplicationName() { return ProjectInfo::projectName; }

const String MainApplication::getApplicationVersion() { return ProjectInfo::versionString; }

bool MainApplication::moreThanOneInstanceAllowed() { return false; }

void MainApplication::initialise(const String& commandLine) {
    mediaManager = std::make_unique<MediaManager>();
    mainWindow = std::make_unique<MainWindow>(getApplicationName());
}

void MainApplication::shutdown() {
    // Delete our main window
    mainWindow = nullptr;
}

void MainApplication::systemRequestedQuit() { quit(); }

void MainApplication::anotherInstanceStarted(const String& commandLine) {}

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION(MainApplication)
