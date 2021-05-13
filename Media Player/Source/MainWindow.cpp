//==============================================================================

#include "MainWindow.h"

#include "MainApplication.h"
#include "MainComponent.h"

MainWindow::MainWindow(String name) : DocumentWindow(name, Colours::white, DocumentWindow::allButtons) {
    setUsingNativeTitleBar(true);
    auto mainComp = std::make_unique<MainComponent>();
    setContentOwned(mainComp.release(), false);

#if JUCE_MAC || JUCE_WINDOWS || JUCE_LINUX
    setResizable(false, true);

    centreWithSize(392, 120);
#else  // JUCE_IOS || JUCE_ANDROID
    setFullScreen(true);
#endif

    setVisible(true);
}

//==============================================================================
// DocumentWindow overrides

void MainWindow::closeButtonPressed() { JUCEApplication::getInstance()->systemRequestedQuit(); }
