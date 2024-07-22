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
class Pair;

class Cell : public TextButton
{
public:
    Cell(Sudoku*, int);
    ~Cell() {};

    void    setCurrentValue (int x);
    void    setActualValue (int x);
    bool    getNote(int number);
    bool    checkCorrect();
    bool    isWrong();
    void    toggleNote (int x);
    void    clearNote (int x);
    void    resetCell(int);
    int     countNotes();
    Pair    getPair();
    void    setUnknown();
    void    clearAllNotes();
    bool    flaggingErrorsEnabled();
    bool    hilightingEnabled();
    bool    hilightingEnabledNote(int);
    int     getActualValue()    { return ActualValue;   }
    int     getCurrentValue()   { return CurrentValue;  }
    bool    isUnknown()         { return Unknown;       }
    bool    hasPair()           { return countNotes() == 2; }
private:
    Sudoku* pParent;
    bool    Unknown;
    int     ActualValue;
    int     CurrentValue;
    bool    Notes[9];
};