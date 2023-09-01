/*
  ==============================================================================

    DeckGUI.h
    Created: 28 Jul 2022 4:04:22pm
    Author:  Api Rich

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "PlaylistComponent.h"
#include "CustomLookAndFeel.h"

//==============================================================================
/*
*/
class DeckGUI  : public juce::Component,
                 public juce::Button::Listener,
                 public juce::Slider::Listener,
                 public juce::LookAndFeel_V4,
                 public juce::ListBoxModel,
                 public juce::MultiTimer
{
public:
    DeckGUI(DJAudioPlayer* _player, 
            juce::AudioFormatManager& formatManagerToUse, 
            juce::AudioThumbnailCache& cacheToUse,
            PlaylistComponent* _playList);
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    //Virtual pure functions from Button::Listener
    void buttonClicked(juce::Button*) override;

    //Virtual pure functions from Slider::Listener
    void sliderValueChanged(juce::Slider* slider) override;

    //Virtual pure functions from MultiTimer
    void timerCallback(int timerID) override;

    //Virtual pure functions from ListBoxModel
    int getNumRows() override;
    //Override functions from ListBoxModel
    void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
    void listBoxItemClicked(int row, const juce::MouseEvent&) override;

private:
    //Images for play and rePlay buttons
    juce::Image playImage = juce::ImageCache::getFromMemory(BinaryData::play_png, BinaryData::play_pngSize);
    juce::Image isPlayingImage = juce::ImageCache::getFromMemory(BinaryData::isPlaying_png, BinaryData::isPlaying_pngSize);

    juce::Image replayImage = juce::ImageCache::getFromMemory(BinaryData::replay_png, BinaryData::replay_pngSize);
    juce::Image replayModeImage = juce::ImageCache::getFromMemory(BinaryData::replayMode_png, BinaryData::replayMode_pngSize);

    //Play, pause and autoReplay buttons
    juce::ImageButton playButton{ "PLAY" };
    juce::ImageButton pauseButton{ "PAUSE" };
    juce::ImageButton replayButton{ "REPLAY" };
    //Bool variable for auto replay
    bool autoReplay = false;

    //Vol down, mute, and vol up buttons
    juce::ImageButton muteButton{ "MUTE" };
    juce::ImageButton volUpButton{ "VOL UP" };
    juce::ImageButton volDownButton{ "VOL DOWN" };
    //Double variable for gain value
    double gainVal = 0.5;

    //Autoplay toggle button
    juce::ToggleButton autoplayBoxButton{"Autoplay"};
    //Bool variable for autoplay
    bool autoPlay = false;

    //Toggle buttons for Pass
    juce::ToggleButton lowPassBoxButton{ "Low Pass" };
    juce::ToggleButton highPassBoxButton{ "High Pass" };
    juce::ToggleButton bandPassBoxButton{ "Band Pass" };
    juce::ToggleButton allPassBoxButton{ "All Pass" };
    //Bool variables for Pass
    bool lowPass = false;
    bool highPass = false;
    bool bandPass = false;
    //Group of the Pass buttons
    enum RadioButtonIds
    {
        PassButtons = 1001
    };

    //Deckin, and deckout buttons
    juce::TextButton deckInButton{ "DECK IN" };
    juce::TextButton deckOutButton{ "DECK OUT" };

    //Cut, Q, speed and pos sliders   
    juce::Slider speedSlider{ "SPEED" };
    juce::Slider posSlider{ "POS" };
    juce::Slider cutOffSlider{ "CUT" };
    juce::Slider QSlider{ "Q" };

    //LookAndFeel (custom graphic) for sliders and buttons
    CustomLookAndFeel customSlider;

    //Vectors to store data from the table list library
    std::vector<juce::URL> queueTracksURL;
    std::vector<std::string> queueTracksTitle;
    std::vector<double> queueTracksDur;

    //Waveform display component
    WaveformDisplay waveformDisplay;

    //Audio player reference (from DJAudioPlayer.cpp)
    DJAudioPlayer* player;

    //Table list library reference (from PlaylistComponent.cpp)
    PlaylistComponent* playList;

    //Deck list box
    juce::ListBox  queueBox;

    //Bool variable for playing status
    bool playStatus = false;

    //Variables for timers
    juce::Time time;
    juce::int64 timeCounter1 = 0;
    juce::int64 timeCounter2 = 0;
    juce::int64 waitingTime = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
