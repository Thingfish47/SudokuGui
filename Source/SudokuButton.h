/*
  ==============================================================================

    SudokuButton.h
    Created: 10 Jul 2024 12:05:51pm
    Author:  godfr

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using namespace juce;

class SudokuButton : public TextButton
{
public:
    SudokuButton(int);
    ~SudokuButton() {};

    void    setCurrentValue (int x);
    bool    getNote(int number);
    void    toggleNote (int x);
    void    clearNote (int x);
    int     countNotes();
    void    setUnknown();
    int     getActualValue()    { return ActualValue; }
    int     getCurrentValue()   { return CurrentValue; }
    bool    isUnknown()         { return Unknown; }
private:
    bool    Unknown;
    int     ActualValue;
    int     CurrentValue;
    bool    Notes[9];
};