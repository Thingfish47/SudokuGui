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

    //const float indent = 0.0f;
    //const int cornerSize = 0;

    Path p;
    p.addRectangle(0, 0, (float)width, (float)height);

    Colour bc(backgroundColour.withMultipliedSaturation(0.3f));

    g.setColour(bc);
    g.fillPath(p);
}

void SudokuButtonLnF::drawButtonText(Graphics& g, TextButton& button, bool, bool)
{
    SudokuButton* sb = (SudokuButton*) &button;

    if (sb->getCurrentValue() == 0 && N == 9)     //  Draw notes for regular 9x9 game.
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
    float factor = sb->isEnabled() ? 0.9f : 0.7f;
    Font font((float) button.getHeight() * factor);
    g.setFont(font);
    if (sb->isWrong() && sb->flaggingErrorsEnabled())
        g.setColour(Colours::red);
    else
    g.setColour(button.findColour(button.getToggleState() ? TextButton::textColourOnId
        : TextButton::textColourOffId)
        .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f));
    if (button.getButtonText().length() > 0)
        g.drawText(sb->getButtonText(), 0, 0, sb->getHeight(), sb->getHeight(), Justification::centred, false);
}

void NumberButtonLnF::drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool /*shouldDrawButtonAsHighlighted*/, bool /*shouldDrawButtonAsDown*/)
{
    Path p;
    p.addRectangle(0, 0, (float)button.getWidth(), (float)button.getHeight());

    Colour bc(backgroundColour.withMultipliedSaturation(0.3f));

    g.setColour(bc);
    g.fillPath(p);
}

void NumberButtonLnF::drawButtonText(Graphics& g, TextButton& button, bool, bool)
{
    SudokuButton* sb = (SudokuButton*) &button;

    Font font((float) button.getHeight() * 0.7f);
    g.setFont(font);
    g.setColour(button.findColour(button.getToggleState() ? TextButton::textColourOnId
        : TextButton::textColourOffId)
        .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f));
    g.drawText(sb->getButtonText(), 0, 0, sb->getHeight(), sb->getHeight(), Justification::centred, false);
}

