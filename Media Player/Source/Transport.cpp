bool shouldDBG = false;

#include "Transport.h"

/// Constructor.
Transport::Transport(const MediaManagerData& mmd) {
    // Set the local managerData member to the copy passed in
    managerData = mmd;

    // add this transport as a listener.
    managerData.addListener(this);

    // Make playPauseButton behave as a toggle button
    playPauseButton.setClickingTogglesState(true);

    // Draw the playPauseButton, add and make it visible, then add this transport as its button listener
    drawPlayButton(playPauseButton);
    addAndMakeVisible(playPauseButton);
    playPauseButton.addListener(this);

    // Draw the goToStartButton, add and make it visible, then add this transport as its button listener.
    drawGoToStartButton(goToStartButton);
    addAndMakeVisible(goToStartButton);
    goToStartButton.addListener(this);

    // Get the gain setting from the manager data, draw the gainButton using that gain setting, then add and make it
    // visible, and add this transport as its button listener.
    drawGainButton(gainButton, managerData.getTransportGain());
    addAndMakeVisible(gainButton);
    gainButton.addListener(this);

    // Set the gainSlider range to 0.0-1.0, initialize it with the manager's gain setting, add and make it visible and
    // add this transport as a slider listener.
    gainSlider.setRange(0.0, 1.0);
    gainSlider.setValue(managerData.getTransportGain());
    addAndMakeVisible(gainSlider);
    gainSlider.addListener(this);

    // Add the tempo slider, set its style to Slider::LinearBar and its text value suffix to " bpm". Its range should be
    // set to 40 to 208 and its initial value to the manager's tempo setting.
    tempoSlider.setSliderStyle(Slider::LinearBar);
    tempoSlider.setTextValueSuffix(" bpm");
    tempoSlider.setRange(40, 208);
    tempoSlider.setValue(managerData.getTransportTempo());
    tempoSlider.updateText();
    addAndMakeVisible(tempoSlider);
    tempoSlider.setNumDecimalPlacesToDisplay(0);

    // Configure the start time label to use Justification::centeredRight, and a font size of 12.0. Add and make
    // visible.
    currentTimeLabel.setJustificationType(Justification::centredRight);
    currentTimeLabel.setFont(Font(12.0f));
    currentTimeLabel.setText("00:00", dontSendNotification);
    addAndMakeVisible(currentTimeLabel);

    // Configure the end time label to use Justification::centeredLeft and a font size of 12.0. Add and make visible.
    endTimeLabel.setJustificationType(Justification::centredLeft);
    endTimeLabel.setFont(Font(12.0f));
    endTimeLabel.setText("100:00", dontSendNotification);
    addAndMakeVisible(endTimeLabel);

    // Add the position slider, set its range to 1.0 and add this component as its listener.
    addAndMakeVisible(positionSlider);
    positionSlider.setRange(0.0, 1.0);
    positionSlider.addListener(this);

    // Set the size of the transport to 250 width and 78 height.
    setBounds(0, 0, 250, 78);

    // Set the component visible.
    setVisible(true);
}

/// Destructor.
Transport::~Transport() {
    // Your method should remove the transport as a listener of managerData.
    managerData.removeListener(this);
}

void Transport::paint(Graphics& gr) {
    gr.setColour(Colours::grey);
    auto bounds = getLocalBounds();
    gr.drawRoundedRectangle((float)bounds.getX(), (float)bounds.getY(), (float)bounds.getWidth(),
                            (float)bounds.getHeight(), 8.0f, 2.0f);

    gr.setColour(Colours::red);
    //        for (int i = 0; i < getNumChildComponents(); i++) gr.drawRect(getChildComponent(i)->getBounds(), 1);
}

void Transport::resized() {
    // All subcomponents should be indented 6 pixels from the bounds of of the transport component.
    double padding{6.0f};
    auto bounds = getLocalBounds().reduced(padding);

    // The playPause button should be centered in the transport with size 36.
    playPauseButton.setBounds(getWidth() / 2 - 36 / 2, bounds.getY(), 36, 36);

    // The goToStartButton and gainButton are size 24, and positioned on opposite sides of the playPauseButton
    // vertically centered within the playPause button's height.
    goToStartButton.setBounds(playPauseButton.getX() - 24,
                              playPauseButton.getY() + (playPauseButton.getHeight() / 2 - 24 / 2), 24, 24);
    gainButton.setBounds(playPauseButton.getX() + playPauseButton.getWidth(),
                         playPauseButton.getY() + (playPauseButton.getHeight() / 2 - 24 / 2), 24, 24);

    // The tempoSlider is centered in height the same as the goToStartButton and occupies the remainer of the left-side
    // space but keeping a 6 pixel indent.
    tempoSlider.setBounds(padding, playPauseButton.getY() + (playPauseButton.getHeight() / 2 - 24 / 2),
                          goToStartButton.getX() - padding * 2, 24);

    // The gainSlider is centered in height the same as the gainButton but starting 6 pixels to the right of the
    // gainButton and taking the rest of the width keeping a 6 pixel indent from the right edge.
    gainSlider.setBounds(gainButton.getX() + gainButton.getWidth() + padding,
                         playPauseButton.getY() + (playPauseButton.getHeight() / 2 - 24 / 2), tempoSlider.getWidth(),
                         24);

    bounds = getLocalBounds().reduced(padding);
    bounds.removeFromTop(36);
    bounds.removeFromTop(bounds.getHeight() / 2 - 12);
    bounds.removeFromBottom(bounds.getHeight() / 2 - 12);

    currentTimeLabel.setBounds(bounds.removeFromLeft(44));
    endTimeLabel.setBounds(bounds.removeFromRight(44));
    positionSlider.setBounds(bounds);
}

//============================================================================
// JUCE Component Callbacks

void Transport::buttonClicked(juce::Button* button) {
    if (button == &playPauseButton) {
        // Flip managerData's play/pause state to its opposite.
        bool state = !managerData.getTransportPlaying();

        // Call managerData.setTransportPlaying() and pass it the new state value of the button. Pass this component as
        // the second value so we don't get retriggered by the callback.
        managerData.setTransportPlaying(state, this);

        drawPlayButton(playPauseButton);
    }

    if (button == &goToStartButton) {
        // Set the position slider value to to zero, used dontSendNotification as the second argument so it doesnt
        // trigger listeners.
        positionSlider.setValue(0.0, dontSendNotification);
        // Call managerData.setTransportRewind().
        managerData.setTransportRewind();
    }

    if (button == &gainButton) {
        // Clicking the gain button either mutes or sets the gainSlider to .5 gain. Pass juce::sendNotificationAsync as
        // the second value.
        if (gainSlider.getValue() == 0)
            gainSlider.setValue(0.5, sendNotificationAsync);
        else
            gainSlider.setValue(0.0, sendNotificationAsync);
    }
}

void Transport::sliderValueChanged(juce::Slider* slider) {
    if (slider == &positionSlider) {
        // Pass the current slider position to managerData.setTransportPosition() passing the transport component
        // ('this') as the second argument to exclude the transport from being called again.
        managerData.setTransportPosition(positionSlider.getValue(), this);
        // Draw the current time label in the Tranport.
        drawCurrentTimeLabel();
    }

    if (slider == &gainSlider) {
        // Pass the current slider position to managerData.setTransportGain() passing the transport component ('this')
        // as the second argument to exclude the transport from being called again.
        managerData.setTransportGain(gainSlider.getValue(), this);
        // Redraw gainButton so it shows the new state.
        drawGainButton(gainButton, gainSlider.getValue());
    }

    if (slider == &tempoSlider) {
        // Pass the current slider position to managerData.setTransportTempo() passing the transport component ('this')
        // as the second argument to exclude the transport from being called again.
        managerData.setTransportTempo(tempoSlider.getValue(), this);
    }
}

//============================================================================
// JUCE ValueTree callbacks (listening to the managerData changes)

void Transport::valueTreePropertyChanged(juce::ValueTree& tree, const juce::Identifier& ident) {
    DBG("Transport -> valueTreePropertyChanged");
    if (ident == MediaManagerData::TRANSPORT_ENABLED) {
        // If the property is MediaManagerData::TRANSPORT_ENABLED then enable or disable the transport component based
        // on the current value of mediaManager.getTransportEnabled()
        setEnabled(managerData.getTransportEnabled());
    }

    if (ident == MediaManagerData::TRANSPORT_TEMPO_ENABLED) {
        // If the property is MediaManagerData::TRANSPORT_TEMPO_ENABLED then enable or disable the tempoSlider based on
        // the current value of managerData.getTransportTempoEnabled().
        tempoSlider.setEnabled(managerData.getTransportTempoEnabled());
    }

    if (ident == MediaManagerData::TRANSPORT_GAIN) {
        // If the property is MediaManagerData::TRANSPORT_GAIN then set the gainSlider to the current value of
        // managerData.getTransportGain() using dontSendNotification as the second value so the slider does not trigger
        // another callback.
        gainSlider.setValue(managerData.getTransportGain(), dontSendNotification);
    }

    if (ident == MediaManagerData::TRANSPORT_TEMPO) {
        // If the property is MediaManagerData::TRANSPORT_TEMPO then set tempoSlider to current value of
        // managerData.getTransportTempo() using dontSendNotification as the second value so the slider does not trigger
        // another callback.
        tempoSlider.setValue(managerData.getTransportTempo(), dontSendNotification);
    }

    if (ident == MediaManagerData::TRANSPORT_CLICK_PLAYPAUSE) {
        // If the property is MediaManagerData::TRANSPORT_CLICK_PLAYPAUSE then trigger a click on the playPauseButton.
        playPauseButton.triggerClick();
    }

    if (ident == MediaManagerData::TRANSPORT_POSITION) {
        // If the property is MediaManagerData::TRANSPORT_POSITION then Set positionSlider to the current value of
        // managerData.getTransportPosition() using dontSendNotification as the second value so the slider does not
        // trigger another callback. Call drawCurrentTimeLabel() to display the current time.
        positionSlider.setValue(managerData.getTransportPosition(), dontSendNotification);
        DBG("TRANSPORT_POSITION, Transport.cpp");
        drawCurrentTimeLabel();
    }

    if (ident == MediaManagerData::TRANSPORT_PLAYBACK_DURATION) {
        // If the property is MediaManagerData::TRANSPORT_PLAYBACK_DURATION property changed the call drawEndTimeLabel()
        // to display the current ending time.
        drawEndTimeLabel();
    }
}

//============================================================================
// Icon Button Drawing

void Transport::drawPlayButton(juce::DrawableButton& button) {
    Path path;
    if (!playPauseButton.getToggleState()) {
        path.addTriangle(Point<float>(0, 0), Point<float>(0, 100), Point<float>(100, 50));
    } else {
        path.addRectangle(0, 0, 42, 100);
        path.addRectangle(100 - 42, 0, 42, 100);
    }

    DrawablePath drawable;
    drawable.setPath(path);
    FillType fill(Colours::white);
    drawable.setFill(fill);
    button.setImages(&drawable);
}

void Transport::drawGoToStartButton(juce::DrawableButton& b) {
    // Juce path drawing done in percentage (100x100)
    juce::DrawablePath image;
    juce::Path path;

    path.addTriangle(100, 0, 100, 100, 13.4, 50);
    path.addRectangle(13.4, 0, 13.4, 100);

    image.setPath(path);
    FillType fill(Colours::white);
    image.setFill(fill);
    b.setImages(&image);
}

void Transport::drawGainButton(juce::DrawableButton& button, double gain) {
    // Juce path drawing done in percentage (100x100)
    juce::DrawablePath drawable;
    juce::Path p;

    // speaker rect from 0 to 30
    p.addRectangle(0, 30, 30, 35);
    // speaker cone from 0 to 45
    p.addTriangle(0, 50, 40, 0, 40, 100);
    // waves start at x=55 spaced 15 apart
    if (gain > 0.1) p.addCentredArc(55, 50, 6, 20, 0, 0, 3.14159f, true);
    if (gain > 0.4) p.addCentredArc(70, 50, 5, 35, 0, 0, 3.14159f, true);
    if (gain > 0.7) p.addCentredArc(85, 50, 5, 50, 0, 0, 3.14159f, true);
    // this makes button image width 100 no matter how many arcs added
    p.startNewSubPath(100, 0);
    drawable.setPath(p);
    drawable.setFill(iconColor);
    button.setImages(&drawable);
}

void Transport::drawCurrentTimeLabel() {
    // Get the floating point playback duration from managerData
    // Scale it by the current value of the transport's positionSlider
    // Call toFormattedTimeString() to convert it to a string
    // Set currentTimeLabel to show that text.
    currentTimeLabel.setText(toFormattedTimeString(managerData.getPlaybackDuration() * positionSlider.getValue()),
                             dontSendNotification);
}

void Transport::drawEndTimeLabel() {
    // Updates endTimeLabel of the transport with the playback duration value from the managerData.
    endTimeLabel.setText(toFormattedTimeString(managerData.getPlaybackDuration()), dontSendNotification);
}

const juce::String Transport::toFormattedTimeString(const double seconds) {
    // Use juce::roundToIntAccurate() to convert the float to integer minutes and seconds return them in the formatted
    // string formatted as "mm:ss".
    String sec = String(roundToIntAccurate(seconds) % 60);
    String min = String(roundToIntAccurate(roundToIntAccurate(seconds) / 60));

    if (sec.length() == 1) sec = "0" + sec;

    if (min.length() == 1) min = "0" + min;

    return String(min) + ":" + String(sec);
}
