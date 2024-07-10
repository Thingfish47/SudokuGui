/*
  ==============================================================================

    SudokuButton.cpp
    Created: 10 Jul 2024 12:05:51pm
    Author:  godfr

  ==============================================================================
*/

#include "SudokuButton.h"

SudokuButton::SudokuButton(int Value)
{
    ActualValue = Value;
    CurrentValue = Value;
    String txt = String(Value);
    setButtonText(txt);
    setEnabled(false);
}

void SudokuButton::setCurrentValue(int Value)
{
    CurrentValue = Value;
    if (Value == 0)
        setButtonText("");
    else
        setButtonText(String(Value));
}

void SudokuButton::setUnknown()
{
    Unknown = true;
    CurrentValue = 0;
    setButtonText("");
    setEnabled(true);
}
