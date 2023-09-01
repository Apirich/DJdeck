/*
  ==============================================================================

    customSlider.cpp
    Created: 28 Aug 2022 5:16:22pm
    Author:  Api Rich

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CustomLookAndFeel.h"

//==============================================================================
CustomLookAndFeel::CustomLookAndFeel()
{

}

CustomLookAndFeel::~CustomLookAndFeel()
{

}

void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle,
    juce::Slider& slider)
{
    auto diameter = (float)juce::jmin(width, height) - 4.0f;
    auto radius = (diameter / 2.0f) * std::cos(juce::MathConstants<float>::pi / 4.0f);
    auto centreX = (float)x + (float)width * 0.5f;
    auto centreY = (float)y + (float)height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
    bool isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();

    juce::Rectangle<float> r(rx, ry, rw, rw);
    auto transform = juce::AffineTransform::rotation(angle, r.getCentreX(), r.getCentreY());

    auto x1 = r.getTopLeft().getX();
    auto y1 = r.getTopLeft().getY();
    auto x2 = r.getBottomLeft().getX();
    auto y2 = r.getBottomLeft().getY();

    transform.transformPoints(x1, y1, x2, y2);

    juce::Path knob;
    knob.addRectangle(r);
    g.setColour(juce::Colours::black);

    g.fillPath(knob, transform);

    g.setColour(juce::Colours::white);
    g.setFont(8.0f);

    if (slider.getName() == "SPEED")
    {
        g.drawText("SPEED", r, juce::Justification::centred, true);
    }
    if (slider.getName() == "POS")
    {
        g.drawText("POS", r, juce::Justification::centred, true);
    }
    if (slider.getName() == "CUT")
    {
        g.drawText("CUTOFF", r, juce::Justification::centred, true);
    }
    if (slider.getName() == "Q")
    {
        g.drawText("Q", r, juce::Justification::centred, true);
    }
  

    juce::Path needle;
    auto r2 = r * 0.1f;
    needle.addRectangle(r2.withPosition({ r.getCentreX() - (r2.getWidth() / 2.0f), r.getY() }));

    g.setColour(juce::Colours::mediumturquoise);
    g.fillPath(needle, juce::AffineTransform::rotation(angle, r.getCentreX(), r.getCentreY()));
}

void CustomLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
    bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    if (button.getButtonText() == "SAVE LIBRARY" ||
        button.getButtonText() == "LOAD LIBRARY")
    {
        g.fillAll(juce::Colours::darkred);
        if (button.isMouseButtonDown())
        {
            g.fillAll(juce::Colours::indianred);
        }
    }
    else if (button.getButtonText() == "DECK IN" ||
             button.getButtonText() == "DECK OUT")
    {
        g.fillAll(juce::Colours::black);
        g.setColour(juce::Colours::mediumturquoise);
        g.drawRect(button.getLocalBounds(), 1);
        if (button.isMouseButtonDown())
        {
            g.fillAll(juce::Colours::darkslategrey);
        }
    }
    else
    {
        g.fillAll(juce::Colours::transparentWhite);
        if (button.isMouseButtonDown())
        {
            g.fillAll(juce::Colours::darkgrey);
        }
    }
}

void CustomLookAndFeel::drawTickBox(juce::Graphics& g, juce::Component& component,
    float x, float y, float w, float h,
    bool ticked,
    bool isEnabled,
    bool /*isMouseOverButton*/,
    bool /*isButtonDown*/)
{
    auto boxSize = w * 0.7f;

    auto isDownOrDragging = component.isEnabled() && (component.isMouseOverOrDragging() || component.isMouseButtonDown());

    auto colour = component.findColour(juce::TextButton::buttonOnColourId)
        .withMultipliedSaturation((component.hasKeyboardFocus(false) || isDownOrDragging) ? 1.3f : 0.9f)
        .withMultipliedAlpha(component.isEnabled() ? 1.0f : 0.7f);

    juce::Rectangle<float> r(x, y + (h - boxSize) * 0.5f, boxSize, boxSize);
  
    g.setColour(juce::Colours::black);
    g.fillRect(r);
    g.setColour(juce::Colours::mediumturquoise);
    g.drawRect(r);

    if (ticked)
    {
        auto tickPath = LookAndFeel_V4::getTickShape(6.0f);
        g.setColour(juce::Colours::white);

        auto transform = juce::RectanglePlacement(juce::RectanglePlacement::centred)
            .getTransformToFit(tickPath.getBounds(),
                r.reduced(r.getHeight() * 0.05f));

        g.fillPath(tickPath, transform);
    }
}

void CustomLookAndFeel::drawTableHeaderBackground(juce::Graphics& g, juce::TableHeaderComponent& header)
{
    g.fillAll(juce::Colours::darkolivegreen);
}

juce::Label* CustomLookAndFeel::createSliderTextBox(juce::Slider& slider)
{
    juce::Label* textbox = LookAndFeel_V3::createSliderTextBox(slider);
    textbox->setColour(juce::Label::backgroundColourId, juce::Colours::black);
    textbox->setColour(juce::Label::outlineColourId, juce::Colours::mediumturquoise);
    textbox->setColour(juce::Label::outlineWhenEditingColourId, juce::Colours::transparentWhite);
    textbox->setColour(juce::Label::textColourId, juce::Colours::white);
    return textbox;
}



