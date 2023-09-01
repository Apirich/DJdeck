/*
  ==============================================================================

    DeckGUI.cpp
    Created: 28 Jul 2022 4:04:22pm
    Author:  Api Rich

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                 juce::AudioFormatManager& formatManagerToUse, 
                 juce::AudioThumbnailCache& cacheToUse,
                 PlaylistComponent* _playList) : player(_player),
                                     waveformDisplay(formatManagerToUse, cacheToUse),
                                     playList(_playList)
{
    //Images, set images for buttons, and make buttons visible
    //Play button
    playButton.setImages(false, true, true, playImage, 1.0f, {}, playImage, 1.0f, {}, playImage, 0.3f, {});
    addAndMakeVisible(playButton);
    //Pause button
    juce::Image pauseImage = juce::ImageCache::getFromMemory(BinaryData::pause_png, BinaryData::pause_pngSize);
    pauseButton.setImages(false, true, true, pauseImage, 1.0f, {}, pauseImage, 1.0f, {}, pauseImage, 0.3f, {});
    addAndMakeVisible(pauseButton);
    //Auto replay button
    replayButton.setImages(false, true, true, replayImage, 1.0f, {}, replayImage, 1.0f, {}, replayImage, 0.3f, {});
    addAndMakeVisible(replayButton);
    //Vol down button
    juce::Image volDownImage = juce::ImageCache::getFromMemory(BinaryData::volDown_png, BinaryData::volDown_pngSize);
    volDownButton.setImages(false, true, true, volDownImage, 1.0f, {}, volDownImage, 1.0f, {}, volDownImage, 0.3f, {});
    addAndMakeVisible(volDownButton);
    //Mute button
    juce::Image muteImage = juce::ImageCache::getFromMemory(BinaryData::volMute_png, BinaryData::volMute_pngSize);
    muteButton.setImages(false, true, true, muteImage, 1.0f, {}, muteImage, 1.0f, {}, muteImage, 0.3f, {});
    addAndMakeVisible(muteButton);
    //Vol down button
    juce::Image volUpImage = juce::ImageCache::getFromMemory(BinaryData::volUp_png, BinaryData::volUp_pngSize);
    volUpButton.setImages(false, true, true, volUpImage, 1.0f, {}, volUpImage, 1.0f, {}, volUpImage, 0.3f, {});
    addAndMakeVisible(volUpButton);

    //Make deck list box visible
    addAndMakeVisible(queueBox);   
    //Set deck list box background color
    queueBox.setColour(juce::TableListBox::backgroundColourId, juce::Colours::darkolivegreen);
    //Set ListBoxModel for deck list box
    queueBox.setModel(this);
    
    //Make buttons, and sliders visible
    //Autoplay button
    addAndMakeVisible(autoplayBoxButton);
    //LowPass, highPass, bandPass, and allPass buttons
    addAndMakeVisible(lowPassBoxButton);
    addAndMakeVisible(highPassBoxButton);
    addAndMakeVisible(bandPassBoxButton);
    addAndMakeVisible(allPassBoxButton);
    //DeckIn, and deckOut buttons
    addAndMakeVisible(deckInButton);
    addAndMakeVisible(deckOutButton); 
    //Cut off, Q, speed and pos sliders
    addAndMakeVisible(cutOffSlider);
    addAndMakeVisible(QSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    //Make waveform visible
    addAndMakeVisible(waveformDisplay);

    //Register listener to all buttons and sliders
    //Play, pause, and replay buttons
    playButton.addListener(this);
    pauseButton.addListener(this);
    replayButton.addListener(this);
    //Vol down, mute, and vol up buttons
    volDownButton.addListener(this);
    muteButton.addListener(this);
    volUpButton.addListener(this);
    //Autoplay button
    autoplayBoxButton.addListener(this);
    //LowPass, highPass, bandPass, and allPass buttons
    lowPassBoxButton.addListener(this);
    highPassBoxButton.addListener(this);
    bandPassBoxButton.addListener(this);
    allPassBoxButton.addListener(this);
    //DeckIn, and deckOut buttons
    deckInButton.addListener(this);
    deckOutButton.addListener(this);
    //Cut off, Q, speed and pos sliders
    cutOffSlider.addListener(this);
    QSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    //Set range for sliders
    //Cut off slider
    cutOffSlider.setRange(20.0, 20000.0);
    //Q slider
    QSlider.setRange(0.3, 20.0);
    //Speed slider
    speedSlider.setRange(0.0, 100.0);
    //Pos slider
    posSlider.setRange(0.0, 1.0);
    
    //Set sliders style
    //Cut off slider 
    //Vertical style
    cutOffSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    //Textbox below
    cutOffSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 15);
    //Text suffix
    cutOffSlider.setTextValueSuffix("Hz");

    //Q slider
    //Vertical style
    QSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    //Textbox below
    QSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 15);

    //Speed slider
    //Vertical style
    speedSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    //Textbox below
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 15);
    //Text suffix
    speedSlider.setTextValueSuffix("x");
    //Set initial value
    speedSlider.setValue(1, juce::NotificationType::sendNotification);

    //Pos slider
    //Vertical style
    posSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    //Textbox below
    posSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 15);
    //Text suffix
    posSlider.setTextValueSuffix("s");

    //Set LookandFeel for buttons and sliders
    //DeckIn, deckOut, and autoPLay buttons
    deckInButton.setLookAndFeel(&customSlider);
    deckOutButton.setLookAndFeel(&customSlider);
    autoplayBoxButton.setLookAndFeel(&customSlider);
    //LowPass, highPass, bandPass, and allPass buttons
    lowPassBoxButton.setLookAndFeel(&customSlider);
    highPassBoxButton.setLookAndFeel(&customSlider);
    bandPassBoxButton.setLookAndFeel(&customSlider);
    allPassBoxButton.setLookAndFeel(&customSlider);
    //Cut off, Q, speed, and pos sliders
    cutOffSlider.setLookAndFeel(&customSlider);
    QSlider.setLookAndFeel(&customSlider);
    speedSlider.setLookAndFeel(&customSlider);
    posSlider.setLookAndFeel(&customSlider);
    
    //Set all pass buttons to the same group
    //(only one of the buttons can be tick at the time)
    lowPassBoxButton.setRadioGroupId(PassButtons);
    highPassBoxButton.setRadioGroupId(PassButtons);
    bandPassBoxButton.setRadioGroupId(PassButtons);
    allPassBoxButton.setRadioGroupId(PassButtons);

    //If there is no track in the deck list box, disable all buttons and sliders (except deckIn, and deckOut buttons)
    if (queueTracksTitle.size() == 0)
    {
        speedSlider.setEnabled(false);
        posSlider.setEnabled(false);
        cutOffSlider.setEnabled(false);
        QSlider.setEnabled(false);

        playButton.setEnabled(false);
        pauseButton.setEnabled(false);
        replayButton.setEnabled(false);
        volDownButton.setEnabled(false);
        muteButton.setEnabled(false);
        volUpButton.setEnabled(false);

        queueBox.setEnabled(false);
        autoplayBoxButton.setEnabled(false);

        lowPassBoxButton.setEnabled(false);
        highPassBoxButton.setEnabled(false);
        bandPassBoxButton.setEnabled(false);
        allPassBoxButton.setEnabled(false);
    }

    //Start timer1 for waveform display
    startTimer(1, 500);
}

DeckGUI::~DeckGUI()
{
    //Stop timer1
    stopTimer(1);
}

void DeckGUI::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
    //Set background color for the whole deckGUI
    g.fillAll(juce::Colours::darkslategrey);
}

void DeckGUI::resized()
{
    //Set the sizes of waveform, all buttons and sliders
    waveformDisplay.setBounds(0, 0, 400, 120);

    cutOffSlider.setBounds(30, 125, 70, 70);
    QSlider.setBounds(110, 125, 70, 70);
    speedSlider.setBounds(30, 200, 70, 70);
    posSlider.setBounds(110, 200, 70, 70);

    playButton.setBounds(15, 265, 65, 65);
    pauseButton.setBounds(75, 265, 65, 65);
    replayButton.setBounds(135, 265, 65, 65);

    volDownButton.setBounds(15, 305, 65, 65);
    muteButton.setBounds(75, 305, 65, 65);
    volUpButton.setBounds(135, 305, 65, 65);

    queueBox.setBounds(195, 130, 180, 115);

    autoplayBoxButton.setBounds(200, 245, 70, 20);
    allPassBoxButton.setBounds(300, 245, 70, 20);

    lowPassBoxButton.setBounds(200, 270, 60, 50);
    highPassBoxButton.setBounds(262, 270, 60, 50);
    bandPassBoxButton.setBounds(322, 270, 60, 50);

    deckInButton.setBounds(215, 330, 55, 20);
    deckOutButton.setBounds(300, 330, 55, 20);





    /*waveformDisplay.setBounds(0, 0, getWidth(), getHeight() / 3);

    cutOffSlider.setBounds(getWidth() / 11, getHeight() / 3, getWidth() / 6, getWidth() / 6);
    QSlider.setBounds(17 * getWidth() / 66, getHeight() / 3, getWidth() / 6, getWidth() / 6);
    speedSlider.setBounds(getWidth() / 11, 6 * getHeight() / 11, getWidth() / 6, getWidth() / 6);
    posSlider.setBounds(17 * getWidth() / 66, 6 * getHeight() / 11, getWidth() / 6, getWidth() / 6);

    playButton.setBounds(getWidth() / 30, 5 * getHeight() / 7, getWidth() / 6, getWidth() / 6);
    pauseButton.setBounds(3 * getWidth() / 17, 5 * getHeight() / 7, getWidth() / 6, getWidth() / 6);
    replayButton.setBounds(163 * getWidth() / 510, 5 * getHeight() / 7, getWidth() / 6, getWidth() / 6);

    volDownButton.setBounds(getWidth() / 30, 5 * getHeight() / 6, getWidth() / 6, getWidth() /6);
    muteButton.setBounds(3 * getWidth() / 17, 5 * getHeight() / 6, getWidth() / 6, getWidth() / 6);
    volUpButton.setBounds(163 * getWidth() / 510, 5 * getHeight() / 6, getWidth() / 6, getWidth() / 6);

    queueBox.setBounds(8 * getWidth() / 15, 4 * getHeight() / 11, 2 * getWidth() / 5, getHeight() / 3);

    autoplayBoxButton.setBounds(5 * getWidth() / 8, 19 * getHeight() / 30, getWidth() / 6, getWidth() / 6);

    lowPassBoxButton.setBounds(4 * getWidth() / 8, 3 * getHeight() / 4, getWidth() / 6, getWidth() / 6);
    highPassBoxButton.setBounds(5 * getWidth() / 8, 3 * getHeight() / 4, getWidth() / 6, getWidth() / 6);
    bandPassBoxButton.setBounds(6 * getWidth() / 8, 3 * getHeight() / 4, getWidth() / 6, getWidth() / 6);

    deckInButton.setBounds(9 * getWidth() / 16, 9 * getHeight() / 10, getWidth() / 7, getHeight() / 16);
    deckOutButton.setBounds(3 * getWidth() / 4, 9 * getHeight() / 10, getWidth() / 7, getHeight() / 16);*/
}

void DeckGUI::buttonClicked(juce::Button* button)
{
    //Play button event
    if (button == &playButton)
    {
        //If playing, disable deckIn and deckOut buttons
        deckInButton.setEnabled(false);
        deckOutButton.setEnabled(false);
        
        //If playing, set play status to be true
        playStatus = true;
        //If playing, set play button with isPlaying image
        playButton.setImages(false, true, true, isPlayingImage, 1.0f, {}, isPlayingImage, 1.0f, {}, isPlayingImage, 0.3f, {});
                    
        if ((time.getMillisecondCounter() - waitingTime) > 10000)
        {
            timeCounter2 = 0;
            player->loadURL(juce::URL{ queueTracksURL[queueBox.getLastRowSelected()] }, autoReplay);
            waveformDisplay.loadURL(juce::URL{ queueTracksURL[queueBox.getLastRowSelected()] });
        }

        if (!autoReplay)    //If not in autoReplay mode
        {
            player->play();

            //Start timer2 so if a track is still playing
            //if another track in the deck list box is chosen, it will be played without cliking play button (feature)

            if (speedSlider.getValue() != 1 && posSlider.getValue() == 0)   //If speedSlider already has been adjusted, but not posSlider
            {
                startTimer(2, (queueTracksDur[queueBox.getLastRowSelected()] * 1000 - timeCounter2) / speedSlider.getValue());  
                autoplayBoxButton.setEnabled(false);
            }
            else if (speedSlider.getValue() == 1 && posSlider.getValue() != 0)   //If posSlider already has been adjusted, but not speedSlider
            {
                startTimer(2, (queueTracksDur[queueBox.getLastRowSelected()] - posSlider.getValue()) * 1000);
                autoplayBoxButton.setEnabled(false);
            }
            else if (speedSlider.getValue() != 1 && posSlider.getValue() != 0)   //If both speedSlider, and posSlider already have been adjusted
            {
                startTimer(2, ((queueTracksDur[queueBox.getLastRowSelected()] - posSlider.getValue()) * 1000 - timeCounter2) / speedSlider.getValue());
                autoplayBoxButton.setEnabled(false);
            }
            else   //Neither speedSlider nor posSlider have been adjusted yet
            {     
                startTimer(2, (queueTracksDur[queueBox.getLastRowSelected()] * 1000) - timeCounter2);
            }
   
            timeCounter1 = time.getMillisecondCounter();
        }
        else   //If in autoReplay mode
        {
            if (!player->checkStreamFinished())   //If the chosen track has never been played 
            {
                player->play();
            }
            else   //If the chosen track already has been played, replay it
            {
                player->replay();
            }
        }     
    }

    //Pause button event
    if (button == &pauseButton)
    {
        //If pause, enable deckIn and deckOut buttons
        deckInButton.setEnabled(true);
        deckOutButton.setEnabled(true);

        //If pause, set play status to be false
        playStatus = false;
        //If pause, set play button with play image
        playButton.setImages(false, true, true, playImage, 1.0f, {}, playImage, 1.0f, {}, playImage, 0.3f, {});

        player->stop();      

        if (!autoReplay)    //If not in autoReplay mode
        {
            //Stop timer2, and set timeCounter2 to the length that the track just has been played
            //and set timeCounter2 so duration will be deducted when play button is clicked again
            //store waitingTime
            stopTimer(2);
            timeCounter2 = time.getMillisecondCounter() - timeCounter1;
            waitingTime = time.getMillisecondCounter();
        }
        else   //If in autoReplay mode
        {
            //If timer3 is running, stop it
            if (isTimerRunning(3))
            {
                stopTimer(3);
            }
        }
    }

    //Auto replay button event
    if (button == &replayButton)
    {
        autoReplay = !autoReplay;

        if (autoReplay)   //If in autoReplay mode
        {
            if (playStatus)   //If playing
            {
                if (!player->checkStreamFinished())   //If the chosen track has never been played
                {
                    //Stop timer2, and start timer3 so when the current stream is finished,
                    //the chosen track will be in looping mode
                    stopTimer(2);
                    timeCounter2 = time.getMillisecondCounter() - timeCounter1;
                    startTimer(3, queueTracksDur[queueBox.getLastRowSelected()] * 1000 - timeCounter2);
                }
                else   //If the chosen track already has been played, and just finished stream
                {
                    //Stop firstTimer, 
                    //reload it just as a new chosen track (autoReplay variable set to be TRUE for looping) 
                    stopTimer(2);
                    player->loadURL(juce::URL{ queueTracksURL[queueBox.getLastRowSelected()] }, autoReplay);
                    waveformDisplay.loadURL(juce::URL{ queueTracksURL[queueBox.getLastRowSelected()] });
                    player->play();
                }                
            }
            else   //If not playing yet, load it just as a new chosen track (autoReplay variable set to be TRUE for looping) 
            {
                player->loadURL(juce::URL{ queueTracksURL[queueBox.getLastRowSelected()] }, autoReplay);
                waveformDisplay.loadURL(juce::URL{ queueTracksURL[queueBox.getLastRowSelected()] });
            }

            //Set autoReplayMode image to auto replay button (to indicate if it is in autoReplay mode)
            replayButton.setImages(false, true, true, replayModeImage, 1.0f, {}, replayModeImage, 1.0f, {}, replayModeImage, 0.3f, {});                       
            
            //Disable autoPlay mode
            autoplayBoxButton.setEnabled(false);
        }
        else   //If not in autoReplay mode
        {
            if (playStatus)   //If playing
            {
                if (isTimerRunning(3))   //Stop timer3 if it is running
                {
                    stopTimer(3);
                }    

                //Set play status to false and pause the track
                playStatus = false;
                player->stop();    
                //Set play button with play image
                playButton.setImages(false, true, true, playImage, 1.0f, {}, playImage, 1.0f, {}, playImage, 0.3f, {});

                //Enable deckIn, and deckOut buttons
                deckInButton.setEnabled(true);
                deckOutButton.setEnabled(true);
            }
            
            //Reload it just as a new chosen track (autoReplay variable set to be FALSE for looping) 
            player->loadURL(juce::URL{ queueTracksURL[queueBox.getLastRowSelected()] }, autoReplay);
            waveformDisplay.loadURL(juce::URL{ queueTracksURL[queueBox.getLastRowSelected()] });

            //Set notautoReplayMode image to auto replay button (to indicate if it is not in autoReplay mode)
            replayButton.setImages(false, true, true, replayImage, 1.0f, {}, replayImage, 1.0f, {}, replayImage, 0.3f, {});
            
            //Enable autoPlay mode
            autoplayBoxButton.setEnabled(true);
        }
    }

    //Vol down button event
    if (button == &volDownButton)
    {
        if (gainVal > 0.2)   //If gain is greater than or equal to 0.2, low it down 0.1
        {
            gainVal -= 0.1;
            player->setGain(gainVal);
        }
    }
    //Mute button event
    if (button == &muteButton)
    {
        //Set gain to 0
        gainVal = 0;
        player->setGain(gainVal);
    }
    //Vol up button event
    if (button == &volUpButton)
    {
        if (gainVal < 0.9)   //If gain is less than or equal to 0.9, raise it up 0.1
        {
            gainVal += 0.1;
            player->setGain(gainVal);
        }
    }

    //AutoPlay button event
    if (button == &autoplayBoxButton)
    {
        autoPlay = !autoPlay;

        if (!autoPlay)   //If not autoPlay
        {
            //Enable speed, pos slider, and auto replay button
            speedSlider.setEnabled(true);
            posSlider.setEnabled(true);
            replayButton.setEnabled(true);
        }
        else   //If autoPlay
        {
            //Disable speed, pos slider, and auto replay button
            speedSlider.setEnabled(false);
            posSlider.setEnabled(false);
            replayButton.setEnabled(false);
        }
        
    }

    //LowPass button event
    if (button == &lowPassBoxButton)
    {
        lowPass = true;
        highPass = false;
        bandPass = false;

        player->setPass(cutOffSlider.getValue(), QSlider.getValue(), lowPass, highPass, bandPass); 
    }
    //HighPass button event
    if (button == &highPassBoxButton)
    {
        lowPass = false;
        highPass = true;
        bandPass = false;
 
        player->setPass(cutOffSlider.getValue(), QSlider.getValue(), lowPass, highPass, bandPass);
    }
    //BandPass button event
    if (button == &bandPassBoxButton)
    {
        lowPass = false;
        highPass = false;
        bandPass = true;
  
        player->setPass(cutOffSlider.getValue(), QSlider.getValue(), lowPass, highPass, bandPass);
    }
    //AllPass button event
    if (button == &allPassBoxButton)
    {
        lowPass = false;
        highPass = false;
        bandPass = false;
        player->setPass(cutOffSlider.getValue(), QSlider.getValue(), lowPass, highPass, bandPass);
    }

    //DeckIn button event
    if (button == &deckInButton)
    {
        if (!playList->loadChosenTrackTitle().empty())   //If the table list library is not empty
        {
            //Store the data of the chosen track in the data vectors
            queueTracksTitle.push_back(playList->loadChosenTrackTitle());
            queueTracksURL.push_back(playList->loadChosenTrackURL());
            queueTracksDur.push_back(playList->loadChosenTrackDurSec());
          
            //Load it just as a new chosen track 
            player->loadURL(juce::URL{ playList->loadChosenTrackURL() }, autoReplay);
            waveformDisplay.loadURL(juce::URL{ playList->loadChosenTrackURL() });

            //Update the deck list box content
            queueBox.updateContent();
            //Select the last row of the deck list box (the last chosen track from the table list library)
            queueBox.selectRow(queueTracksTitle.size() - 1);
        }   

        if (queueTracksTitle.size() != 0)   //If there is any tracks in the deck list box
        {
            speedSlider.setEnabled(true);
            posSlider.setEnabled(true);
            cutOffSlider.setEnabled(true);
            QSlider.setEnabled(true);

            playButton.setEnabled(true);
            pauseButton.setEnabled(true);
            replayButton.setEnabled(true);
            if (autoPlay)   //If in autoPlay mode
            {
                replayButton.setEnabled(false);
                speedSlider.setEnabled(false);
                posSlider.setEnabled(false);
            }            
            volDownButton.setEnabled(true);
            muteButton.setEnabled(true);
            volUpButton.setEnabled(true);

            queueBox.setEnabled(true);
            autoplayBoxButton.setEnabled(true);
            if (autoReplay)   //If in autoReplay mode
            {
                autoplayBoxButton.setEnabled(false);
            }
            

            lowPassBoxButton.setEnabled(true);
            highPassBoxButton.setEnabled(true);
            bandPassBoxButton.setEnabled(true);
            allPassBoxButton.setEnabled(true);
        }
    }
    //DeckOut button event
    if (button == &deckOutButton)
    {
        if (queueTracksTitle.size() > 1)   //If the deck list box has more than one track
        {
            queueTracksTitle.erase(queueTracksTitle.begin() + queueBox.getLastRowSelected());
            queueTracksURL.erase(queueTracksURL.begin() + queueBox.getLastRowSelected());
            queueTracksDur.erase(queueTracksDur.begin() + queueBox.getLastRowSelected());

            queueBox.updateContent();
            queueBox.selectRow(queueTracksTitle.size() - 1);

            player->loadURL(juce::URL{ queueTracksURL[queueBox.getLastRowSelected()] }, autoReplay);
            waveformDisplay.loadURL(juce::URL{ queueTracksURL[queueBox.getLastRowSelected()] });
        }
        else if (queueTracksTitle.size() == 1)   //If the deck list box has only one track
        {
            queueTracksTitle.erase(queueTracksTitle.begin() + queueBox.getLastRowSelected());
            queueTracksURL.erase(queueTracksURL.begin() + queueBox.getLastRowSelected());
            queueTracksDur.erase(queueTracksDur.begin() + queueBox.getLastRowSelected());

            queueBox.updateContent();

            player->loadURL(juce::URL{ }, autoReplay);
            waveformDisplay.loadURL(juce::URL{ });

            speedSlider.setEnabled(false);
            posSlider.setEnabled(false);
            cutOffSlider.setEnabled(false);
            QSlider.setEnabled(false);

            playButton.setEnabled(false);
            pauseButton.setEnabled(false);
            replayButton.setEnabled(false);
            volDownButton.setEnabled(false);
            muteButton.setEnabled(false);
            volUpButton.setEnabled(false);

            queueBox.setEnabled(false);
            autoplayBoxButton.setEnabled(false);

            lowPassBoxButton.setEnabled(false);
            highPassBoxButton.setEnabled(false);
            bandPassBoxButton.setEnabled(false);
            allPassBoxButton.setEnabled(false);
        }
    }
}

void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
    //Cut off slider event
    if (slider == &cutOffSlider)
    {
        player->setPass(slider->getValue(), QSlider.getValue(), lowPass, highPass, bandPass);
    }
    //Q slider event
    if (slider == &QSlider)
    {
        player->setPass(cutOffSlider.getValue(), slider->getValue(), lowPass, highPass, bandPass);
    }

    //Speed slider event
    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());

        if (slider->getValue() != 1 && playStatus)  //If speed slider has been adjusted, and is playing (disable autoPlay mode)
        {
            if (autoplayBoxButton.isEnabled())
            {
                autoplayBoxButton.setEnabled(false);
            }
            if (!replayButton.isEnabled())
            {
                replayButton.setEnabled(true);
            }

            deckInButton.setEnabled(false);
            deckOutButton.setEnabled(false);

            timeCounter2 = time.getMillisecondCounter() - timeCounter1;      

            if (posSlider.getValue() == 0)
            {                
                startTimer(2, 
                           (queueTracksDur[queueBox.getLastRowSelected()] * 1000 - timeCounter2) / slider->getValue());     
                timeCounter1 = time.getMillisecondCounter();
            }
            else
            {
                startTimer(2, 
                           ((queueTracksDur[queueBox.getLastRowSelected()] - posSlider.getValue()) * 1000 - timeCounter2) / slider->getValue());
                timeCounter1 = time.getMillisecondCounter();
            }
        }

        if (slider->getValue() == 1 
            && posSlider.getValue() == 0 
            && !playStatus)   //If both speed slider and pos slider have never been adjusted, and is not playing (enable autoPlay mode)
        {
            if (queueTracksTitle.size() != 0)
            {
                if (!replayButton.isEnabled())
                {
                    autoplayBoxButton.setEnabled(true);
                }
            }         
           
            deckInButton.setEnabled(true);
            deckOutButton.setEnabled(true);
        }
    }
    //Pos slider event
    if (slider == &posSlider)   
    {               
        player->setPositionRelative(slider->getValue());

        if (slider->getValue() != 0 && playStatus)   //If pos slider has been adjusted, and is playing (disable autoPlay mode)
        {
            if (autoplayBoxButton.isEnabled())
            {
                autoplayBoxButton.setEnabled(false);
            }
            if (!replayButton.isEnabled())
            {
                replayButton.setEnabled(true);
            }

            autoplayBoxButton.setEnabled(false);      
            deckInButton.setEnabled(false);
            deckOutButton.setEnabled(false);

            if (speedSlider.getValue() == 0)
            {                           
                startTimer(2, (queueTracksDur[queueBox.getLastRowSelected()] - slider->getValue()) * 1000);  
                timeCounter1 = time.getMillisecondCounter();
            }
            else
            {
                timeCounter2 = time.getMillisecondCounter() - timeCounter1;               
                startTimer(2, 
                           (queueTracksDur[queueBox.getLastRowSelected()] * 1000 - timeCounter2) / speedSlider.getValue() - (slider->getValue() * 1000));
                timeCounter1 = time.getMillisecondCounter();
            }
        }

        if (slider->getValue() == 0 
            && speedSlider.getValue() == 1
            && !playStatus)   //If both speed slider and pos slider have never been adjusted, and is not playing (enable autoPlay mode)
        {
            if (queueTracksTitle.size() != 0)
            {
                if (!replayButton.isEnabled())
                {
                    autoplayBoxButton.setEnabled(true);
                }
            }
        
            deckInButton.setEnabled(true);
            deckOutButton.setEnabled(true);
        } 
    }
   
}

int DeckGUI::getNumRows()
{
    //Set number of row as the number of tracks that are stored in the data vectors
    return queueTracksTitle.size();
}

void DeckGUI::paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)   //Set row background color when it is selected
    {
        g.fillAll(juce::Colours::darkgrey);
    }

    //Set text color for all cells
    g.setColour(juce::Colours::white);
 
    // //Draw titles of tracks
    g.drawText(queueTracksTitle[rowNumber],
        2, 0,
        width - 4, height,
        juce::Justification::centredLeft, true);
}

void DeckGUI::listBoxItemClicked(int row, const juce::MouseEvent&)
{
    //If a row of the deck list box is clicked
    player->loadURL(juce::URL{ queueTracksURL[row] }, autoReplay);
    waveformDisplay.loadURL(juce::URL{ queueTracksURL[row]});

    speedSlider.setValue(1, juce::NotificationType::sendNotification);
    posSlider.setValue(0, juce::NotificationType::sendNotification);

    if (!replayButton.isEnabled())   //If not in auto replay mode
    {
        autoplayBoxButton.setEnabled(true);
    }
  
    if (!autoplayBoxButton.isEnabled())   //If not in autoPLay mode
    {
        replayButton.setEnabled(true);
    }
  
    if (playStatus)   //If playing
    {
        player->play();

        timeCounter2 = 0;       
        startTimer(2, (queueTracksDur[queueBox.getLastRowSelected()] * 1000) - timeCounter2);
        timeCounter1 = time.getMillisecondCounter();   
    }
}

void DeckGUI::timerCallback(int timerID)
{
    //Callback for timer1
    if (timerID == 1)
    {
        waveformDisplay.setPositionRelative(player->getPositionRelative());       
    }

    //Callback for timer2
    if (timerID == 2)
    {
        if (autoPlay)   //If in autoPlay mode
        {
            if (queueBox.getLastRowSelected() == (queueTracksTitle.size() - 1))   //If the chosen track is the last track on the deck list box
            {
                stopTimer(2);

                deckInButton.setEnabled(true);
                deckOutButton.setEnabled(true);

                playStatus = false;
                playButton.setImages(false, true, true, playImage, 1.0f, {}, playImage, 1.0f, {}, playImage, 0.3f, {});
                
                player->loadURL(juce::URL{ queueTracksURL[queueBox.getLastRowSelected()] }, false);
                waveformDisplay.loadURL(juce::URL{ queueTracksURL[queueBox.getLastRowSelected()] });  
            }
            else   //If the chosen track is other than the last track
            {
                queueBox.selectRow(queueBox.getLastRowSelected() + 1);

                player->loadURL(juce::URL{ queueTracksURL[queueBox.getLastRowSelected()] }, false);
                waveformDisplay.loadURL(juce::URL{ queueTracksURL[queueBox.getLastRowSelected()] });

                player->play();
                
                startTimer(2, queueTracksDur[queueBox.getLastRowSelected()] * 1000);
                timeCounter1 = time.getMillisecondCounter();
            }
        }  
        else   //If not in autoPlay mode
        {
            stopTimer(2);

            if (!autoReplay)   //If not in autoReplay mode
            {
                playStatus = false;
                playButton.setImages(false, true, true, playImage, 1.0f, {}, playImage, 1.0f, {}, playImage, 0.3f, {});

                deckInButton.setEnabled(true);
                deckOutButton.setEnabled(true);

                player->loadURL(juce::URL{ queueTracksURL[queueBox.getLastRowSelected()]}, autoReplay);
                waveformDisplay.loadURL(juce::URL{ queueTracksURL[queueBox.getLastRowSelected()]});

                speedSlider.setValue(1, juce::NotificationType::sendNotification);
                posSlider.setValue(0, juce::NotificationType::sendNotification);
                if (!autoplayBoxButton.isEnabled())
                {
                    autoplayBoxButton.setEnabled(true);
                }              
            }   
        }
    }

    //Callback for timer3
    if (timerID == 3)
    {
        stopTimer(3);

        player->loadURL(juce::URL{ queueTracksURL[queueBox.getLastRowSelected()] }, autoReplay);
        waveformDisplay.loadURL(juce::URL{ queueTracksURL[queueBox.getLastRowSelected()] });
        player->play();     
    } 
}




