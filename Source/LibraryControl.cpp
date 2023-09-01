/*
  ==============================================================================

    LibraryControl.cpp
    Created: 21 Aug 2022 1:52:03pm
    Author:  Api Rich

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LibraryControl.h"

//==============================================================================
LibraryControl::LibraryControl(PlaylistComponent* _playList, 
                               juce::AudioFormatManager& _formatManager) : playList(_playList),
                                                                           formatManager(_formatManager)
{
    //Set image for the upload track button
    //and make it visible
    juce::Image importImage = juce::ImageCache::getFromMemory(BinaryData::import_png, BinaryData::import_pngSize);
    importButton.setImages(false, true, true, importImage, 1.0f, {}, importImage, 1.0f, {}, importImage, 1.0f, {});
    addAndMakeVisible(importButton);
    //Make buttons and the search field visible on the GUI layout
    addAndMakeVisible(saveButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(searchInput);   

    //Register listener to all buttons and the search field
    importButton.addListener(this);
    saveButton.addListener(this);
    loadButton.addListener(this);
    searchInput.addListener(this);

    //Make the search field editable
    searchInput.setEditable(true);
    //Set the search field prompt text
    searchInput.setText("Search Track", juce::NotificationType::dontSendNotification);

    //Set LookandFeel for buttons and the search field
    saveButton.setLookAndFeel(&customAllButton);
    loadButton.setLookAndFeel(&customAllButton);
    searchInput.setLookAndFeel(&customAllButton);
}

LibraryControl::~LibraryControl()
{
    
}

void LibraryControl::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    //Set background color of the whole LibraryControl component
    g.fillAll(juce::Colours::darkslategrey);
    //Set text color
    g.setColour (juce::Colours::white);
    //Set font size
    g.setFont (14.0f);

    //Set color for searchInput field
    searchInput.setColour(juce::Label::backgroundColourId, juce::Colours::darkred);  
}

void LibraryControl::resized()
{
    //Set the sizes of all button and the search field
    importButton.setBounds( - getHeight() / 2.5, - getHeight() / 2.5, 1.8 * getHeight(), 1.8 * getHeight());
    saveButton.setBounds(getWidth() / 12, getHeight() / 3, getWidth() / 6, getHeight() / 2);
    loadButton.setBounds(5 * getWidth() / 18, getHeight() / 3, getWidth() / 6, getHeight() / 2);
    searchInput.setBounds(17 * getWidth() / 36, getHeight() / 3, getWidth() / 2, getHeight() / 2);
}

void LibraryControl::buttonClicked(juce::Button* button)
{
    //Upload track button event
    if (button == &importButton)
    {
        //Create & open a file chooser to load a file
        juce::FileChooser chooser{ "Select a file..." };
        if (chooser.browseForFileToOpen())
        {
            //Get the URL of the chosen track
            juce::URL fileURL = chooser.getURLResult() ;
            //Check if it is in audio format
            auto* reader = formatManager.createReaderFor(fileURL.createInputStream(false));

            if (reader != nullptr)  //Good file
            {
                //Get the file of the chosen track
                juce::File trackFile = chooser.getResult();

                //Get the name of the chosen track
                std::string fileName = chooser.getResult().getFileNameWithoutExtension().toStdString();
                //Get the length (in second) of the chosen track
                double lengthInSeconds = reader->lengthInSamples / reader->sampleRate;
                //Add the chosen track to the table list library
                playList->addNewTrack(trackFile, fileName, fileURL, lengthInSeconds);
            }
        }
    }

    //Save library button event
    if (button == &saveButton)
    {
        //Create & open a file chooser to save
        juce::FileChooser fcsl{ "Save Library",
                     juce::File::getCurrentWorkingDirectory().getChildFile(""),
                     "",
                     true };

        if (fcsl.browseForFileToSave(true))
        {
            //Create a temporary file in the directory where the file chooser was last chosen
            juce::File tempFile = fcsl.getResult();
            //Create a directory for the temporary file
            tempFile.createDirectory();
            //Sent the directory to saveLibrary() (in PlaylistComponent.cpp)
            //for all the current files on the table list library to be copied to the directory
            playList->saveLibrary(tempFile);
        }
    }

    //Load library button event
    if (button == &loadButton)
    {
        //Create & open a file chooser to load a directory
        juce::FileChooser fcll{ "Load Library",
                    juce::File::getCurrentWorkingDirectory().getChildFile(""),
                    "",
                    true };

        if (fcll.browseForDirectory())
        {
            //Call loadLibrary() (in PlaylistComponent.cpp) to clear data on the current table list library
            playList->loadLibrary();

            //Store all files from the chosen directory
            juce::Array<juce::File> libDirFiles = fcll.getResult().findChildFiles(juce::File::findFiles, false);

            //Looping through all the files to get it data and add them to the table list library
            for (int i = 0; i < libDirFiles.size(); ++i)
            {
                //Get URLs of the files
                juce::URL libDirFileURL(libDirFiles.operator[](i));
                
                //Check if it is in audio format (in case the library was adjusted outside the Otodesks app)
                auto* reader = formatManager.createReaderFor(libDirFileURL.createInputStream(false));

                if (reader != nullptr)  //Good file
                {
                    juce::File trackFile = libDirFiles.operator[](i);                
                    std::string fileName = libDirFiles.operator[](i).getFileNameWithoutExtension().toStdString();
                    double lengthInSeconds = reader->lengthInSamples / reader->sampleRate;
                    playList->addNewTrack(trackFile, fileName, libDirFileURL, lengthInSeconds);
                }       
            }
        }
    }
}

void LibraryControl::labelTextChanged(juce::Label* labelThatHasChanged)
{
    //Search field event
    if (labelThatHasChanged = &searchInput)
    {     
        //Check if currently there is on the table list library
        searchTracks = playList->searchLibrary();
        if (searchTracks.empty())   //No
        {
            searchInput.setText("Library has no track!", juce::NotificationType::dontSendNotification);
        }
        else   //Yes
        {
            //Get the name of the search track
            std::string searchVal = searchInput.getText().toStdString();

            //Looping through all the tracks on the table list library
            for (int i = 0; i < searchTracks.size(); ++i)
            {
                if (searchVal == searchTracks[i])   //The search track is currently on the table list library
                {
                    searchInput.setText(searchVal, juce::NotificationType::dontSendNotification);
                    playList->chooseRow(i);
                    std::cout << "match" << searchTracks.size() << std::endl;
                    break;                          
                }
                else   //The search track is currently not on the table list library              
                {
                    searchInput.setText("No track found!", juce::NotificationType::dontSendNotification);
                    std::cout << "no match" << searchTracks.size() << std::endl;
                }
            }
        }
    }
}

