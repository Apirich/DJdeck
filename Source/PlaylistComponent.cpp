/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 20 Aug 2022 4:44:53pm
    Author:  Api Rich

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent() : selectedRow(-1)
{
    //Make the table list library visible on the GUI layout
    addAndMakeVisible(tableComponent);

    //Set the table list library as TableListBoxModel
    tableComponent.setModel(this);
    //Set LookandFeel for the table list library
    tableComponent.setLookAndFeel(&customTable);
    //Set the table list library background color (not including header)
    tableComponent.setColour(juce::TableListBox::backgroundColourId, juce::Colours::darkslategrey);
    //Set the table list library text color
    tableComponent.getHeader().setColour(juce::TableHeaderComponent::textColourId, juce::Colours::white);

    //Add columns to the table list library
    tableComponent.getHeader().addColumn("Track Title", 1, 500);
    tableComponent.getHeader().addColumn("Duration", 2, 150);
    tableComponent.getHeader().addColumn("Delete", 3, 150);
   
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
}

void PlaylistComponent::resized()
{
    //Set the size of the table list library
    tableComponent.setBounds(0, 0, getWidth(), getHeight());
}

int PlaylistComponent::getNumRows()
{
    //Set number of row as the number of tracks that are stored in the data vectors
    return trackTitles.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g, 
                                           int rowNumber, 
                                           int width, 
                                           int height, 
                                           bool rowIsSelected)
{
    if (rowIsSelected)  //Set row background color when it is selected
    {
        g.fillAll(juce::Colours::darkgrey);
    }
    else  //Set row background color when it is not selected
    {
        g.fillAll(juce::Colours::darkslategrey);
    }
}

void PlaylistComponent::paintCell(juce::Graphics& g, 
                                  int rowNumber, 
                                  int columnId, 
                                  int width, 
                                  int height, 
                                  bool rowIsSelected)
{
    //Set text color for all cells
    g.setColour(juce::Colours::white);

    //Draw titles of tracks
    if (columnId == 1)
    {
        g.drawText(trackTitles[rowNumber],
                   2, 0,
                   width - 4, height,
                   juce::Justification::centredLeft, true);
    }
    //Draw durations of tracks
    if (columnId == 2)
    { 
        g.drawText(trackDurations[rowNumber],
            2, 0,
            width - 4, height,
            juce::Justification::centredLeft, true);
    }
}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
                                                            int columnId,
                                                            bool isRowSelected,
                                                            juce::Component* existingComponentToUpdate)
{
    //Create delete button on each row
    if (columnId == 3)
    {
        if (existingComponentToUpdate == nullptr)
        {
            juce::TextButton* btn = new juce::TextButton{ "Delete" };       

            juce::String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);

            btn->addListener(this);

            existingComponentToUpdate = btn;
        }
    }

    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
    //Clear the data of a track when it is chosen to be deleted
    int id = std::stoi(button->getComponentID().toStdString());
    std::cout << "PlaylistComponent::buttonClicked " << trackTitles[id] << std::endl;
    trackFiles.erase(trackFiles.begin() + id);
    trackTitles.erase(trackTitles.begin() + id);
    trackURLs.erase(trackURLs.begin() + id);
    trackDurations.erase(trackDurations.begin() + id);
    trackDurSec.erase(trackDurSec.begin() + id);
    tableComponent.updateContent();
}

void PlaylistComponent::addNewTrack(juce::File trackFile, std::string fileName, juce::URL fileURL, double length)
{
    //Check if a track is already in the table list library
    for (int i = 0; i < trackTitles.size(); ++i)
    {
        if (fileName == trackTitles[i])
        {
            std::cout << "This track title already has been added in the list!" << std::endl;
            return;
        }
    }

    //If a track is not yet in the table list library, then add it in
    //Convert length into hr:min:sec 
    double hour = trunc(length / 3600);
    double min = trunc((length - (hour * 3600)) / 60);
    double sec = trunc(length - (hour * 3600) - (min * 60));
    //Store hr:min:sec as a string (duration)
    std::string duration = std::to_string((int)hour) + ":" + std::to_string((int)min) + ":" + std::to_string((int)sec);
    //Store data of the track into data vectors
    trackFiles.push_back(trackFile);
    trackTitles.push_back(fileName);
    trackURLs.push_back(fileURL);
    trackDurations.push_back(duration);
    trackDurSec.push_back(length);
    //Update the content of the table list library
    tableComponent.updateContent();
}


void PlaylistComponent::selectedRowsChanged(int lastRowSelected)
{
    //Store lastRowSelected in selectedRow variable
    selectedRow = lastRowSelected;
    std::cout << lastRowSelected << std::endl;
}

juce::URL PlaylistComponent::loadChosenTrackURL()
{
    //Return URL of a track (lastRowSelected) for DeckGUI to Deckin tracks
    if (selectedRow != -1)
    {
        return trackURLs[selectedRow];
    }
    else
    {
        std::cout << "PlaylistComponent::loadChosenTrack() No track has been selected!" << std::endl;
        return juce::URL();
    }
}

std::string PlaylistComponent::loadChosenTrackTitle()
{
    //Return title of a track (lastRowSelected) for DeckGUI to Deckin tracks
    if (selectedRow != -1)
    {
        return trackTitles[selectedRow];
    }
    else
    {
        std::cout << "PlaylistComponent::loadChosenTrack() No track has been selected!" << std::endl;
        return std::string();
    }
}

double PlaylistComponent::loadChosenTrackDurSec()
{
    //Return duration of a track (lastRowSelected) for DeckGUI to Deckin tracks
    if (selectedRow != -1)
    {
        return trackDurSec[selectedRow];
    }
    else
    {
        std::cout << "PlaylistComponent::loadChosenTrack() No track has been selected!" << std::endl;
        return double();
    }
}


void PlaylistComponent::saveLibrary(juce::File tempFile)
{
    //Looping through the file vectors 
    //and copy all of the files to the directory that was chosen from the filechooser from Save Library button (in LibraryControl.cpp)
    for (int i = 0; i < trackFiles.size(); ++i)
    {
        std::string trackExt = trackFiles[i].getFileExtension().toStdString();
        std::string trackOutExt = trackTitles[i] + trackExt;
        trackFiles[i].copyFileTo(tempFile.getChildFile(trackOutExt));
    }
}

void PlaylistComponent::loadLibrary()
{
    //Clear all current data on the table list library and update its content
    trackFiles.clear();
    trackTitles.clear();
    trackURLs.clear();
    trackDurations.clear();
    tableComponent.updateContent();
    std::cout << "PlaylistComponent::loadLibrary all clear" << std::endl;
}

std::vector<std::string> PlaylistComponent::searchLibrary()
{
    //Return the titles vector for the searchInput (in LibraryControl.cpp)
    return trackTitles;
}

void PlaylistComponent::chooseRow(int rowNum)
{
    //Receive the row number of the search track in searchInput (in LibraryControl.cpp)
    //and select the row 
    //(when user search for a track, if it is currently on the table list library, its row will be automatically selected)
    tableComponent.selectRow(rowNum);
}



