/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 29 Jul 2022 6:04:23pm
    Author:  Api Rich

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                                 juce::AudioThumbnailCache& cacheToUse) : audioThumb(1000, formatManagerToUse, cacheToUse),
                                                                          fileLoaded(false),
                                                                          position(0)
{
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    //Set background color of the whole waveform component
    g.fillAll(juce::Colours::black);

    //Set color of the waveform
    g.setColour (juce::Colours::darkturquoise);

    if(fileLoaded)   //If a chosen file has been loaded
    {
        audioThumb.drawChannel(g,
                               getLocalBounds(),
                               0,
                               audioThumb.getTotalLength(),
                               0,
                               1.0f);
        g.setColour(juce::Colours::darkred);
        g.fillRect(position * getWidth(), 0, 2, getHeight());
    }
    else   //If there is no chosen file or load failed
    {
        g.setFont(20.0f);
        g.drawFittedText("File not loaded... \nPlease selected track from the library!", getLocalBounds(), juce::Justification::centred, true);   // draw some placeholder text
    }
  
}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    //Repaint if there is any change
    repaint();
}


void WaveformDisplay::loadURL(juce::URL audioURL)
{
    //Clear previous waveform
    audioThumb.clear();

    //Set new waveform for a chosen track
    fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));

    if(fileLoaded)
    {
        std::cout << "wfd: loaded!" << std::endl;
    }
    else
    {
        std::cout << "wfd: not loaded" << std::endl;
    }
}

void WaveformDisplay::setPositionRelative(double pos)
{
    //Repaint when there is change on the chosen track (new position of the playing chosen track)
    if (pos != position)
    {
        position = pos;
        repaint();
    }
}
