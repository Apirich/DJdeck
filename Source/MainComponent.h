#pragma once

#include <JuceHeader.h>

#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"
#include "LibraryControl.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent,
                       public juce::MixerAudioSource
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...

    //Left audio player & left deckGUI
    DJAudioPlayer player1{formatManager};
    DeckGUI deckGUI1{&player1, formatManager, thumbCache, &playlistComponent};

    //Right audio player & right deckGUI
    DJAudioPlayer player2{formatManager};
    DeckGUI deckGUI2{&player2, formatManager, thumbCache, &playlistComponent};

    //Mix source of both audio players
    MixerAudioSource mixerSource;

    //Library control to upload file, save library, and upload library
    LibraryControl libraryControl{&playlistComponent, formatManager};

    //Library list table to store uploaded tracks
    PlaylistComponent playlistComponent;
    
    //Audio format manager for the whole Otodecks app
    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbCache{100};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
