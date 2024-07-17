/*
  ==============================================================================

    Sudoku.h
    Created: 9 Jul 2024 11:59:22am
    Author:  godfrey.small

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "IniFile.h"
#include "grid.h"
#include "SudokuButton.h"
#include "SudokuButtonLnF.h"
#include "Pairs.h"
#include "Triples.h"

using namespace juce;

#define     VERSION_LIT     "Ver 3.3.1"
#define     INIFILENAME     "\\Sudoku\\.ini"
#define     SHOWERRORS      "ShowErrors"
#define     CHECKINGSTATE   "AllowChecking"
#define     HILIGHTING      "AllowHilighting"
#define     TIDYNOTES       "AllowNotesTidy"
#define     RIGHTCLICK      "RightClickTogglesNotes"
#define     LIGHTNESS       0.56f
#define     WIDTH           960
#define     HEIGHT          710

//==============================================================================
/*
*/
class Sudoku  : public juce::Component,
                public Timer,
                public Button::Listener

{
public:
    Sudoku();
    ~Sudoku() override;

public:     //  Functions
    bool flaggingErrorsEnabled() { return cbErrorHilight->getToggleState(); }
    bool hilightingEnabled() { return cbCellHilight->getToggleState(); }
    bool hilightingEnabledNote(int);
private:    //  Functions
    void timerCallback() override;
    void paint (juce::Graphics&) override;
    void resized() override;
    void buttonClicked(Button* buttonThatWasClicked) override;
    bool keyPressed(const juce::KeyPress& key) override;
    void mouseUp(const MouseEvent&) override;
    void createButtons();
    void handleQuit();
    void handleClear();
    void handleNew();
    void handleReset();
    void handleCheck();
    void handleAllPossible();
    void handleFindPairs();
    void checkCompletedNumbers();
    int  countCompletedNumbers(int number);
    void handleCheckboxes();
    void handleHilightChange();
    void tidyNotes();
    void handleNumberEntry(int number);
    void handleCurrentCell(int);
    void hilightRow (int, bool);
    void hilightCol (int, bool);
    void hilightSquare (int, bool);
    void hilightNumbers (int, bool);
    int  cellToSquare(int);
    void findAllPossible(int cell);
    bool isPossible(int cell, int number);
    bool isInSquare(int square, int number);
    bool isInRow(int row, int number);
    bool isInCol(int col, int number);
    void setSolved();
    bool solved();
public:     //  Data
private:    //  Data
    int     NumberToRemove;
    int     CurrentNumber;
    int     CurrentCell;
    int     CurrentRow;
    int     CurrentCol;
    int     Height;
    int     Width;
    SudokuButtonLnF                 sbLnF;
    NumberButtonLnF                 nbLnF;
    std::unique_ptr<SudokuGrid>     grid;
    std::unique_ptr<SudokuGrid>     hold;
    std::unique_ptr<SudokuGrid>     skel;
    std::unique_ptr<IniFile>        IniReg;
    std::unique_ptr<Label>          lbStatus;
    std::unique_ptr<ToggleButton>   cbErrorHilight;
    std::unique_ptr<ToggleButton>   cbCellHilight;
    std::unique_ptr<ToggleButton>   cbChecking;
    std::unique_ptr<ToggleButton>   cbNotesTidy;
    std::unique_ptr<ToggleButton>   cbNotesToggle;
    std::unique_ptr<ToggleButton>   cbShowPairs;
    std::unique_ptr<ToggleButton>   cbShowPossibles;
    //
    std::unique_ptr<TextButton>     bnCheck;
    std::unique_ptr<TextButton>     bnQuit;
    std::unique_ptr<TextButton>     bnNotes;
    std::unique_ptr<TextButton>     bnNew;
    std::unique_ptr<TextButton>     bnReset;
    //
    std::unique_ptr<TextButton>     bnOne;
    std::unique_ptr<TextButton>     bnTwo;
    std::unique_ptr<TextButton>     bnThree;
    std::unique_ptr<TextButton>     bnFour;
    std::unique_ptr<TextButton>     bnFive;
    std::unique_ptr<TextButton>     bnSix;
    std::unique_ptr<TextButton>     bnSeven;
    std::unique_ptr<TextButton>     bnEight;
    std::unique_ptr<TextButton>     bnNine;
    std::unique_ptr<TextButton>     bnClear;
    std::unique_ptr<SudokuButton>   bnCells[N * N];
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sudoku)
};
