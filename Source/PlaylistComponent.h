/*
  ==============================================================================

    PlaylistComponent.h
    Created: 20 Aug 2022 4:44:53pm
    Author:  Api Rich

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "CustomLookAndFeel.h"


//==============================================================================
/*
*/
class PlaylistComponent  : public juce::TableListBox,
                           public juce::TableListBoxModel,
                           public juce::Button::Listener
{
public:
    PlaylistComponent();
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    //Virtual pure functions from TableListBoxModel
    int getNumRows() override;
    void paintRowBackground(juce::Graphics&, 
                            int rowNumber, 
                            int width, 
                            int height, 
                            bool rowIsSelected) override;
    void paintCell(juce::Graphics&, 
                   int rowNumber, 
                   int columnId, 
                   int width, 
                   int height, 
                   bool rowIsSelected) override;
    //Override function from TableListBoxModel
    juce::Component* refreshComponentForCell(int rowNumber, 
                                       int columnId, 
                                       bool isRowSelected, 
                                       Component* existingComponentToUpdate) override;

    //Virtual pure functions from Button::Listener
    void buttonClicked(juce::Button* button) override;

    //Receive a file from the browsers (import button and loadLibrary button in LibraryControl)
    //and store its data into the vectors, and then add them on the table list library
    void addNewTrack(juce::File, std::string fileName, juce::URL fileURL, double length);
  
    //Override function from TableListBox
    void selectedRowsChanged(int lastRowSelected) override;

    //Return data for DeckGUI to Deckin tracks
    juce::URL loadChosenTrackURL();
    std::string loadChosenTrackTitle();
    double loadChosenTrackDurSec();

    //Retrieve current data on the table list library to save library 
    void saveLibrary(juce::File tempFile);
    //Clear all current data on the table list library to load library with its data
    void loadLibrary();

    //Search to see if a track is currently store on the table list library
    std::vector<std::string> searchLibrary();
    //If the search track is currently store on the table list library, select its row
    void chooseRow(int rowNum);

private:
    //The table list library
    juce::TableListBox tableComponent;
    //LookAndFeel (custom graphic) for the table list library
    CustomLookAndFeel customTable;

    //Vectors to store data from received files
    std::vector<juce::File> trackFiles;
    std::vector<std::string> trackTitles;
    std::vector<juce::URL> trackURLs;
    std::vector<std::string> trackDurations;
    std::vector<double> trackDurSec;

    //Store the last row selected from the table list library (to Deckin its data)
    double selectedRow;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
