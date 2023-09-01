#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() 
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }

    //Make all components visible on the GUI layout
    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(libraryControl);
    addAndMakeVisible(playlistComponent);

    //Register the Audio format manager
    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    //Prepare both left and right audio player & the Mix source
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    //Add both left and right audio player into the Mix source
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}


void MainComponent::releaseResources()
{
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
    g.setColour(juce::Colours::mediumturquoise);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component
}

void MainComponent::resized()
{
    DBG("MainComponent::resized");

    //Set the sizes of all components
    deckGUI1.setBounds(0, 0, getWidth() / 2, 3 * getHeight() / 5);
    deckGUI2.setBounds(getWidth() / 2, 0, getWidth() / 2, 3 * getHeight() / 5);
    libraryControl.setBounds(0, 3 * getHeight() / 5, getWidth(), getHeight() / 10);
    playlistComponent.setBounds(0, 7 * getHeight() / 10, getWidth(), 3 * getHeight() / 5);
}




