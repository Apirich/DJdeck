/*
  ==============================================================================

    customSlider.h
    Created: 28 Aug 2022 5:16:22pm
    Author:  Api Rich

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class CustomLookAndFeel : public::juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel();
    ~CustomLookAndFeel() override;

    void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height, 
                          float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, 
                          juce::Slider&) override;

    void drawButtonBackground(juce::Graphics&, juce::Button&, const juce::Colour& backgroundColour, 
                              bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

    void drawTickBox(juce::Graphics&, juce::Component& component,
                     float x, float y, float w, float h,
                     bool ticked,
                     bool isEnabled,
                     bool /*isMouseOverButton*/,
                     bool /*isButtonDown*/) override;

    void drawTableHeaderBackground(juce::Graphics&, juce::TableHeaderComponent&) override;

    juce::Label* createSliderTextBox(juce::Slider&) override;
private:
};