//==============================================================================
// @file MainWindow.h
// Defines the app's main window definition.

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

/// The main application window.
struct MainWindow : public DocumentWindow {
    /// MainWindow constructor. In general your method should take the same
    /// actions as in the Wave Lab app but with the following changes:
    /// * Minimum window width is 608, maximum width is twice that.
    /// * Minimim window height is 412, maximum height is twice that.
    MainWindow(String name);

    //==============================================================================
    // DocumentWindow overrides

    /// Called when the user tries to close this window. See Wave Lab app.
    void closeButtonPressed() override;

   private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};
