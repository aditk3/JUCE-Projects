//==============================================================================
// MainComponent.h
// This file defines the class representing our main window content component.
//==============================================================================

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

/// MainComponent lives inside our window and defines almost all the app's GUI
/// controls and content.  MainComponent inherits from three "listener" (callback)
/// classes so it can respond to mouse input from the user.
class MainComponent : public Component, public Button::Listener, public Slider::Listener, public ComboBox::Listener {
   public:
    /// The MainComponent constructor.
    /// Your method should perform the following actions:
    /// * For each GUI subcomponent call MainComponent::addAndMakeVisible() and pass it the subcomponent
    /// to add (see the private variables section in the header file for the list of subcomponents).
    /// * For each  Button, Slider and ComboBox subcomponent, call it's addListener() method and pass
    /// it the MainComponent instance.
    /// * For colorMenu:
    ///   * Add menu items for the color names "Navy", "Purple", "Brown", "Green", "Black".
    ///     The menu IDs for the colors should start with 1.
    ///     Hint: a fast way to add these names is to use ComboBox.addItemList().
    ///   * Call its setTextWhenNothingSelected() method and pass it the string "Colors".
    /// * For transparencySlider:
    ///   * Set its range 0.0 to 1.0.
    /// * For fontSizeSlider:
    ///   * Set its slider style to Slider::IncDecButtons.
    ///   * Set its text box style to Slider::TextBoxLeft, false, 30, fontSizeSlider.getTextBoxHeight().
    ///   * Set its range to 8.0, 32.0, and 1.0.
    ///   * Set its value to textFont.getHeight(), and NotificationType::dontSendNotification.
    ///   * Set its range 0.0 to 1.0.
    /// * For the textEditor:
    ///   * Set its font to textFont.
    ///   * Set it to be mult-line.
    ///   * Set it so that the Return key starts a new line.
    ///   * Set it to be read-only.
    /// * Call setVisible(true) on our main component. <b>Note</b>: technically this isn't necessary here
    /// because our component will be the window's content component and the window will automatically
    /// do this for us. However, in all other cases you'll need to explicity set a component's visbility
    /// to true before you can see it, so get used to it!
    MainComponent();

    /// The class destructor. Put any cleanup code you need to
    /// execute here, and it'll happen when the instance is deleted.
    /// Your method should be empty.
    ~MainComponent();

    //==============================================================================
    // Component overrides

    /// Called to paint the component. Not all components need paint
    /// functions, but many at least set the component's backgroud colour.
    /// Your method should perform the following actions:
    /// * Completely fill the background with juce::ResizableWindow's backgroundColour.
    /// To do this you will call the app's LookAndFeel method findColour() and pass it
    /// ResizableWindow::backgroundColourId. See: Graphics::fillAll(),
    /// ResizableWindow::backgroundColourId, getLookAndFeel(), getLocalBounds().
    /// * Fill the area behind the textEditor using Graphics::fillCheckerBoard().
    /// Make the size of each square in the checkerboard one ninth the width and
    /// height of the component. The alternating colors for the squares should be
    /// juce::Colours::lightgrey and juce::Colours::white.
    void paint(Graphics&) override;

    /// The resized() method is called to update subcomponent positions and sizes.
    /// Your method should perform the following actions:
    /// * Provide an 8 pixel padding on all sides of MainComponent (i.e. all
    /// subcomponents should be inset by 8 pixels from the four edges).
    /// * The height of all subcomponents except the textBox should be 24 pixels.
    /// * There should be an 8 pixel padding between each row of components.
    /// * Set clearButton's width to 120.
    /// * colorMenu's width is 100 and it is positioned 8 pixels to the right of clearButton.
    /// * transparencyLabel's width is 100 and it is positioned 8 pixels to the right of colorMenu.
    /// * transparencySlider's is positioned to the right of transparencyLabel and its width is the remainder of the
    /// line.
    /// * fontSizeLabel's width is 70.
    /// * fontSizeSlider's width is 80 and it is positioned to the right of fontSizeLabel.
    /// * fontStyleLabel's width is 80 and it is positioned 24 pixels to the right of fontStyleLabel.
    /// * boldStyleButton's width is 60 and it is positioned to the right of fontSizeSlider.
    /// * italicStyleButton's width is 60 and it is positioned to the right of boldStyleButton.
    /// * editTextButton's width is 86 and it is positioned at the far right of the line.
    /// * textEditor takes the remained of the component's area offset by 8 pixels on all sides.
    ///
    /// You can accomplish this layout quite easily using MainComponent's getLocalBounds()
    /// method together with the following Rectangle functions: reduced(), removeFromLeft(),
    /// removeFromRight(), removeFromTop(), removeFromBottom(). Consult the documentation for
    /// more information.
    void resized() override;

    //==============================================================================
    // Listener overrides

    /// Lets our component respond to button clicks.
    /// This callback happens when a user clicks a button that has registered
    /// our MainComponent as a listener. Since the MainComponent is listening
    /// to more than one button the method will need to determine which button
    /// was clicked by checking each button against the button passed into the
    /// callback.
    ///
    /// Your method should perform the following actions:
    /// * If clearButton was clicked the textEditor should be set to an empty string.
    /// * If the editTextButton was clicked the textEditor should be set to an
    /// empty string.
    /// * If boldStyleButton is toggled on then textEditor's font style should be
    /// Font::bold, otherwise it should be not be bold. See: Button::getToggleState(),
    /// Font::setStyleFlags(), TextEditor::applyFontToAllText().
    /// * If italicStyleButton is toggled on then textEditor's font style should
    ///  be Font::italic, otherwise it should be not be italic.
    void buttonClicked(Button* buttonClicked) override;

    /// Lets our component respond to slider changes. This callback happens when
    /// a user clicks a slider that has registered our MainComponent as a listener.
    ///
    /// Your method should perform the following actions:
    /// * As transparencySlider's value moves from 0.0 to 1.0 the textEditor's
    /// opacity should move from 1.0 to 0.0. See: Slider::getValue(),
    /// TextEditor::setAlpha()
    void sliderValueChanged(Slider* sliderMoved) override;

    /// Lets our component respond to combo menu changes.
    void comboBoxChanged(ComboBox* menu) override;

    //==============================================================================
    // MainComponent members

    /// Sets the window's text for the given textId, one of LATIN_TEXT, CYRILLIC_TEXT,
    /// or GREEK_TEXT.
    void setTextForId(const int textId);

   private:
    /// Initialize textButton to "Clear Message"
    TextButton clearButton;
    /// Initialize transparencyLabel to  "Transparency:"
    Label transparencyLabel;
    /// Initialize transparencySlider to LinearHorizontal and NoTextBox. See: juce::Slider.
    Slider transparencySlider;
    /// The colorMenu will be configured in the constructor.
    ComboBox colorMenu;
    /// Initialize fontSizeLabel to "Font size:"
    Label fontSizeLabel;
    /// The fontSizeSlider will be configured in the constructor.
    Slider fontSizeSlider;
    /// Initialize fontStyleLabel to "Font style:".
    Label fontStyleLabel;
    /// Initialize boldStyleButton to "Bold".
    ToggleButton boldStyleButton;
    /// Initialize italicStyleButton to "Italic".
    ToggleButton italicStyleButton;
    /// Initialize editTextButton to "Edit Text".
    ToggleButton editTextButton;
    /// The textEditor will be configured in the constructor.
    TextEditor textEditor;
    /// Initialize textFont to "Times", size 15.0 and style plain. See: juce::Font.
    Font textFont;
    /// Initialize the colors array to navy, purple, darkred, darkgreen, black. See: juce::Colours.
    std::vector<Colour> colors{Colours::navy, Colours::purple, Colours::darkred, Colours::darkgreen, Colours::black};

    /// Latin text (from http://generator.lorem-ipsum.info)
    String latinText{
        "Lorem ipsum dolor sit amet, labores deseruisse vix in, ex nobis eruditi qui! Id amet commune sit, at per "
        "oportere iudicabit intellegat, fabulas quaerendum in eum. Ut laudem equidem mea, copiosae inimicus consequat "
        "ius et? Ad pri augue labores expetenda, phaedrum postulant eu pro. Ei etiam vidisse petentium per?"};
    /// Greek text requires CharPointer_UTF8 so string chars are interpreted as UTF8.
    CharPointer_UTF8 greekText{
        "Λορεμ ιπσθμ δολορ σιτ αμετ, qθι αν αθτεμ ηαβεμθσ, τε vιμ σιντ τατιον ινιμιcθσ, αν νθμqθαμ ιραcθνδια vιξ. Μει "
        "ινιμιcθσ σιγνιφερθμqθε εθ. Τατιον ποστεα γραεcισ σεα νε, προ αμετ ομνιθμ cομμθνε ατ. Παρτεμ ομνιθμ μεα εα, "
        "ορνατθσ τηεοπηραστθσ ετ."};
    /// Cyrillic text requires CharPointer_UTF8 so string chars are interpreted as UTF8.
    CharPointer_UTF8 cyrillicText{
        "Лорем ипсум долор сит амет, еи долоре путент еирмод иус, моллис луцилиус еа хис. Мел цасе фацилисис те, "
        "хомеро цорпора праесент сед ин, аццусата губергрен либерависсе те еум! Еа вел легимус витуперата, утамур "
        "делицатиссими вел ан. Яуи но цонгуе лаборес. Пер амет аугуе дебет еу. Иус еи цонгуе легере."};

    enum MenuColours { navy = 1, purple, brown, green, black };

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
