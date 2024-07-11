/*
  ==============================================================================

    SudokuButtonLnF.cpp
    Created: 10 Jul 2024 12:16:36pm
    Author:  godfr

  ==============================================================================
*/

#include "Sudoku.h"
#include "SudokuButton.h"
#include "SudokuButtonLnF.h"


void SudokuButtonLnF::drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool /*shouldDrawButtonAsHighlighted*/, bool /*shouldDrawButtonAsDown*/)
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

    //if (shouldDrawButtonAsHighlighted)
    //{
    //    if (shouldDrawButtonAsDown)
    //        bc = bc.brighter();
    //    else if (bc.getBrightness() > 0.5f)
    //        bc = bc.darker(0.1f);
    //    else
    //        bc = bc.brighter(0.1f);
    //}

    g.setColour(bc);
    g.fillPath(p);

    //g.setColour(bc.contrasting().withAlpha((shouldDrawButtonAsHighlighted) ? 0.6f : 0.4f));
    //g.strokePath(p, PathStrokeType((shouldDrawButtonAsHighlighted) ? 2.0f : 1.4f));
}

void SudokuButtonLnF::drawButtonText(Graphics& g, TextButton& button, bool, bool)
{
    SudokuButton* sb = (SudokuButton*) &button;

    if (sb->getCurrentValue() == 0)     //  Draw notes.
    {
        if (sb->countNotes() == 0)
            return;
        Font font((float) (sb->getHeight() / 3));
        g.setColour(Colours::black);
        int Third = sb->getHeight() / 3;
        for (int i=0 ; i<N ; i++)
            if (sb->getNote(i+1))
            {
                String txt = String(i + 1);
                int Row = i / 3;
                int Col = i % 3;
                g.drawText(txt, Col * Third, Row * Third, Third, Third, Justification::centred, false);
            }
        return;
    }
    Font font((float) button.getHeight());
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

