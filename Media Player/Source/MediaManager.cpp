bool DBG = true;

#include "MediaManager.h"

#include "MainApplication.h"

MediaManager::MediaManager() {
    // Initialize the audioDeviceManager: (0, 2, nullptr, true).
    auto errorMessage = audioDeviceManager.initialise(0, 2, nullptr, true);
    // use jassert to ensure audioError is empty
    jassert(errorMessage.isEmpty());

    // Add this MediaManager as listener on the managerData value tree.
    managerData.addListener(this);

    // Register the default formats for formatManager (See: AudioFormatManager)
    formatManager.registerBasicFormats();

    // Add the audioSourcePlayer to the AudioDeviceManager so it will continuously stream data.
    audioDeviceManager.addAudioCallback(&audioSourcePlayer);

    // Make the transportSource the source for the audioSourcePlayer.
    audioSourcePlayer.setSource(&transportSource);

    // The component is a timer, start it running every 50ms.
    startTimer(50);
}

MediaManager::~MediaManager() {
    // Stop the timer.
    stopTimer();

    // Call removeListener() to stop this component from listening to managerData.
    managerData.removeListener(this);

    // Stop the playbackThread, allowing 100ms for it to stop.
    //    playbackThread.get()->stopThread(100);

    // Delete the playbackThread
    //    playbackThread.release();

    // Call sendAllSoundsOff() to make sure the synth stops playing any notes it already has.
    //    sendAllSoundsOff();

    // Set the transportSource's source to null, set the audioSource's source to null and remove the auioSourcePlayer
    // from the AudioDevice manager
    transportSource.setSource(nullptr);
    audioSourcePlayer.setSource(nullptr);
    audioDeviceManager.removeAudioCallback(&audioSourcePlayer);
}

///=============================================================================
/// The ManagerData callbacks

void MediaManager::valueTreePropertyChanged(juce::ValueTree& tree, const juce::Identifier& ident) {
    auto mediaType = managerData.getLoadedMediaType();

    if (ident == MediaManagerData::LAUNCH_OPEN_MEDIA_DIALOG) {
        // If the MediaManagerData::LAUNCH_OPEN_MEDIA_DIALOG property changed call openMediaFile().
        openMediaFile();
    }

    if (ident == MediaManagerData::TRANSPORT_PLAYING) {
        // If the MediaManagerData::TRANSPORT_PLAYING property changed:
        // If the Transport is playing call either playMidi() or playAudio() depending on the value of mediaType else
        // (the Transport is pausing) call either pauseMidi() or pauseAudio() depending on the value of mediaType.
        if (managerData.getTransportPlaying()) {
            if (mediaType == MediaManagerData::MEDIA_AUDIO) playAudio();
            if (mediaType == MediaManagerData::MEDIA_MIDI) playMidi();
        } else {
            if (mediaType == MediaManagerData::MEDIA_AUDIO) pauseAudio();
            if (mediaType == MediaManagerData::MEDIA_MIDI) pauseMidi();
        }
    }

    if (ident == MediaManagerData::TRANSPORT_GAIN) {
        //  If the MediaManagerData::TRANSPORT_GAIN property changed call either setMidiGain() or setAudioGain()
        //  depending on the value of mediaType.

        double gain = managerData.getTransportGain();
        if (mediaType == MediaManagerData::MEDIA_AUDIO) setAudioGain(gain);
        if (mediaType == MediaManagerData::MEDIA_MIDI) setMidiGain(gain);
    }

    if (ident == MediaManagerData::TRANSPORT_TEMPO) {
        // If the MediaManagerData::TRANSPORT_TEMPO property changed then call either setMidiTempo() or setAudioTempo()
        // depending on the value of mediaType.
        double tempo = managerData.getTransportTempo();
        if (mediaType == MediaManagerData::MEDIA_AUDIO) setAudioTempo(tempo);
        if (mediaType == MediaManagerData::MEDIA_MIDI) setMidiTempo(managerData.getTransportTempo());
    }

    if (ident == MediaManagerData::TRANSPORT_REWIND) {
        // If the MediaManagerData::TRANSPORT_REWIND property changed:
        // If the Transport is playing call managerData.clickPlayPause(). Call either rewindMidi() or rewindAudio()
        // depending on the value of mediaType.
        if (managerData.getTransportPlaying()) managerData.clickPlayPause();

        if (mediaType == MediaManagerData::MEDIA_AUDIO) rewindAudio();
        if (mediaType == MediaManagerData::MEDIA_MIDI) rewindMidi();
    }

    if (ident == MediaManagerData::TRANSPORT_POSITION) {
        // If the MediaManagerData::TRANSPORT_POSITION property changed:
        // Get the current Transport playback position. Call either setMidiPlaybackPosition() or
        // setAudioPlaybackPosition() depending on the value of mediaType.
        auto pos = managerData.getTransportPosition();

        if (mediaType == MediaManagerData::MEDIA_AUDIO) setAudioPlaybackPosition(pos);
        if (mediaType == MediaManagerData::MEDIA_MIDI) setMidiPlaybackPosition(pos);
    }

    if (ident == MediaManagerData::MIDI_OUTPUT_OPEN_ID) {
        // If the MediaManagerData::MIDI_OUTPUT_OPEN_ID property changed call managerData.getMidiOutputOpenID() and if
        // its zero call closeMidiOutput() otherwise call openMidiOutput() and be sure to subtract 1 from it the open
        // port id so it becomes a valid device index.
        auto id = managerData.getMidiOutputOpenID();
        if (id == 0)
            closeMidiOutput();
        else
            openMidiOutput(id - 1);
    }

    if (ident == MediaManagerData::LAUNCH_MEDIA_INFO_DIALOG) {
        // If the MediaManagerData::LAUNCH_MEDIA_INFO_DIALOG property changed call openMediaInfoDialog().
        openMediaInfoDialog();
    }
}

///=============================================================================
// PlaybackPositionTimer callback

void MediaManager::timerCallback() {
    // Call either scrollMidiPlaybackPosition() or scrollAudioPlaybackPosition() depending on the value of
    // getLoadedMediaType().
    switch (managerData.getLoadedMediaType()) {
        case MediaManagerData::MEDIA_AUDIO:
            scrollAudioPlaybackPosition();
            break;

        case MediaManagerData::MEDIA_MIDI:
            scrollMidiPlaybackPosition();
            break;

        default:
            break;
    }
}

//==============================================================================
// Generic Media Support
//==============================================================================

void MediaManager::openMediaFile() {
    // Pass (wildcard) matches of all the supported midi and audio file extensions to a juce FileChooser. Midi wild
    // types are "*.mid;*.midi" and audio file types are given by formatManager.getWildcardForAllFormats().
    String audioFileTypes = formatManager.getWildcardForAllFormats();
    FileChooser myChooser("Open Media File", File(), audioFileTypes);

    // Call browseForFileToOpen() and if it returns true then call loadMidiFile() if the file is a midi file else call
    // loadAudioFile(). See: FileChooser.browserForFileToOpen().
    if (myChooser.browseForFileToOpen()) {
        File audioFile(myChooser.getResult());
        loadAudioFile(audioFile);
    }
}

void MediaManager::openMediaInfoDialog() {
    // Create a TextEditor to hold the info. Configure the editor to be multiline, readonly, sized 400x200. Then give it
    // the text string.
    auto ptr = std::unique_ptr<TextEditor>(new TextEditor());

    ptr.get()->setSize(400, 200);
    ptr.get()->setMultiLine(true);
    ptr.get()->setReadOnly(true);
    ptr.get()->setSize(400, 200);
    ptr.get()->setText(getMediaInfo());

    // Allocate a juce::DialogWindow::LaunchOptions struct and fill it. The dialog's titlebar should be native, it
    // should be resizable, the title should be "Media Info" and the background color our application's backgroundColor.
    DialogWindow::LaunchOptions dw;

    dw.useNativeTitleBar = true;
    dw.resizable = false;
    dw.dialogTitle = "Media Info";
    dw.dialogBackgroundColour = Colour(50, 62, 68);

    dw.content.setOwned(ptr.get());
    ptr.release();
    dw.launchAsync();
}

const String MediaManager::getMediaInfo() {
    // If the managerData's loaded media type is MEDIA_MIDI then return getMidiInfo() else if its MEDIA_AUDIO return
    // getAudioInfo() else return a null string.

    switch (managerData.getLoadedMediaType()) {
        case MediaManagerData::MEDIA_MIDI:
            return getMidiInfo();

        case MediaManagerData::MEDIA_AUDIO:
            return getAudioInfo();

        default:
            return "";
    }
}

//==============================================================================
// Audio playback support

void MediaManager::openAudioSettings() {}

const String MediaManager::getAudioInfo() {
    // Your method should collect string containing the following information from the loaded audio file or its
    // audioFileReaderSource, each on its own line terminated by a return character ("\n"):
    String msg;
    File f = managerData.getLoadedMediaFile();

    // "Audio file:" The pathname of the audio file
    msg += "Audio file: " + f.getFullPathName() + "\n";

    // "File size:" The size of file in bytes.
    msg += "File size: " + String(f.getSize()) + "\n";

    // TODO: "Audio format:" The audio format of the file.
    //        msg += "Audio format: " + formatManager.get

    // TODO: "Channels:" The number of channels in the audio file.
    //    msg += "Channels: " + String(managerData.c) + "\n";

    // TODO: FINISH THIS FUNC

    // "Sample rate:" The sample rate of the file.
    // "Sample frames:" The number of audio frames in the file.
    // "Bits per sample:" The bits per sample.
    // "Floating point data:" Either "yes" or "no".
    // Access the reader's metadata values (getAllKeys() and getAllValues()) and print the list of 'key' and 'value'
    // pairs each on its own line.
    return msg;
}

void MediaManager::loadAudioFile(File audioFile) {
    if (DBG) DBG("Loading audio file...");

    // Try to create an AudioFormatReader for the given audio file, if you can then call loadIntoTrasport() to load it
    // and update managerData with the the new midi file and media type (MEDIA_AUDIO). If a reader is not created open
    // an AlertWindow telling the user that the audio file was an unsupported format, then zero out the mediaFile and
    // mediaType in the managerData (use File() and MEDIA_NONE.
    auto reader = formatManager.createReaderFor(audioFile);

    if (reader != nullptr) {
        loadIntoTransport(reader);
    } else {
        // TODO: open an AlertWindow
        DBG("Invalid file type");
        audioFile = File();
    }

    // Call managerData.setLoadedMediaFile() and pass it the file, this will be a valid file if loaded otherwise an
    // empty file. e.g. File().
    managerData.setLoadedMediaFile(audioFile);

    // Call managerData.setLoadedMediaType() and pass it MEDIA_AUDIO if the file was loaded into the transport,
    // otherwise MEDIA_NONE.
    auto mediaType = (audioFile == File()) ? MediaManagerData::MEDIA_NONE : MediaManagerData::MEDIA_AUDIO;
    managerData.setLoadedMediaType(mediaType);
}

void MediaManager::loadIntoTransport(AudioFormatReader* reader) {
    // Call the managerData method to stop the transport playing.
    managerData.setTransportPlaying(false);

    // Call the managerData method to set the transport's position to 0.
    managerData.setTransportPosition(0.0);

    // Call clearAudioPlaybackState() to zero out the MediaManager's playback data.
    clearAudioPlaybackState();

    // Create a new AudioFormatReaderSource for the reader.
    audioFileReaderSource = std::make_unique<AudioFormatReaderSource>(reader, true);

    // Pass that reader source to the transportSource using its setSource() method (you will also need to pass it the
    // reader's sampleRate value.)
    transportSource.setSource(audioFileReaderSource.get(), 0, 0, reader->sampleRate);

    // Call transportSource.getLengthInSeconds() and pass that value to the managerData method that sets the total
    // duration of playback.
    managerData.setPlaybackDuration(transportSource.getLengthInSeconds());
}

void MediaManager::clearAudioPlaybackState() {
    // Call transportSource.setSource() and give it a null pointer.
    transportSource.setSource(nullptr);

    // Set audioFileReaderSource to a null pointer.
    audioFileReaderSource = nullptr;
}

//==============================================================================
// MIDI transport callbacks

void MediaManager::playMidi() { std::cout << "MediaManager::playMidi()\n"; }

void MediaManager::pauseMidi() { std::cout << "MediaManager::pauseMidi()\n"; }

void MediaManager::setMidiGain(double gain) { std::cout << "MediaManager::setMidiGain(" << gain << ")\n"; }

void MediaManager::setMidiTempo(double tempo) { std::cout << "MediaManager::setMidiTempo(" << tempo << ")\n"; }

void MediaManager::rewindMidi() { std::cout << "MediaManager::rewindMidi()\n"; }

void MediaManager::setMidiPlaybackPosition(double position) {
    std::cout << "MediaManager::setMidiPlaybackPosition(" << position << ")\n";
}

void MediaManager::scrollMidiPlaybackPosition() { std::cout << "MediaManager::scrollMidiPlaybackPosition()\n"; }

/// Audio transport callbacks

void MediaManager::playAudio() {
    std::cout << "MediaManager::playAudio()\n";
    // If the transportSource has finished playing set its position to 0.0
    if (transportSource.hasStreamFinished()) {
        transportSource.setPosition(0.0);
    }
    // Set the transportSource's gain to the managerData's gain.
    transportSource.setGain(managerData.getTransportGain());
    // Start the transport source.
    transportSource.start();
}

void MediaManager::pauseAudio() {
    std::cout << "MediaManager::pauseAudio()\n";
    // Stop the transportSource.
    transportSource.stop();
}

void MediaManager::setAudioGain(double gain) {
    std::cout << "MediaManager::setAudioGain(" << gain << ")\n";
    // Set the transportSource's gain to the managerData's gain.
    transportSource.setGain(managerData.getTransportGain());
}

void MediaManager::setAudioTempo(double tempo) {
    std::cout << "MediaManager::setAudioTempo(" << tempo << ")\n";
    // nothing to do!
}

void MediaManager::rewindAudio() {
    std::cout << "MediaManager::rewindAudio()\n";
    // set the transportSource's position back to 0.0.
    transportSource.setPosition(0.0);
}

/// Sets the audio transport player's position.
void MediaManager::setAudioPlaybackPosition(double pos) {
    auto playing = managerData.getTransportPlaying();
    std::cout << "media manager: receiving position:" << pos << ", playing:" << managerData.getTransportPlaying()
              << "\n";
    if (pos == 0) {
        // setSource(0) stops popping on rewind and play
        transportSource.setSource(0);
        transportSource.setSource(audioFileReaderSource.get());
        if (playing) transportSource.start();
    } else {
        // std::cout << "transport position=" << position << "\n";
        if (playing) transportSource.stop();  // not sure why this is necessary!
        transportSource.setPosition(pos * transportSource.getLengthInSeconds());
        if (playing) transportSource.start();
    }
}

void MediaManager::scrollAudioPlaybackPosition() {
    double sec = transportSource.getCurrentPosition();
    double dur = transportSource.getLengthInSeconds();
    double pos = sec / dur;
    //    std::cout << "pbPos=" << sec << ", pbDur=" << dur << ", pbrat=" << pos << "\n";
    managerData.setTransportPosition(pos, this);
    // auto-pause if at end-of-file
    if (pos >= 1.0) managerData.clickPlayPause();
}

///==============================================================================

void MediaManager::openMidiOutput(int dev) {
    // Call MidiOutput::openDevice and reset the midiOutputDevice to it.
    midiOutputDevice = MidiOutput::openDevice(dev);
    jassert(midiOutputDevice != nullptr);
}

void MediaManager::closeMidiOutput() {
    // Set the midiOutputDevice to nullptr.
    midiOutputDevice.reset(nullptr);
}

bool MediaManager::isInternalSynthAvailable() { return false; }

///==============================================================================
/// MidiFile Functions

const String MediaManager::getMidiInfo() { return ""; }

void MediaManager::clearMidiPlaybackState() {}

void MediaManager::loadMidiFile(File midiFile) {}

void MediaManager::loadIntoPlayer(MidiFile& midifile) {}

///==============================================================================
/// MidiMessage Functions

void MediaManager::sendMessage(const MidiMessage& message) {
    ScopedLock sl(sendLock);
    if (midiOutputDevice) {
        midiOutputDevice->sendMessageNow(message);
    } else if (isInternalSynthAvailable()) {
        playInternalSynth(message);
    }
}

void MediaManager::playInternalSynth(const MidiMessage& message) {}

void MediaManager::sendAllSoundsOff() {
    std::cout << "Sending allSoundsOff, output port is " << (midiOutputDevice.get() ? "OPEN" : "CLOSED") << "\n";
    for (int i = 1; i <= 16; i++) {
        juce::MidiMessage msg = juce::MidiMessage::allSoundOff(i);
        sendMessage(msg);
    }
}

//==============================================================================
// MidiPlaybackClient callbacks

void MediaManager::handleMessage(const MidiMessage& message) { sendMessage(message); }

void MediaManager::addMidiPlaybackMessages(MidiPlaybackThread::MidiMessageQueue& queue,
                                           MidiPlaybackThread::PlaybackPosition& position) {
    int index = position.index;
    for (; index < position.length; index++) {
        juce::MidiMessageSequence::MidiEventHolder* ev = sequence.getEventPointer(index);
        // skip over non-channel messages
        if (ev->message.getChannel() < 1) continue;
        // skip over noteOffs because we add by pairs with noteOns
        if (ev->message.isNoteOff()) continue;
        // add every message that is at or earlier (e.g. late) than the current time
        if (ev->message.getTimeStamp() <= position.beat) {
            queue.addMessage(new juce::MidiMessage(ev->message));
            if (ev->noteOffObject) {
                queue.addMessage(new juce::MidiMessage(ev->noteOffObject->message));
            }
        } else
            break;
    }
    // index is now the index of the next (future) event or length
    position.index = index;
    if (position.isAtEnd()) std::cout << "Midi playback at end!\n";
}
