//==============================================================================
// MainApplication.h
// This file defines the class representing our application instance.
//==============================================================================

// '#pragma once' stops this header file from being read more than one
// time by the compiler. Your .h files should normally start with this
// declaration.

#pragma once

// You must ensure that the JuceHeader.h file generated by the
// ProJucer is read before your code references any JUCE classes.

#include "../JuceLibraryCode/JuceHeader.h"

// Forward declarations (see below)
class MainWindow;
class MainMenu;

/// Our app's JUCEApplication instance that represents the running application.
/// The instance contains the basic components of the app: a menu bar and a main
/// window and a main component.
class MainApplication : public JUCEApplication {
public:
  
  /// The app's constructor. Don't add any code here, use the initialise()
  /// function to perform your app's startup actions.
  MainApplication();
  
  //==============================================================================
  // JUCEApplication overrides
  
  /// Returns the application's name.
  /// Your method should return the string ProjectInfo::projectName.
  const String getApplicationName() override;
  
  /// Returns the application's Version string.
  /// Your method should return the string ProjectInfo::versionString.
  const String getApplicationVersion() override;
  
  /// Returns true if multiple instances of the app should be able to
  /// run at the same time.
  /// Your method should return false.
  bool moreThanOneInstanceAllowed() override;
  
  /// This method is where you should add the app's startup code.
  /// Your method should perform the following steps:
  /// * Set the mainMenuBarModel member to a new instance of MainMenuBarModel.
  /// * Call registerAllCommandsForTarget to add all the application
  /// commands. See: juce::getAllCommands() and juce::getCommandInfo().
  /// * On MacOS add the app's menubar to the top of the screen. (On windows
  /// and linux the menubar is added by the main window.) See: juce::JUCE_MAC,
  /// juce::MenuBarModel::setMacMainMenu.
  /// * Set the mainWindow member to a new MainWindow instance.
  void initialise (const String& commandLine) override;
  
  /// This method is where you add the app's cleanup code.
  /// Your method should perform the following steps:
  /// * On MacOS remove the app's menubar from the top of the screen.
  /// See: juce::MenuBarModel::setMacMainMenu.
  /// * On Windows or Linux remove the main window's menubar,
  /// See: juce::MainWindow::setMenuBar()
  void shutdown() override;
  
  /// This is called when the app is being asked to quit: you can
  /// ignore this request and let the app carry on running, or call
  /// quit() to allow the app to close.
  /// Your method should perform the following steps:
  /// * Call juce::quit() to quit the app.
  void systemRequestedQuit() override;
  
  /// When another instance of the app is launched while this one is running,
  /// this method is invoked, and the commandLine parameter tells you what
  /// the other instance's command-line arguments were.
  /// Your method should be empty.
  void anotherInstanceStarted (const String& commandLine) override;
  
  //============================================================================
  // ApplicationCommandTarget overrides (JUCEApplicaton is an ApplicationCommandTarget)
  
  /// This returns the list of commands that the app can handle.
  /// Your method should add the ids LATIN_TEXT, CYRILLIC_TEXT, GREEK_TEXT to
  /// the commands array.
  void getAllCommands (Array<CommandID> &commands) override;
  
  /// This provides details about each command the app can perform.
  /// Your method should use juce::ApplicationCommandInfo::setInfo() to provide
  /// a menu name for each command, e.g. "Latin", "Cyrillic" and "Greek".
  void getCommandInfo (CommandID commandID, ApplicationCommandInfo &result) override;
  
  /// This performs command actions and turns true if a command was performed.
  /// Your method should call MainComponent::setTextforId() and pass it the
  /// info's commandID (one of LATIN_TEXT, GREEK_TEXT or CYRILLIC_TEXT).
  /// See: juce::InvocationInfo.
  bool perform (const InvocationInfo &info) override;
  
  //==============================================================================
  // MainApplication members
  
  /// Returns a reference to our running application.  Defining a static function
  /// to do this makes it easy for any code in the application to reference data
  /// associated with the app instance. See: JUCEApplication::getInstance()
  static MainApplication& getApp();
  
  /// Returns our applications menubar model.
  MenuBarModel* getMenuBarModel() ;
  
  /// Menubar's command ID for latin text.
  static const CommandID LATIN_TEXT    = 0x100;
  /// Menubar's command ID for cyrillic text.
  static const CommandID CYRILLIC_TEXT = 0x200;
  /// Menubar's command ID for greek text.
  static const CommandID GREEK_TEXT    = 0x300;
  
  /// The application's command manager.
  ApplicationCommandManager commandManager;
  
private:

  /// Smart pointer managing the app's main window.
  std::unique_ptr<MainWindow> mainWindow;
  /// Forward declare of the application's menu bar defined in the .cpp file.
  struct MainMenuBarModel;
  /// Smart pointer holding the application's MenuBarModel.
  std::unique_ptr<MainMenuBarModel> mainMenuBarModel;
};
