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

class Sudoku;

class SudokuButton : public TextButton
{
public:
    SudokuButton(Sudoku*, int);
    ~SudokuButton() {};

    void    setCurrentValue (int x);
    bool    getNote(int number);
    bool    isWrong();
    void    toggleNote (int x);
    void    clearNote (int x);
    void    reset();
    int     countNotes();
    void    setUnknown();
    bool    flaggingErrorsEnabled();
    bool    hilightingEnabled();
    bool    hilightingEnabledNote(int);
    int     getActualValue()    { return ActualValue;   }
    int     getCurrentValue()   { return CurrentValue;  }
    bool    isUnknown()         { return Unknown;       }
private:
    Sudoku* pParent;
    bool    Unknown;
    int     ActualValue;
    int     CurrentValue;
    bool    Notes[9];
};