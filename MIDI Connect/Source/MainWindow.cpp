//==============================================================================

#include "MainWindow.h"

#include "MainApplication.h"
#include "MainComponent.h"

MainWindow::MainWindow(String name) : DocumentWindow(name, Colours::white, DocumentWindow::allButtons) {
    setUsingNativeTitleBar(true);
    auto mainComp = std::make_unique<MainContentComponent>();
    setContentOwned(mainComp.release(), false);

#if JUCE_MAC || JUCE_WINDOWS || JUCE_LINUX
    setResizable(true, true);
    setResizeLimits(608, 412, 608 * 2, 412 * 2);
    centreWithSize(getWidth(), getHeight());
#else  // JUCE_IOS || JUCE_ANDROID
    setFullScreen(true);
#endif

    setVisible(true);
}

//==============================================================================
// DocumentWindow overrides

void MainWindow::closeButtonPressed() { JUCEApplication::getInstance()->systemRequestedQuit(); }
