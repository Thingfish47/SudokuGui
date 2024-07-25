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
#include "Cell.h"
#include "SudokuButtonLnF.h"
#include "Pairs.h"
#include "Triples.h"

using namespace juce;

//#define     PAIRSDEBUG


#define     VERSION_LIT     "Ver 3.3.1"
#define     INIFILENAME     "\\Sudoku\\.ini"
#define     SAVEFILENAME    "\\Sudoku\\.save"
#define     SHOWERRORS      "ShowErrors"
#define     CHECKINGSTATE   "AllowChecking"
#define     HILIGHTING      "AllowHilighting"
#define     TIDYNOTES       "AllowNotesTidy"
#define     RIGHTCLICK      "RightClickTogglesNotes"
#define     LIGHTNESS       0.56f
#define     WIDTH           960
#define     HEIGHT          710

const int SqIdx[] = { 0, 3, 6, 27, 30, 33, 54, 57, 60 };

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
    void handleTest();
    void handleClear();
    void handleNew();
    void handleLoad();
    void handleSave();
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
    int  cellToRow(int x) { return x / N; }
    int  cellToCol(int x) { return x % N; }
    void findAllPossible(int cell);
    bool isPossible(int cell, int number);
    bool isInSquare(int square, int number);
    bool isInRow(int row, int number);
    bool isInCol(int col, int number);
    void setSolved();
    bool solved();
    void printGrid();
    bool onlyInRow(int);
    bool onlyInCol(int);
    bool onlyInSquare(int);
    bool checkPairsRow(int);
    bool checkPairsCol(int);
    bool checkPairsSquare(int);
    bool checkHiddenMatchedPairsRow(int);
    bool checkHiddenMatchedPairsCol(int);
    bool checkHiddenMatchedPairsSquare(int);
    bool checkHiddenPairsSquare(int);
    Cell* getCellxCol(int x, int Col);
    Cell* getCellxRow(int x, int Row);
    Cell* getCellxSqr(int x, int Sqr);
    int  getIdxxCol(int x, int Col);
    int  getIdxxRow(int x, int Row);
    int  getIdxxSqr(int x, int Sqr);
    String cellToRowCol(int);
    void setMatchedPairs();
public:     //  Data
private:    //  Data
    int     NumberToRemove;
    int     CurrentNumber;
    int     CurrentCell;
    int     CurrentRow;
    int     CurrentCol;
    int     Height;
    int     Width;
    int     Phase = 0;
    SudokuButtonLnF                 sbLnF;
    NumberButtonLnF                 nbLnF;
    std::unique_ptr<Pair>           matchedPairs[N*N];
    std::unique_ptr<SudokuGrid>     grid;
    std::unique_ptr<SudokuGrid>     hold;
    std::unique_ptr<SudokuGrid>     skel;
    std::unique_ptr<IniFile>        IniReg;
    std::unique_ptr<IniFile>        SaveReg;
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
    std::unique_ptr<TextButton>     bnEnter;
    std::unique_ptr<TextButton>     bnLoad;
    std::unique_ptr<TextButton>     bnSave;
    std::unique_ptr<TextButton>     bnTest;
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
    std::unique_ptr<Cell>           bnCells[N * N];
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sudoku)
};
