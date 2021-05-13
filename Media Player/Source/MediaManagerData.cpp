bool DEBUG_MODE = false;

#include "MediaManagerData.h"

const juce::Identifier MediaManagerData::MEDIA_MANAGER_DATA_TYPEID{"MediaManagerData"};
const juce::Identifier MediaManagerData::TRANSPORT_PLAYING{"TransportPlaying"};
const juce::Identifier MediaManagerData::TRANSPORT_GAIN{"TransportGain"};
const juce::Identifier MediaManagerData::TRANSPORT_TEMPO{"TransportTempo"};
const juce::Identifier MediaManagerData::TRANSPORT_POSITION{"TransportPosition"};
const juce::Identifier MediaManagerData::TRANSPORT_ENABLED{"TransportEnabled"};
const juce::Identifier MediaManagerData::TRANSPORT_TEMPO_ENABLED{"TransportTempoEnabled"};
const juce::Identifier MediaManagerData::TRANSPORT_CLICK_PLAYPAUSE{"TransportClickPlayPlause"};
const juce::Identifier MediaManagerData::TRANSPORT_REWIND{"TransportRewind"};
const juce::Identifier MediaManagerData::TRANSPORT_PLAYBACK_DURATION{"TransportPlaybackDuration"};
const juce::Identifier MediaManagerData::LOADED_MEDIA_FILE{"LoadedMediaFile"};
const juce::Identifier MediaManagerData::LOADED_MEDIA_TYPE{"LoadedMediaType"};
const juce::Identifier MediaManagerData::MIDI_OUTPUT_OPEN_ID{"MidiOutputOpenID"};
const juce::Identifier MediaManagerData::INTERNAL_SYNTH_AVAILABLE{"InternalSynthAvailable"};
const juce::Identifier MediaManagerData::LAUNCH_OPEN_MEDIA_DIALOG{"LaunchOpenMediaDialog"};
const juce::Identifier MediaManagerData::LAUNCH_MEDIA_INFO_DIALOG{"LaunchMediaInfoDialog"};

bool MediaManagerData::getTransportEnabled() {
    // Returns the TRANSPORT_ENABLED property, or false if it is not set.
    return data.getProperty(TRANSPORT_ENABLED, false);
}

void MediaManagerData::setTransportEnabled(bool shouldBeEnabled) {
    // Sets the TRANSPORT_ENABLED property to shouldBeEnabled.
    data.setProperty(TRANSPORT_ENABLED, shouldBeEnabled, nullptr);
}

bool MediaManagerData::getTransportTempoEnabled() {
    // Returns the TRANSPORT_TEMPO_ENABLED property or false if it is not set.
    return data.getProperty(TRANSPORT_TEMPO_ENABLED, false);
}

void MediaManagerData::setTransportTempoEnabled(bool shouldBeEnabled) {
    // Sets the TRANSPORT_TEMPO_ENABLED property to shouldBeEnabled.
    data.setProperty(TRANSPORT_TEMPO_ENABLED, shouldBeEnabled, nullptr);
}

bool MediaManagerData::getTransportPlaying() {
    // Returns the TRANSPORT_PLAYING property, or false if it is not set.
    return data.getProperty(TRANSPORT_PLAYING, false);
}

void MediaManagerData::setTransportPlaying(bool shouldBePlaying, ValueTree::Listener* exclude) {
    // Sets the TRANSPORT_ENABLED property to shouldBePlaying and excludes the listener, which defaults to nullptr.
    data.setPropertyExcludingListener(exclude, TRANSPORT_PLAYING, shouldBePlaying, nullptr);
}

double MediaManagerData::getTransportGain() {
    // Returns the TRANSPORT_GAIN property or 1.0 if it is not set.
    return data.getProperty(TRANSPORT_GAIN, 1.0);
}

void MediaManagerData::setTransportGain(double gain, ValueTree::Listener* exclude) {
    // Sets the TRANSPORT_GAIN property to gain and excludes the listener, which defaults to nullptr.
    data.setPropertyExcludingListener(exclude, TRANSPORT_GAIN, gain, nullptr);
}

double MediaManagerData::getTransportTempo() {
    // Returns the TRANSPORT_TEMPO property or 60.0 if it is not set.
    return data.getProperty(TRANSPORT_TEMPO, 60.0);
}

void MediaManagerData::setTransportTempo(double tempo, ValueTree::Listener* exclude) {
    // Sets the TRANSPORT_TEMPO property to tempo and excludes the listener, which defaults to nullptr.
    data.setPropertyExcludingListener(exclude, TRANSPORT_TEMPO, tempo, nullptr);
}

double MediaManagerData::getTransportPosition() { return data.getProperty(TRANSPORT_POSITION, 0.0); }

void MediaManagerData::setTransportPosition(double pos, ValueTree::Listener* exclude) {
    data.setPropertyExcludingListener(exclude, TRANSPORT_POSITION, pos, nullptr);
}

void MediaManagerData::clickPlayPause(ValueTree::Listener* exclude) {
    // Increments a TRANSPORT_CLICK_PLAYPAUSE value each time so listeners always fire (the value incremented is
    // unimportant). We can't use the sendPropertyChangeMessage() for this property because that method does not support
    // excluding listeners.

    int val = data.getProperty(TRANSPORT_CLICK_PLAYPAUSE, 0);
    data.setPropertyExcludingListener(exclude, TRANSPORT_CLICK_PLAYPAUSE, val + 1, nullptr);
}

void MediaManagerData::setTransportRewind(ValueTree::Listener* exclude) {
    // Increments a TRANSPORT_REWIND value each time so listeners always fire (the value incremented is unimportant).
    // We can't use the sendPropertyChangeMessage() for this property because that method does not support excluding
    // listeners.

    int val = data.getProperty(TRANSPORT_REWIND, 0);
    data.setPropertyExcludingListener(exclude, TRANSPORT_REWIND, val + 1, nullptr);
}

double MediaManagerData::getPlaybackDuration() {
    // Returns the TRANSPORT_PLAYBACK_DURATION property or 0.0 if it is not set.
    return data.getProperty(TRANSPORT_PLAYBACK_DURATION, 0.0);
}

void MediaManagerData::setPlaybackDuration(double time, ValueTree::Listener* exclude) {
    // Sets the TRANSPORT_PLAYBACK_DURATION property to tempo and excludes the listener, which defaults to nullptr.
    data.setPropertyExcludingListener(exclude, TRANSPORT_PLAYBACK_DURATION, time, nullptr);
}

File MediaManagerData::getLoadedMediaFile() {
    // Return a File(pathname) object from the LOADED_MEDIA_FILE pathname (string), or File() if it is empty.
    String path = data.getProperty(LOADED_MEDIA_FILE, "");

    if (path.isEmpty()) return File();

    return File(path);
}

void MediaManagerData::setLoadedMediaFile(const File& file) {
    // Sets the LOADED_MEDIA_FILE property to the full pathname of the file.
    DBG("set loaded media file");
    data.setProperty(LOADED_MEDIA_FILE, file.getFullPathName(), nullptr);
}

int MediaManagerData::getLoadedMediaType() {
    // Returns the LOADED_MEDIA_TYPE property or MEDIA_NONE if it is not set.
    return data.getProperty(LOADED_MEDIA_TYPE, MEDIA_NONE);
}

void MediaManagerData::setLoadedMediaType(int mediaType) {
    // Sets the LOADED_MEDIA_TYPE property to mediaType.
    data.setProperty(LOADED_MEDIA_TYPE, mediaType, nullptr);
}

int MediaManagerData::getMidiOutputOpenID() { return 0; }

void MediaManagerData::setMidiOutputOpenID(int ident, ValueTree::Listener* exclude) {}

bool MediaManagerData::isInternalSynthAvailable() { return false; }

void MediaManagerData::setInternalSynthAvailable(bool isAvailable) {
    // Sets the INTERNAL_SYNTH_AVAILABLE property to isAvailable.
}

void MediaManagerData::launchOpenMediaDialog() {
    // Calls sendPropertyChangeMessage() for the LAUNCH_OPEN_MEDIA_DIALOG property so the MediaManager will launch its
    // Open File dialog.
    if (DEBUG_MODE) DBG("Called launchOpenMediaDialog");
    data.sendPropertyChangeMessage(LAUNCH_OPEN_MEDIA_DIALOG);
}

void MediaManagerData::launchMediaInfoDialog() {
    // Calls sendPropertyChangeMessage() for the LAUNCH_MEDIA_INFO_DIALOG property so the MediaManager will launch its
    // Open File dialog.
    data.sendPropertyChangeMessage(LAUNCH_MEDIA_INFO_DIALOG);
}
