/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 18 Jul 2022 5:42:56pm
    Author:  Api Rich

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class DJAudioPlayer : public juce::AudioSource
{
    public:
        DJAudioPlayer(juce::AudioFormatManager& _formatManager);
        ~DJAudioPlayer();

        //==============================================================================
        //Virtual pure functions from AudioSource
        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
        void releaseResources() override;

        //Load URL of a chosen track
        void loadURL(juce::URL audioURL, bool looping);

        //Set gain for vol
        void setGain(double gain);
        //Set ratio for speed
        void setSpeed(double ratio);
        //Set sec for position
        void setPosition(double posInSecs);
        //Set position relative
        void setPositionRelative(double pos);

        //Set Pass
        void setPass(double cutOff, double Q, bool lowPass, bool highPass, bool bandPass);

        //Play audio
        void play();
        //Pause audio
        void stop();
        
        //Bool function to check if stream finished
        bool checkStreamFinished();
        //Replay audio
        void replay();

        //Get the relative position of the playhead
        double getPositionRelative();

    private:
        juce::AudioFormatManager& formatManager;
        std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
        juce::AudioTransportSource transportSource;
        juce::ResamplingAudioSource resampleSource{ &transportSource, false, 2 };

        //Filter for Pass
        juce::IIRCoefficients iirCoef;
        juce::IIRFilterAudioSource iirFil{&resampleSource, false};

        //Double variable to store sampleRate for Pass cases
        double samRate;
};