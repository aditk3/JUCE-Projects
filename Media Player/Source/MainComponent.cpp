//==============================================================================

#include "MainComponent.h"

#include "MainApplication.h"

//==============================================================================
// Component overrides

MainComponent::MainComponent() : midiOutputMenu(*this) {
    // Set our managerData member variable to the MediaManager's managerData and add this component as a listener of our
    // local copy.
    managerData = MainApplication::getApp().getMediaManager()->getManagerData();

    managerData.addListener(this);

    // Add the openButton and make it visible, add this component as its listener.
    addAndMakeVisible(openButton);
    openButton.addListener(this);

    // Add and make visible the infoButton. Call setEnabled(false) to disabled it and add this component as a listener.
    addAndMakeVisible(infoButton);
    infoButton.setEnabled(false);
    infoButton.addListener(this);

    // Add the midiOutputMenu and make visible. When nothing is selected is should display "MIDI Outputs". it and add
    // this component as a listener.
    addAndMakeVisible(midiOutputMenu);
    midiOutputMenu.addListener(this);
    midiOutputMenu.setTextWhenNothingSelected("MIDI Outputs");

    // Create the Transport giving it our local managerData and make it visible. The transport's initial state should
    // disabled and our component should be its listener.
    transport = std::make_unique<Transport>(managerData);
    addAndMakeVisible(transport.get());
    transport->setEnabled(false);

    setVisible(true);

    auto col = getLookAndFeel().findColour(DocumentWindow::backgroundColourId);
    DBG(String(col.getRed()) + String(col.getBlue()) + String(col.getGreen()));
}

MainComponent::~MainComponent() {
    // remove this component as a listener of managerData.
    managerData.removeListener(this);
}

void MainComponent::paint(Graphics& g) {
    g.fillAll(getLookAndFeel().findColour(DocumentWindow::backgroundColourId));

    g.setColour(Colours::red);
    //    for (int i = 0; i < getNumChildComponents(); i++) g.drawRect(getChildComponent(i)->getBounds(), 1);
}

void MainComponent::resized() {
    // The top line of buttons has a height of 24.
    double compHeight{24.0};
    double padding{8.0};

    // The widths of the openButton and infoButton buttons are 90.
    double buttonWidth{90.0};

    // All subcomponents are inset from the component's edges by 8 pixels.

    auto bounds = getBounds().reduced(padding);
    auto copy = getBounds();

    auto topLine = bounds.removeFromTop(compHeight);

    openButton.setBounds(topLine.removeFromLeft(buttonWidth));
    infoButton.setBounds(topLine.removeFromRight(buttonWidth));

    // The midiOutputMenu is center-justified with a width of 180.
    midiOutputMenu.setBounds((copy.getWidth() / 2) - (180 / 2), padding, 180, compHeight);

    // The transport is already sized, center it in the remaining area 8 pixels below the top line.
    copy.removeFromTop(compHeight + padding);
    transport.get()->setBounds((getWidth() / 2) - (transport.get()->getWidth() / 2),
                               copy.getY() + (copy.getHeight() / 2 - transport.get()->getHeight() / 2),
                               transport.get()->getWidth(), transport.get()->getHeight());
}

//==============================================================================
// JUCE Listener overrides

void MainComponent::buttonClicked(Button* button) {
    if (button == &openButton) {
        // If the openButton is clicked call managerData's launchOpenMediaDialog() method.
        managerData.launchOpenMediaDialog();
    }

    if (button == &infoButton) {
        // If the infoButton is clicked call managerData's launchMediaInfoDialog() method.
        managerData.launchMediaInfoDialog();
    }
}

void MainComponent::comboBoxChanged(ComboBox* menu) {
    if (menu == &midiOutputMenu) {
        managerData.setMidiOutputOpenID(menu->getSelectedId());
    }
}

//==============================================================================
// ValueTree::Listener overrides

void MainComponent::valueTreePropertyChanged(juce::ValueTree& tree, const juce::Identifier& ident) {
    auto mediaType = managerData.getLoadedMediaType();

    if (ident == MediaManagerData::TRANSPORT_PLAYING) {
        // If the MediaManagerData::TRANSPORT_PLAYING property changed:
        // Use managerData.getTransportPlaying() to see if pausing.
        bool isPlaying = managerData.getTransportPlaying();

        // If the transport is pausing then enable the openButton, otherwise disable it.
        openButton.setEnabled(!isPlaying);

        // If the MEDIA_TYPE is MEDIA_MIDI enable the midiOutputMenu if the transport is pausing otherwise enable it.
        //        if (mediaType == MediaManagerData::MEDIA_MIDI) {
        //            if (isPlaying) {
        //                midiOutputMenu.setEnabled(true);
        //            } else {
        //                midiOutputMenu.setEnabled(false);
        //            }
        //        }
    }

    if (ident == MediaManagerData::LOADED_MEDIA_TYPE) {
        DBG("Loaded type");
        // If the MediaManagerData::LOADED_MEDIA_TYPE property changed:
        switch (mediaType) {
            // if the mediaType variable is MEDIA_AUDIO:
            case MediaManagerData::MEDIA_AUDIO:
                // Enable the infoButton.
                infoButton.setEnabled(true);

                // Disable the midiOutputMenu.
                midiOutputMenu.setEnabled(false);

                // Set managerData.setTransportEnabled() to true.
                DBG("Main comp -> enabled transport");
                managerData.setTransportEnabled(true);

                // Set managerData.setTransportTempoEnabled() to false.
                managerData.setTransportTempoEnabled(false);
                break;

            case MediaManagerData::MEDIA_NONE:
                infoButton.setEnabled(false);
                midiOutputMenu.setEnabled(false);
                managerData.setTransportEnabled(false);
                managerData.setTransportTempoEnabled(false);
                break;

            default:
                break;
        }

        // if the mediaType variable is MEDIA_MIDI:
        // Enable the infoButton.
        // Enable the midiOutputMenu.
        // Set managerData.setTransportEnabled() to true.
        // Set managerData.setTransportTempoEnabled() to false.
        // if the mediaType variable is MEDIA_NONE:
        // Disable the infoButton.
        // Disable the midiOutputMenu.
        // Set managerData.setTransportEnabled() to false.
        // Set managerData.setTransportTempoEnabled() to false.
    }
}
