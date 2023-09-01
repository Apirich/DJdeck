/*
  ==============================================================================

    WaveformDisplay.h
    Created: 29 Jul 2022 6:04:23pm
    Author:  Api Rich

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformDisplay  : public juce::Component,
                         public juce::ChangeListener
{
public:
    WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                    juce::AudioThumbnailCache& cacheToUse);
    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    //Virtual pure functions from ChangeListener
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    //Receive URL of a chosen track and set it as a source for waveformDisplay
    void loadURL(juce::URL audioURL);

    //Set the relative position of the playhead
    void setPositionRelative(double pos);

private:
    //Waveform variable
    juce::AudioThumbnail audioThumb;

    //Bool to check if a chosen file has been loaded yet
    bool fileLoaded;

    //Bool for new position of the playhead of the waveform
    double position;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
