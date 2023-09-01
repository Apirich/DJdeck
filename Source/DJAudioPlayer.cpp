/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 18 Jul 2022 5:42:56pm
    Author:  Api Rich

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager) :formatManager(_formatManager)
{
    
}

DJAudioPlayer::~DJAudioPlayer()
{

}

//==============================================================================
void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    iirFil.prepareToPlay(samplesPerBlockExpected, sampleRate);
    samRate = sampleRate;
}

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    iirFil.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
    iirFil.releaseResources();
}

void DJAudioPlayer::loadURL(juce::URL audioURL, bool looping)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) //good file
    {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
        newSource->setLooping(looping);
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }
    else
    {
        std::cout << "DJAudioPlayer::loadURL Something went wrong loading the file " << std::endl;
    }
}

void DJAudioPlayer::setGain(double gain)
{
    //Gain has to be between 0.0 and 1.0
    if (gain < 0 || gain > 1.0)
    {
        std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1." << std::endl;
    }
    else
    {
        transportSource.setGain(gain);
    }
}

void DJAudioPlayer::setSpeed(double ratio)
{
    //Ratio has to be between 0 and 100
    if (ratio < 0 || ratio > 100.0)
    {
        std::cout << "DJAudioPlayer::setSpeed ratio should be between 0 and 100." << std::endl;
    }
    else
    {
        resampleSource.setResamplingRatio(ratio);
    }
}

void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
    //Position has to be between 0 and 1.0
    if (pos < 0 || pos > 1.0)
    {
        std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 and 1." << std::endl;
    }
    else
    {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

void DJAudioPlayer::setPass(double cutOff, double Q, bool lowPass, bool highPass, bool bandPass)
{
    //Pass filter
    if (lowPass)
    {
        iirFil.setCoefficients(iirCoef.makeLowPass(samRate, cutOff, Q));
    }
    else if (highPass)
    {
        iirFil.setCoefficients(iirCoef.makeHighPass(samRate, cutOff, Q));
    }
    else if (bandPass)
    {
        iirFil.setCoefficients(iirCoef.makeBandPass(samRate, cutOff, Q));
    }
    else
    {
        iirFil.setCoefficients(iirCoef.makeAllPass(samRate, cutOff, Q));
    }

}

void DJAudioPlayer::play()
{
    transportSource.start();
    std::cout << "Play button was clicked" << std::endl;
}

void DJAudioPlayer::stop()
{
    transportSource.stop();
    std::cout << "Stop button was clicked" << std::endl;
}

bool DJAudioPlayer::checkStreamFinished()
{
    if (transportSource.hasStreamFinished())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void DJAudioPlayer::replay()
{
    if (transportSource.hasStreamFinished())
    {
        setPosition(0);
        transportSource.start();
    }
}

double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

