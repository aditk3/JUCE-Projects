//==============================================================================

#include "MainApplication.h"
#include "MainComponent.h"

using namespace std;

MainComponent::MainComponent() : deviceManager(MainApplication::getApp().audioDeviceManager), audioVisualizer(2) {
    addAndMakeVisible(settingsButton);
    settingsButton.addListener(this);

    addAndMakeVisible(waveformMenu);
    waveformMenu.addListener(this);
    waveformMenu.setTextWhenNothingSelected("Waveforms");
    waveformMenu.addItemList(noises, WhiteNoise);
    waveformMenu.addSeparator();
    waveformMenu.addItemList(sine, SineWave);
    waveformMenu.addSeparator();
    waveformMenu.addItemList(LF, LF_ImpulseWave);
    waveformMenu.addSeparator();
    waveformMenu.addItemList(BL, BL_ImpulseWave);
    waveformMenu.addSeparator();
    waveformMenu.addItemList(WT, WT_SineWave);
    waveformMenu.addSeparator();

    addAndMakeVisible(levelSlider);
    levelSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxLeft, true, 90, 22);
    levelSlider.setRange(0.0, 1.0);
    levelSlider.addListener(this);

    addAndMakeVisible(levelLabel);
    levelLabel.setJustificationType(Justification::right);

    addAndMakeVisible(freqSlider);
    freqSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxLeft, true, 90, 22);
    freqSlider.setRange(0.0, 5000.0);
    freqSlider.setSkewFactorFromMidPoint(500.0);
    freqSlider.addListener(this);

    addAndMakeVisible(freqLabel);
    freqLabel.setJustificationType(Justification::right);

    addAndMakeVisible(playButton);
    drawPlayButton(playButton, true);
    //    playButton.setEnabled(false);
    playButton.addListener(this);

    addAndMakeVisible(cpuLabel);
    cpuLabel.setJustificationType(Justification::centredRight);

    addAndMakeVisible(cpuUsage);
    cpuUsage.setJustificationType(Justification::centredRight);

    addAndMakeVisible(audioVisualizer);

    deviceManager.addAudioCallback(&audioSourcePlayer);
    startTimer(100);
}

MainComponent::~MainComponent() {
    audioSourcePlayer.setSource(nullptr);
    deviceManager.removeAudioCallback(&audioSourcePlayer);
    deviceManager.closeAudioDevice();
}

//==============================================================================
// Component overrides
//==============================================================================

void MainComponent::paint(Graphics& g) {
    g.fillAll(getLookAndFeel().findColour(DocumentWindow::backgroundColourId));

    g.setColour(Colours::red);

    //    for (int i = 0; i < getNumChildComponents(); i++) g.drawRect(getChildComponent(i)->getBounds(), 1);
}

void MainComponent::resized() {
    double compHeight{24.0};
    double padding{8.0};

    auto bounds = getBounds().reduced(8);

    auto chunk1 = bounds.removeFromTop(2 * compHeight + padding);

    auto settingsAndWaveformsMenu = chunk1.removeFromLeft(118);
    settingsButton.setBounds(settingsAndWaveformsMenu.removeFromTop(compHeight));
    settingsAndWaveformsMenu.removeFromTop(padding);

    waveformMenu.setBounds(settingsAndWaveformsMenu);

    chunk1.removeFromLeft(padding);

    auto transportButtonChunk = chunk1.removeFromLeft(56);
    playButton.setBounds(transportButtonChunk);

    chunk1.removeFromLeft(padding);

    auto levelSliderChunk = chunk1.removeFromTop(compHeight);
    levelLabel.setBounds(levelSliderChunk.removeFromLeft(72));
    levelSlider.setBounds(levelSliderChunk);

    chunk1.removeFromTop(padding);
    freqLabel.setBounds(chunk1.removeFromLeft(72));
    freqSlider.setBounds(chunk1);

    auto bottomChunk = bounds.removeFromBottom(compHeight);
    cpuUsage.setBounds(bottomChunk.removeFromRight(66));
    cpuLabel.setBounds(bottomChunk.removeFromRight(36));

    bounds.removeFromTop(padding);
    audioVisualizer.setBounds(bounds);
}

void MainComponent::drawPlayButton(juce::DrawableButton& button, bool showPlay) {
    juce::Path path;
    if (showPlay) {
        path.addTriangle(Point<float>(0, 0), Point<float>(0, 100), Point<float>(100, 50));
    } else {
        path.addRectangle(0, 0, 42, 100);
        path.addRectangle(100 - 42, 0, 42, 100);
    }

    juce::DrawablePath drawable;
    drawable.setPath(path);
    juce::FillType fill(Colours::white);
    drawable.setFill(fill);
    button.setImages(&drawable);
}

//==============================================================================
// Listener overrides
//==============================================================================

void MainComponent::buttonClicked(Button* button) {
    if (button == &settingsButton) {
        openAudioSettings();
    }

    else if (button == &playButton) {
        if (isPlaying()) {
            audioSourcePlayer.setSource(nullptr);
        } else {
            audioSourcePlayer.setSource(this);
        }

        drawPlayButton(playButton, !isPlaying());
    }
}

void MainComponent::sliderValueChanged(Slider* slider) {
    if (slider == &freqSlider) {
        freq = slider->getValue();
        phaseDelta = freq / srate;
    }

    else if (slider == &levelSlider) {
        level = levelSlider.getValue();
    }
}

void MainComponent::comboBoxChanged(ComboBox* menu) {
    if (menu == &waveformMenu) {
        waveformId = WaveformId(waveformMenu.getSelectedId());
        
        auto id = waveformId;
        if (id == WhiteNoise || id == BrownNoise) {
            freqSlider.setEnabled(false);
            freqLabel.setEnabled(false);
        } else {
            freqSlider.setEnabled(true);
            freqLabel.setEnabled(true);
        }
        
//        audioSourcePlayer.setSource(this);
//        drawPlayButton(playButton, !isPlaying());
    }
}

//==============================================================================
// Timer overrides
//==============================================================================

void MainComponent::timerCallback() {
    cpuUsage.setText(String(deviceManager.getCpuUsage() * 100, 2) + "%", dontSendNotification);
}

//==============================================================================
// AudioSource overrides
//==============================================================================

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    srate = sampleRate;
    phase = 0.0;
    phaseDelta = freq / srate;

    audioVisualizer.setBufferSize(samplesPerBlockExpected);
    audioVisualizer.setSamplesPerBlock(8);

    /*
            switch (waveformId) {
                case WhiteNoise:
                    DBG(random.nextFloat());
                    break;

                case BrownNoise:
                    break;

                case DustNoise:
                    break;

                case SineWave:
                    break;

                case LF_ImpulseWave:
                    break;

                case LF_SquareWave:
                    break;

                case LF_SawtoothWave:
                    break;

                case LF_TriangeWave:
                    break;

                case BL_ImpulseWave:
                    break;

                case BL_SquareWave:
                    break;

                case BL_SawtoothWave:
                    break;

                case BL_TriangeWave:
                    break;

                case WT_SineWave:
                    break;

                case WT_ImpulseWave:
                    break;

                case WT_SquareWave:
                    break;

                case WT_SawtoothWave:
                    break;

                case WT_TriangleWave:
                    break;

                default:
                    break;
            }*/
}

void MainComponent::releaseResources() {}

void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) {
    bufferToFill.clearActiveBufferRegion();
    switch (waveformId) {
        case WhiteNoise:
            whiteNoise(bufferToFill);
            break;
        case DustNoise:
            dust(bufferToFill);
            break;
        case BrownNoise:
            brownNoise(bufferToFill);
            break;
        case SineWave:
            sineWave(bufferToFill);
            break;
        case LF_ImpulseWave:
            LF_impulseWave(bufferToFill);
            break;
        case LF_SquareWave:
            LF_squareWave(bufferToFill);
            break;
        case LF_SawtoothWave:
            LF_sawtoothWave(bufferToFill);
            break;
        case LF_TriangeWave:
            LF_triangleWave(bufferToFill);
            break;
        case BL_ImpulseWave:
            BL_impulseWave(bufferToFill);
            break;
        case BL_SquareWave:
            BL_squareWave(bufferToFill);
            break;
        case BL_SawtoothWave:
            BL_sawtoothWave(bufferToFill);
            break;
        case BL_TriangeWave:
            BL_triangleWave(bufferToFill);
            break;
        case WT_SineWave:
        case WT_ImpulseWave:
        case WT_SquareWave:
        case WT_SawtoothWave:
        case WT_TriangleWave:
            WT_wave(bufferToFill);
            break;
        case Empty:
            break;
    }
    audioVisualizer.pushBuffer(bufferToFill);
}

//==============================================================================
// Audio Utilities
//==============================================================================

double MainComponent::phasor() {
    double p = phase;
    phase = std::fmod(phase += phaseDelta, 1.0);
    //    DBG(phase);
    return p;
}

float MainComponent::ranSamp() { return (random.nextFloat() * 2) - 1; }

float MainComponent::ranSamp(const float mul) { return (ranSamp() * mul); }

float MainComponent::lowPass(const float value, const float prevout, const float alpha) {
    return prevout + alpha * (value - prevout);
}

bool MainComponent::isPlaying() { return audioSourcePlayer.getCurrentSource() != nullptr; }

void MainComponent::openAudioSettings() {
    auto ptr = unique_ptr<AudioDeviceSelectorComponent>(
        new AudioDeviceSelectorComponent(deviceManager, 0, 2, 0, 2, true, false, true, false));

    ptr.get()->setSize(500, 270);

    DialogWindow::LaunchOptions dw;
    dw.useNativeTitleBar = true;
    dw.resizable = false;
    dw.dialogTitle = "Audio Settings";
    dw.dialogBackgroundColour = getLookAndFeel().findColour(DocumentWindow::backgroundColourId);

    dw.content.setOwned(ptr.get());
    ptr.release();
    dw.launchAsync();
}

void MainComponent::createWaveTables() {
    createSineTable(sineTable);
    oscillators.push_back(std::make_unique<WavetableOscillator>(sineTable));
    createImpulseTable(impulseTable);
    oscillators.push_back(std::make_unique<WavetableOscillator>(impulseTable));
    createSquareTable(squareTable);
    oscillators.push_back(std::make_unique<WavetableOscillator>(squareTable));
    createSawtoothTable(sawtoothTable);
    oscillators.push_back(std::make_unique<WavetableOscillator>(sawtoothTable));
    createTriangleTable(triangleTable);
    oscillators.push_back(std::make_unique<WavetableOscillator>(triangleTable));
}

//==============================================================================
// Noise
//==============================================================================

// White Noise

void MainComponent::whiteNoise(const AudioSourceChannelInfo& bufferToFill) {
    for (int channelNum = 0; channelNum < bufferToFill.buffer->getNumChannels(); ++channelNum) {  // Channel iteration
        auto const channelData = bufferToFill.buffer->getWritePointer(channelNum, bufferToFill.startSample);

        for (auto i = 0; i < bufferToFill.buffer->getNumSamples(); i++) {  // Sample iteration
            channelData[i] = ranSamp(level);
        }
    }
}

// Dust

void MainComponent::dust(const AudioSourceChannelInfo& bufferToFill) {
    for (int channelNum = 0; channelNum < bufferToFill.buffer->getNumChannels(); ++channelNum) {  // Channel iteration
        auto const channelData = bufferToFill.buffer->getWritePointer(channelNum, bufferToFill.startSample);

        for (auto i = 0; i < bufferToFill.buffer->getNumSamples(); i++) {  // Sample iteration
            if (random.nextInt(srate * bufferToFill.buffer->getNumChannels()) < freq - 1)
                channelData[i] = ranSamp(level);
            else
                channelData[i] = 0;
        }
    }
}

// Brown Noise

void MainComponent::brownNoise(const AudioSourceChannelInfo& bufferToFill) {
    for (int channelNum = 0; channelNum < bufferToFill.buffer->getNumChannels(); ++channelNum) {  // Channel iteration
        auto const channelData = bufferToFill.buffer->getWritePointer(channelNum, bufferToFill.startSample);

        for (auto i = 0; i < bufferToFill.buffer->getNumSamples(); i++) {  // Sample iteration

            double output = lowPass(ranSamp(level), prevOutput, alpha);
            prevOutput = output;
            channelData[i] = output * 3.0;
        }
    }
}

//==============================================================================
// Sine Wave
//==============================================================================

void MainComponent::sineWave(const AudioSourceChannelInfo& bufferToFill) {
    auto p = phasor();
    for (int channelNum = 0; channelNum < bufferToFill.buffer->getNumChannels(); ++channelNum) {  // Channel iteration
        phase = p;
        auto const channelData = bufferToFill.buffer->getWritePointer(channelNum, bufferToFill.startSample);

        for (auto i = 0; i < bufferToFill.numSamples; i++) {  // Sample iteration
            channelData[i] = std::sin(phasor() * TwoPi) * level;
        }
    }
}

//==============================================================================
// Low Frequency Waveforms
//==============================================================================

/// Impulse wave

void MainComponent::LF_impulseWave(const AudioSourceChannelInfo& bufferToFill) {
    auto p = phasor();
    for (int channelNum = 0; channelNum < bufferToFill.buffer->getNumChannels(); ++channelNum) {  // Channel iteration
        phase = p;
        double temp, prev = 0.0;
        auto const channelData = bufferToFill.buffer->getWritePointer(channelNum, bufferToFill.startSample);

        for (auto i = 0; i < bufferToFill.numSamples; i++) {  // Sample iteration
            temp = phasor();
            if (temp < prev)
                channelData[i] = level;
            else
                channelData[i] = 0;

            prev = temp;
        }
    }
}

/// Square wave

void MainComponent::LF_squareWave(const AudioSourceChannelInfo& bufferToFill) {
    auto p = phasor();
    for (int channelNum = 0; channelNum < bufferToFill.buffer->getNumChannels(); ++channelNum) {  // Channel iteration
        phase = p;
        auto const channelData = bufferToFill.buffer->getWritePointer(channelNum, bufferToFill.startSample);

        for (auto i = 0; i < bufferToFill.numSamples; i++) {  // Sample iteration
            if (phasor() < 0.5)
                channelData[i] = -1.0 * level;
            else
                channelData[i] = 1.0 * level;
        }
    }
}

/// Sawtooth wave

void MainComponent::LF_sawtoothWave(const AudioSourceChannelInfo& bufferToFill) {
    auto p = phasor();
    for (int channelNum = 0; channelNum < bufferToFill.buffer->getNumChannels(); ++channelNum) {  // Channel iteration
        phase = p;
        auto const channelData = bufferToFill.buffer->getWritePointer(channelNum, bufferToFill.startSample);

        for (auto i = 0; i < bufferToFill.numSamples; i++) {  // Sample iteration
            channelData[i] = ((phasor() * 2) - 1.0) * level;
        }
    }
}

/// Triangle wave

void MainComponent::LF_triangleWave(const AudioSourceChannelInfo& bufferToFill) {
    auto p = phasor();
    for (int channelNum = 0; channelNum < bufferToFill.buffer->getNumChannels(); ++channelNum) {  // Channel iteration
        phase = p;
        auto const channelData = bufferToFill.buffer->getWritePointer(channelNum, bufferToFill.startSample);

        for (auto i = 0; i < bufferToFill.numSamples; i++) {  // Sample iteration
            auto temp = phasor();
            if (temp < 0.5)
                channelData[i] = (temp * 4 - 1) * level;
            else
                channelData[i] = (-1 * ((temp - 0.5) * 4 - 1)) * level;
        }
    }
}

//==============================================================================
// Band Limited Waveforms
//==============================================================================

/// Impulse (pulse) wave

/// Synthesized by summing sin() over frequency and all its harmonics at equal
/// amplitude. To make it band limited only include harmonics that are at or
/// below the nyquist limit.
void MainComponent::BL_impulseWave(const AudioSourceChannelInfo& bufferToFill) {
    auto const chan0 = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    double numHarmonics = srate / 2 / freq;
    double normalizedLevel = level / numHarmonics;

    for (auto sampleIdx = 0; sampleIdx < bufferToFill.numSamples; ++sampleIdx) {
        chan0[sampleIdx] = 0;
        auto p = phasor();

        for (auto h = 1; h <= numHarmonics; ++h) {
            chan0[sampleIdx] += sin(TwoPi * p * h) * normalizedLevel;
        }
    }

    for (int channelNum = 1; channelNum < bufferToFill.buffer->getNumChannels(); ++channelNum) {
        auto const channelData = bufferToFill.buffer->getWritePointer(channelNum, bufferToFill.startSample);
        std::memcpy(channelData, chan0, bufferToFill.numSamples * sizeof(float));
    }
}

/// Square wave

/// Synthesized by summing sin() over all ODD harmonics at 1/harmonic amplitude.
/// To make it band limited only include harmonics that are at or below the
/// nyquist limit.
void MainComponent::BL_squareWave(const AudioSourceChannelInfo& bufferToFill) {
    auto const chan0 = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);

        for (auto sampleIdx = 0; sampleIdx < bufferToFill.numSamples; ++sampleIdx) {
            auto p = phasor();
            int numHarmonics = 0;
            if (freq != 0)
                numHarmonics = srate / 2 / freq;
            
            chan0[sampleIdx] = 0;

            for (auto h = 1; h <= numHarmonics; ++h) {
                if (h % 2 == 1)
                    chan0[sampleIdx] += (sin(TwoPi * p * h) / h) * level;
                else
                    chan0[sampleIdx] += 0;
            }
        }

    auto const channelData = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
    std::memcpy(channelData, chan0, bufferToFill.numSamples * sizeof(float));
}

/// Sawtooth wave
///
/// Synthesized by summing sin() over all harmonics at 1/harmonic amplitude. To make
/// it band limited only include harmonics that are at or below the nyquist limit.
void MainComponent::BL_sawtoothWave(const AudioSourceChannelInfo& bufferToFill) {
    auto const chan0 = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);

        for (auto sampleIdx = 0; sampleIdx < bufferToFill.numSamples; ++sampleIdx) {
            auto p = phasor();
            int numHarmonics = 0;
            if (freq != 0)
                numHarmonics = srate / 2 / freq;
            
            chan0[sampleIdx] = 0;

            for (auto h = 1; h <= numHarmonics; ++h)
                chan0[sampleIdx] += (sin(TwoPi * p * h) / h) * level;
        }

    auto const channelData = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
    std::memcpy(channelData, chan0, bufferToFill.numSamples * sizeof(float));
}

/// Triangle wave
///
/// Synthesized by summing sin() over all ODD harmonics at 1/harmonic**2 amplitude.
/// To make it band limited only include harmonics that are at or below the
/// Nyquist limit.
void MainComponent::BL_triangleWave(const AudioSourceChannelInfo& bufferToFill) {
    auto const chan0 = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    //        double normalizedLevel = level / numHarmonics;

            for (auto sampleIdx = 0; sampleIdx < bufferToFill.numSamples; ++sampleIdx) {
                auto p = phasor();
                int numHarmonics = 0;
                if (freq != 0)
                    numHarmonics = srate / 2 / freq;
                
                chan0[sampleIdx] = 0;

                for (auto h = 1; h <= numHarmonics; ++h) {
                    auto a = level / std::pow(h, 2);
                    if (h % 2 == 1)
                        chan0[sampleIdx] += sin(TwoPi * p * h) * a;
                    else
                        chan0[sampleIdx] += 0;
                }
            }

    //        for (int channelNum = 1; channelNum < bufferToFill.buffer->getNumChannels(); ++channelNum) {
    //            auto const channelData = bufferToFill.buffer->getWritePointer(channelNum, bufferToFill.startSample);
    //            std::memcpy(channelData, chan0, bufferToFill.numSamples * sizeof(float));
    //        }
        auto const channelData = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
        std::memcpy(channelData, chan0, bufferToFill.numSamples * sizeof(float));
}

//==============================================================================
// WaveTable Synthesis
//==============================================================================

// The audio block loop
void inline MainComponent::WT_wave(const AudioSourceChannelInfo& bufferToFill) {
    
}

// Create a sine wave table
void MainComponent::createSineTable(AudioSampleBuffer& waveTable) {
    waveTable.setSize(1, tableSize + 1);
    waveTable.clear();
    auto* samples = waveTable.getWritePointer(0);
    auto phase = 0.0;
    auto phaseDelta = MathConstants<double>::twoPi / (double)(tableSize - 1);
    for (auto i = 0; i < tableSize; ++i) {
        samples[i] += std::sin(phase);
        phase += phaseDelta;
    }

    samples[tableSize] = samples[0];
}

// Create an inpulse wave table
void MainComponent::createImpulseTable(AudioSampleBuffer& waveTable) {}

// Create a square wave table
void MainComponent::createSquareTable(AudioSampleBuffer& waveTable) {}

// Create a sawtooth wave table
void MainComponent::createSawtoothTable(AudioSampleBuffer& waveTable) {}

// Create a triagle wave table
void MainComponent::createTriangleTable(AudioSampleBuffer& waveTable) {}
