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

using namespace juce;

#define     VERSION_LIT     "Ver 3.3.1"
#define     INIFILENAME     "\\Sudoku\\.ini"
#define     SHOWERRORS      "ShowErrors"
#define     CHECKINGSTATE   "AllowChecking"
#define     HILIGHTING      "AllowHilighting"
#define     WIDTH           960
#define     HEIGHT          710

//==============================================================================
/*
*/
class Sudoku  : public juce::Component,
                public Button::Listener

{
public:
    Sudoku();
    ~Sudoku() override;

public:     //  Functions
    bool flaggingErrorsEnabled() { return cbErrorHilight->getToggleState(); }
private:    //  Functions
    void paint (juce::Graphics&) override;
    void resized() override;
    void buttonClicked(Button* buttonThatWasClicked) override;
    bool keyPressed(const juce::KeyPress& key) override;
    void createButtons();
    void handleQuit();
    void handleClear();
    void handleCheck();
    void handleCheckboxes();
    void removeDigits(int numToRemove);
    void handleNumberEntry(int number);
    void handleCurrentSquare(int);
    void hilightRow (int, bool);
    void hilightCol (int, bool);
    void hilightSquare (int, bool);
    int  collToSquare(int);
    void setSolved();
    bool solved();
    bool hilightingEnabled() { return cbCellHilight->getToggleState(); }
public:     //  Data
private:    //  Data
    int     CurrentCell;
    int     CurrentRow;
    int     CurrentCol;
    int     Height;
    int     Width;
    SudokuButtonLnF                 sbLnF;
    NumberButtonLnF                 nbLnF;
    std::unique_ptr<SudokuGrid>     grid;
    std::unique_ptr<IniFile>        IniReg;
    std::unique_ptr<Label>          lbStatus;
    std::unique_ptr<ToggleButton>   cbErrorHilight;
    std::unique_ptr<ToggleButton>   cbCellHilight;
    std::unique_ptr<ToggleButton>   cbChecking;
    std::unique_ptr<TextButton>     bnCheck;
    std::unique_ptr<TextButton>     bnQuit;
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
    std::unique_ptr<TextButton>     bnNotes;
    std::unique_ptr<SudokuButton>   bnCells[N * N];
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sudoku)
};
