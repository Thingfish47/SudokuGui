/*
  ==============================================================================

    SudokuButtonLnF.cpp
    Created: 10 Jul 2024 12:16:36pm
    Author:  godfr

  ==============================================================================
*/

#include "SudokuButtonLnF.h"


void SudokuButtonLnF::drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    const int width = button.getWidth();
    const int height = button.getHeight();

    const float indent = 0.0f;
    const int cornerSize = 0;

    Path p;
    p.addRoundedRectangle(indent, indent,
        (float)width - indent * 2.0f,
        (float)height - indent * 2.0f,
        (float)cornerSize);

    Colour bc(backgroundColour.withMultipliedSaturation(0.3f));

    if (shouldDrawButtonAsHighlighted)
    {
        if (shouldDrawButtonAsDown)
            bc = bc.brighter();
        else if (bc.getBrightness() > 0.5f)
            bc = bc.darker(0.1f);
        else
            bc = bc.brighter(0.1f);
    }

    g.setColour(bc);
    g.fillPath(p);

    //g.setColour(bc.contrasting().withAlpha((shouldDrawButtonAsHighlighted) ? 0.6f : 0.4f));
    //g.strokePath(p, PathStrokeType((shouldDrawButtonAsHighlighted) ? 2.0f : 1.4f));
}

void SudokuButtonLnF::drawButtonText(Graphics& g, TextButton& button, bool, bool)
{
    Font font(button.getHeight());
    g.setFont(font);
    g.setColour(button.findColour(button.getToggleState() ? TextButton::textColourOnId
        : TextButton::textColourOffId)
        .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f));

    const int yIndent = jmin(4, button.proportionOfHeight(0.3f));
    const int cornerSize = jmin(button.getHeight(), button.getWidth()) / 2;

    const int fontHeight = roundToInt(font.getHeight() * 0.6f);
    const int leftIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
    const int rightIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
    const int textWidth = button.getWidth() - leftIndent - rightIndent;

    if (textWidth > 0)
        g.drawFittedText(button.getButtonText(),
            leftIndent, yIndent, textWidth, button.getHeight() - yIndent * 2,
            Justification::centred, 2);

}

