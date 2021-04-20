//==============================================================================

#include "MainComponent.h"
#include "MainApplication.h"

MainComponent::MainComponent() {
    textFont.setTypefaceName("Times");
    textFont.setHeight(15.0);
    textFont.setStyleFlags(0);
    
    addAndMakeVisible(clearButton);
    clearButton.setButtonText("Clear Messages");
    clearButton.addListener(this);
    
    addAndMakeVisible(transparencyLabel);
    transparencyLabel.setText("Transparency:", dontSendNotification);
    
    addAndMakeVisible(transparencySlider);
    transparencySlider.addListener(this);
    transparencySlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    transparencySlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox,true,0,0);
    transparencySlider.setRange(0.0, 1.0);
    
    addAndMakeVisible(colorMenu);
    colorMenu.addListener(this);
    colorMenu.addItem("Navy", navy);
    colorMenu.addItem("Purple", purple);
    colorMenu.addItem("Brown", brown);
    colorMenu.addItem("Green", green);
    colorMenu.addItem("Black", black);
    colorMenu.setTextWhenNothingSelected("Colors");
    
    addAndMakeVisible(fontSizeLabel);
    fontSizeLabel.setText("Font size:", dontSendNotification);
    
    addAndMakeVisible(fontSizeSlider);
    fontSizeSlider.addListener(this);
    fontSizeSlider.setSliderStyle(Slider::SliderStyle::IncDecButtons);
    fontSizeSlider.setTextBoxStyle(Slider::TextBoxLeft,false,30,fontSizeSlider.getTextBoxHeight());
    fontSizeSlider.setRange(8.0, 32.0, 1.0);
    fontSizeSlider.setValue(textFont.getHeight(), dontSendNotification);
    
    addAndMakeVisible(fontStyleLabel);
    fontStyleLabel.setText("Font style:", dontSendNotification);
    
    addAndMakeVisible(boldStyleButton);
    boldStyleButton.setButtonText("Bold");
    boldStyleButton.addListener(this);
    
    addAndMakeVisible(italicStyleButton);
    italicStyleButton.setButtonText("Italic");
    italicStyleButton.addListener(this);
    
    addAndMakeVisible(editTextButton);
    editTextButton.setButtonText("Edit Text");
    editTextButton.addListener(this);
    
    addAndMakeVisible(textEditor);
    textEditor.setFont(textFont);
    textEditor.setMultiLine(true);
    textEditor.setReadOnly(true);
    textEditor.setReturnKeyStartsNewLine(true);
}

MainComponent::~MainComponent() {
}

//==============================================================================
// Component overrides

void MainComponent::paint (Graphics& g) {
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
    
    
    auto textEditorBounds = textEditor.getBounds().toFloat();
    g.fillCheckerBoard(textEditorBounds, textEditorBounds.getWidth() / 9, textEditorBounds.getHeight() / 9, Colours::lightgrey, Colours::white);
}

void MainComponent::resized() {
    int compHeight = 24, padding = 8;
    
    auto curHeight = padding;
    auto curWidth = padding;
    
    // ROW 1
    clearButton.setBounds(curWidth, curHeight, 120, compHeight);
    curWidth += 120 + padding;
    
    colorMenu.setBounds(curWidth, curHeight, 100, compHeight);
    curWidth += 100 + padding;
    
    transparencyLabel.setBounds(curWidth, curHeight, 100, compHeight);
    curWidth += 100;
    
    transparencySlider.setBounds(curWidth, curHeight, getWidth() - (padding + curWidth), compHeight);
    
    curWidth = padding;
    curHeight += compHeight + padding;
    
    // ROW 2
    fontSizeLabel.setBounds(curWidth, curHeight, 70, compHeight);
    curWidth += 70;
    
    fontSizeSlider.setBounds(curWidth, curHeight, 80, compHeight);
    curWidth += 80 + padding * 3;
    
    fontStyleLabel.setBounds(curWidth, curHeight, 80, compHeight);
    curWidth += 80;
    
    boldStyleButton.setBounds(curWidth, curHeight, 60, compHeight);
    curWidth += 60;
    
    italicStyleButton.setBounds(curWidth, curHeight, 60, compHeight);
    curWidth += 60;
    
    editTextButton.setBounds(getWidth() - (padding + 86), curHeight, 86, compHeight);
    curWidth = padding;
    curHeight += compHeight + padding;
    
    textEditor.setBounds(curWidth, curHeight, getWidth() - padding * 2, getHeight() - curHeight - padding);
}

//==============================================================================
// Listener overrides

void MainComponent::buttonClicked(Button* button ) {
    if (button == &clearButton) {
        textEditor.setText("");
    }
    
    if (button == &editTextButton) {
        if (editTextButton.getToggleState()) {
            textEditor.setReadOnly(false);
        } else {
            textEditor.setReadOnly(true);
        }
    }
        
    if (button == &boldStyleButton || button == &italicStyleButton) {
        if (boldStyleButton.getToggleState() && italicStyleButton.getToggleState()) {
            textFont.setStyleFlags(3);
            textEditor.applyFontToAllText(textFont);
        }
        
        if (boldStyleButton.getToggleState() && !italicStyleButton.getToggleState()) {
            textFont.setStyleFlags(1);
            textEditor.applyFontToAllText(textFont);
        }
        
        if (!boldStyleButton.getToggleState() && italicStyleButton.getToggleState()) {
            textFont.setStyleFlags(2);
            textEditor.applyFontToAllText(textFont);
        }
        
        if (!boldStyleButton.getToggleState() && !italicStyleButton.getToggleState()) {
            textFont.setStyleFlags(0);
            textEditor.applyFontToAllText(textFont);
        }
    }
}

void MainComponent::sliderValueChanged(Slider* slider) {
    if (slider == &transparencySlider) {
        textEditor.setAlpha(1 - transparencySlider.getValue());
    }
    
    if (slider == &fontSizeSlider) {
        textFont.setHeight(fontSizeSlider.getValue());
        textEditor.applyFontToAllText(textFont);
    }
}

void MainComponent::comboBoxChanged(ComboBox* menu) {
    if (menu == &colorMenu) {
        textEditor.setColour(TextEditor::backgroundColourId, colors[colorMenu.getSelectedId() - 1]);
        repaint();
    }
}

//==============================================================================
// MainComponent methods

void MainComponent::setTextForId(const int id) {
}
