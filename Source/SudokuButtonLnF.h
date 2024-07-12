/*
  ==============================================================================

    SudokuButtonLnF.h
    Created: 10 Jul 2024 12:16:36pm
    Author:  godfr

  ==============================================================================
*/

#pragma once


#include <JuceHeader.h>

using namespace juce;


class SudokuButtonLnF : LookAndFeel_V2
{
    void drawButtonBackground (Graphics&, Button&, const Colour&, bool, bool)  override;
    void drawButtonText (Graphics&, TextButton&, bool, bool)  override;
};


class NumberButtonLnF : LookAndFeel_V2
{
    void drawButtonBackground (Graphics&, Button&, const Colour&, bool, bool)  override;
    void drawButtonText (Graphics&, TextButton&, bool, bool)  override;
};

