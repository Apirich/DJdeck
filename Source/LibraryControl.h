/*
  ==============================================================================

    LibraryControl.h
    Created: 21 Aug 2022 1:52:03pm
    Author:  Api Rich

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <string>
#include "PlaylistComponent.h"

#include "CustomLookAndFeel.h"

//==============================================================================
/*
*/
class LibraryControl  : public juce::Component,
                        public juce::Button::Listener,
                        public juce::Label::Listener
{
public:
    LibraryControl(PlaylistComponent* _playList, juce::AudioFormatManager& _formatManager);
    ~LibraryControl() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    //Virtual pure functions from Button::Listener
    void buttonClicked(juce::Button*) override;

    //Virtual pure functions from Label::Listener
    void labelTextChanged(juce::Label* labelThatHasChanged) override;


private:
    //Upload track button
    juce::ImageButton importButton{ "IMPORT" };
    //Save Library button
    juce::TextButton saveButton{ "SAVE LIBRARY" };
    //Load Library button
    juce::TextButton loadButton{ "LOAD LIBRARY" };

    //Table list library reference (from PlaylistComponent.cpp)
    PlaylistComponent* playList;

    //Audio format manager reference (from MainComponent.cpp)
    juce::AudioFormatManager& formatManager;

    //Search field
    juce::Label searchInput;

    //Store current tracks that are on the table list library (from PlaylistComponent.cpp)
    std::vector<std::string> searchTracks;

    //LookAndFeel (custom graphic) for buttons and the search field
    CustomLookAndFeel customAllButton;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LibraryControl)
};
