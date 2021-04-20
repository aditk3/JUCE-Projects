//==============================================================================

#include "MainApplication.h"
#include "MainWindow.h"
#include "MainComponent.h"

MainWindow::MainWindow(String name)
: DocumentWindow(name, Colours::white, DocumentWindow::allButtons) {
    // Window title bar uses  the OS'es window style
    setUsingNativeTitleBar(true);
      
  // add our content component. owned means that when the window is
  // deleted it will automatically delete our content component
  auto maincomp = std::make_unique<MainComponent>();
  setContentOwned(maincomp.release(), false);
  
  // On laptops put the window in the middle of the screen with size
  // determined by our content component.
    #if JUCE_MAC || JUCE_WINDOWS || JUCE_LINUX
        setResizable(true, true);
        setResizeLimits(600, 400, 1200, 800);
        centreWithSize(getWidth(), getHeight());
    #else // JUCE_IOS || JUCE_ANDROID
         setFullScreen (true);
    #endif
  // make the window visible
  setVisible(true);
}

//==============================================================================
// DocumentWindow overrides

void MainWindow::closeButtonPressed() {
  // when the main window is closed signal the app to exit
  JUCEApplication::getInstance()->systemRequestedQuit();
}

