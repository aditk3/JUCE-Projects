//==============================================================================
// MainWindow.h
// This file defines the class representing our app's main window instance.
//==============================================================================

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

/// MainWindow implements the application's window containing an instance of
/// our MainContentComponent class.

class MainWindow : public DocumentWindow {
   public:
    /// Constructor for the MainWindow.
    /// @param name The string name of the window.
    ///
    /// In the constructor's initialization list your method should:
    /// * Call the superclass DocumentWindow constructor and pass it three
    /// values (1) the name of the window, (2) Colours::white and
    /// (3) DocumentWindow::allButtons.
    ///
    /// The main body of the constructor should:
    /// * Call setUsingNativeTitleBar() and pass it true.
    /// * On Windows or Linux call setMenuBar() and pass it the app's menubar model.
    /// * Use a unique pointer to allocate an instance of our MainComponent and
    /// then call addContentOwned() passing it two values: the MainComponent instance
    /// (use unique_ptr::release()) and true. See: juce::DocumentWindow::addContentOwned()
    /// for more information.
    /// * Call setResizable() and pass it two values: true and true.
    /// * Call setResizeLimits() and pass it the limits 500, 250, 1000, and 500.
    /// * Call centreWithSize() and pass it the window's width and height.
    /// * Call setVisible(true) to make the window visible.
    MainWindow(String name);

    /// Destructor for the MainWindow. This method should be empty.
    ~MainWindow();

    //==============================================================================
    // DocumentWindow overrides

    /// This is called when the user tries to close this window.
    /// Your method should perform the following steps:
    /// * Access the running application instance and call its systemRequestedQuit()
    /// method.
    /// See: MainApplication::getApp(), JUCEApplication::systemRequestedQuit().
    void closeButtonPressed() override;

   private:
    // A handy juce macro that detects memory leaks when running in DEBUG mode.
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};
