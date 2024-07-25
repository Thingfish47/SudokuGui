/*
  ==============================================================================

    SudokuButton.cpp
    Created: 10 Jul 2024 12:05:51pm
    Author:  godfr

  ==============================================================================
*/

#include "Sudoku.h"
#include "Cell.h"

Cell::Cell(Sudoku* p, int Value)
{
    pParent = p;
    Unknown = false;
    for (int i = 0; i < N; i++)
        Notes[i] = false;
    ActualValue = Value;
    CurrentValue = Value;
    String txt = String(Value);
    setButtonText(txt);
    setEnabled(true);
}

void Cell::resetCell(int Value)
{
    Unknown = Value == 0;
    for (int i = 0; i < N; i++)
        Notes[i] = false;
    ActualValue = Value;
    CurrentValue = Value;
    String txt = String(Value);
    setButtonText(txt);
    setEnabled(true);
}

void Cell::setCurrentValue(int Value)
{
    CurrentValue = Value;
    if (Value == 0)
        setButtonText("");
    else
        setButtonText(String(Value));
}

void Cell::setActualValue(int Value)
{
    ActualValue = Value;
    CurrentValue = 0;
    //
    for (int i = 0; i < N; i++)
        Notes[i] = false;
    Unknown = (ActualValue == 0) ? true : false;
    if (ActualValue == 0)
        setButtonText("");
    else
        setButtonText(String(ActualValue));
}

bool Cell::isKnown()
{
    if (Unknown == false && CurrentValue != 0)
        return true;
    return false;
}

bool Cell::isWrong()
{
    if (CurrentValue == 0)
        return false;
    return ActualValue != CurrentValue;
}

bool Cell::checkCorrect()
{
    if (Unknown == false)
        return true;
    if (CurrentValue == 0)
        return false;
    return ActualValue == CurrentValue;
}

bool Cell::flaggingErrorsEnabled()
{
    return pParent->flaggingErrorsEnabled();
}

bool Cell::hilightingEnabled()
{
    return pParent->hilightingEnabled();
}

bool Cell::hilightingEnabledNote(int number)
{
    return pParent->hilightingEnabledNote(number);
}

void Cell::setUnknown()
{
    Unknown = true;
    CurrentValue = 0;
    setButtonText("");
}

Pair Cell::getPair()
{
    Pair ret;

    int a = 0;
    int b = 0;
    for (int i = 0; i < N; i++)
    {
        if (Notes[i])
        {
            if (a == 0)
                a = i + 1;
            else
            {
                b = i + 1;
                break;
            }
        }
    }
    return Pair(a, b);
}

int Cell::countNotes()
{
    int count = 0;

    for (int i = 0; i < N; i++)
        if (Notes[i])
            count++;
    return count;
}

bool Cell::getNote(int number)
{
    return Notes[number - 1];
}

void Cell::toggleNote(int number)
{
    Notes[number - 1] = Notes[number - 1] ? false : true;
}

void Cell::clearNote(int number)
{
    Notes[number - 1] = false;
}

void Cell::clearAllNotes()
{
    for (int i = 0; i < N; i++)
        Notes[i] = false;
}


