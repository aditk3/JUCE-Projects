//==============================================================================

#include "MidiPianoRoll.h"

//==============================================================================
// PianoRollNote

PianoRollNote::PianoRollNote(int key, int vel) {
    keynum = key;
    velocity = vel;
}

PianoRollNote::~PianoRollNote() {}

void PianoRollNote::paint(Graphics& g) {
    g.setColour(Colours::red);
    g.fillRect(getLocalBounds());
}

//==============================================================================
// MidiPianoRoll

MidiPianoRoll::MidiPianoRoll() { setFramesPerSecond(FPS); }

MidiPianoRoll::~MidiPianoRoll() {}

void MidiPianoRoll::clear() { notes.clear(); }

void MidiPianoRoll::paint(Graphics& g) { g.fillAll(Colours::black); }

void MidiPianoRoll::update() {
    for (int i = 0; i < notes.size(); i++) {
        if (!notes[i].get()->haveNoteOff) {
            notes[i].get()->setBounds(0, notes[i].get()->getY(), notes[i].get()->getWidth() + PPF,
                                      notes[i].get()->getHeight());
        } else {
            notes[i].get()->setBounds(notes[i].get()->getX() + PPF, notes[i].get()->getY(), notes[i].get()->getWidth(),
                                      notes[i].get()->getHeight());
            
            if (notes[i].get()->getX() > getWidth()) {
                notes.erase(notes.begin() + i);
            }
        }
    }
}

void MidiPianoRoll::addMidiMessage(const MidiMessage& msg) {
    if (msg.isNoteOn()) {
        notes.push_back(std::make_unique<PianoRollNote>(msg.getNoteNumber(), msg.getVelocity()));
        float noteHeight = getHeight() / 128.0f;
        int keynum = notes[notes.size() - 1]->keynum;
        int height = noteHeight * (abs(keynum - 127));

        notes[notes.size() - 1]->setBounds(0, height, 0, noteHeight);
        addAndMakeVisible(notes[notes.size() - 1].get());
    }

    if (msg.isNoteOff()) {
        for (int i = 0; i < notes.size(); i++)
            if (msg.getNoteNumber() == notes[i].get()->keynum) notes[i].get()->haveNoteOff = true;
    }
}
