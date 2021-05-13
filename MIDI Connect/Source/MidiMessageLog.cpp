//==============================================================================
#include "MidiMessageLog.h"

MidiMessageLog::MidiMessageLog() { setModel(this); }

MidiMessageLog::~MidiMessageLog() {}

//==============================================================================
/// ListBoxModel overrides.

int MidiMessageLog::getNumRows() { return messageLog.size(); }

void MidiMessageLog::paintListBoxItem(int row, Graphics& g, int width, int height, bool) {
    g.setColour(Colours::white);
    g.setFont(Font(15));
    g.drawText(messageLog[row], 4, 4, width, height, Justification::centredLeft);
}

//==============================================================================
/// AsyncUpdater override.

void MidiMessageLog::handleAsyncUpdate() {
    updateContent();
    scrollToEnsureRowIsOnscreen(getNumRows());
    repaint();
}

//==============================================================================
// MidiMessageLog methods.

void MidiMessageLog::addMidiMessage(const MidiMessage& message) {
    messageLog.add(midiMessageString(message));
    triggerAsyncUpdate();
}

void MidiMessageLog::clear() {
    messageLog.clear();
    triggerAsyncUpdate();
}

const String MidiMessageLog::midiMessageString(const MidiMessage& msg) {
    if (msg.isNoteOn()) {
        String str = "[" + String(msg.getTimeStamp()) + "] " + "NoteOn" + ": channel=" + String(msg.getChannel()) +
                     ", key=" + String(msg.getNoteNumber()) + ", velocity=" + String(msg.getVelocity()) +
                     ", pitch=" + keynumToPitch(msg.getNoteNumber()) +
                     ", freq=" + String(keynumToFrequency(msg.getNoteNumber()));

        return str;
    }

    if (msg.isNoteOff()) {
        String str = "[" + String(msg.getTimeStamp()) + "] " + "NoteOff" + ": channel=" + String(msg.getChannel()) +
                     ", key=" + String(msg.getNoteNumber()) + ", velocity=" + String(msg.getVelocity());

        return str;
    }

    if (msg.isProgramChange()) {
        String str = "[" + String(msg.getTimeStamp()) + "] " + "ProgramChange" +
                     ": channel=" + String(msg.getChannel()) + ", number=" + String(msg.getProgramChangeNumber());

        return str;
    }

    if (msg.isPitchWheel()) {
        String str = "[" + String(msg.getTimeStamp()) + "] " + "PitchWheel" + ": channel=" + String(msg.getChannel()) +
                     ", value=" + String(msg.getPitchWheelValue());

        return str;
    }

    if (msg.isController()) {
        String str = "[" + String(msg.getTimeStamp()) + "] " + "ControlChange" +
                     ": channel=" + String(msg.getChannel()) +
                     ", controller=" + String(msg.getControllerName(msg.getControllerNumber())) +
                     ", value=" + String(msg.getControllerValue());

        return str;
    }

    return msg.getDescription();
}

float MidiMessageLog::keynumToFrequency(int keynum) { return (float)440.0 * pow(2.0f, (keynum - 69.0f) / 12.0f); }

String MidiMessageLog::keynumToPitch(int keynum) {
    using namespace std;
    map<int, String> pNDict = {{0, "C"},  {1, "C#"}, {2, "D"},  {3, "Eb"}, {4, "E"},   {5, "F"},
                               {6, "F#"}, {7, "G"},  {8, "Ab"}, {9, "A"},  {10, "Bb"}, {11, "B"}};

    int pitchClass = keynum % 12;
    int octave = keynum / 12;

    int finalPitch = pitchClass - (0);
    if (finalPitch < 0) {
        finalPitch = 12 + finalPitch;
        octave -= 1;
    }

    if (finalPitch > 11) {
        finalPitch = finalPitch - 12;
        octave += 1;
    }

    auto answer = pNDict[finalPitch];
    octave -= 1;

    String oct = String(octave);
    if (oct == "-1") oct = "00";

    return String(answer) + oct;
}

float MidiMessageLog::velocityToAmplitude(int velocity) { return (float)velocity / 127; }
