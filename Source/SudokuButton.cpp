/*
  ==============================================================================

    SudokuButton.cpp
    Created: 10 Jul 2024 12:05:51pm
    Author:  godfr

  ==============================================================================
*/

#include "Sudoku.h"
#include "SudokuButton.h"

SudokuButton::SudokuButton(Sudoku* p, int Value)
{
    pParent = p;
    Unknown = false;
    for (int i = 0; i < N; i++)
        Notes[i] = false;
    ActualValue = Value;
    CurrentValue = Value;
    String txt = String(Value);
    setButtonText(txt);
    setEnabled(false);
}

void SudokuButton::setCurrentValue(int Value)
{
    CurrentValue = Value;
    Unknown = false;
    if (Value == 0)
        setButtonText("");
    else
        setButtonText(String(Value));
}

bool SudokuButton::isWrong()
{
    if (CurrentValue == 0)
        return false;
    return ActualValue != CurrentValue;
}

bool SudokuButton::flaggingErrorsEnabled()
{
    return pParent->flaggingErrorsEnabled();
}
void SudokuButton::setUnknown()
{
    Unknown = true;
    CurrentValue = 0;
    setButtonText("");
    setEnabled(true);
}

int SudokuButton::countNotes()
{
    int count = 0;

    for (int i = 0; i < N; i++)
        if (Notes[i])
            count++;
    return count;
}

bool SudokuButton::getNote(int number)
{
    return Notes[number - 1];
}

void SudokuButton::toggleNote(int number)
{
    DBG("toggleNote " << number);
    Notes[number - 1] = Notes[number - 1] ? false : true;
}


void SudokuButton::clearNote(int number)
{
    DBG("SetNote " << number);
    Notes[number - 1] = false;
}


