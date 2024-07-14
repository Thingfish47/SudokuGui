/*
  ==============================================================================

    Sudoku.cpp
    Created: 9 Jul 2024 11:59:22am
    Author:  godfrey.small

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Sudoku.h"

//==============================================================================
Sudoku::Sudoku()
{
    //srand(time(0));
    setWantsKeyboardFocus(true);
    NumberToRemove = 45;
    for (int i = 0; i < N * N; i++)
    {
        bnCells[i].reset(new SudokuButton(this, 0));
        addAndMakeVisible(bnCells[i].get());
        bnCells[i]->addListener(this);
        //
        bnCells[i]->setLookAndFeel((LookAndFeel*)&sbLnF);
        bnCells[i]->setColour(TextButton::buttonOnColourId, Colours::darkgrey);
        bnCells[i]->setColour(TextButton::buttonColourId, Colours::grey);
        bnCells[i]->setColour(TextButton::textColourOnId, Colours::black);
        bnCells[i]->setColour(TextButton::textColourOffId, Colours::black);
    }
    handleNew();
    //grid.reset (new SudokuGrid());
    //NumberToRemove = 45;
    //    hold.reset(new SudokuGrid(grid.get()));
    //    hold->removeDigits(NumberToRemove);
    //    skel.reset(new SudokuGrid(hold.get()));
    //} while (!hold->resolveSudoku());
    //for (int i = 0; i < N * N; i++)
    //{
    //    int val = grid->getValue(i / N, i % N);
    //    int xval = skel->getValue(i / N, i % N);
    //    bnCells[i].reset(new SudokuButton(this, val));
    //    //bnCells[i]->setCurrentValue(val);
    //    if (xval == 0)
    //        bnCells[i]->setUnknown();
    //    addAndMakeVisible(bnCells[i].get());
    //    bnCells[i]->setLookAndFeel((LookAndFeel *) &sbLnF);
    //    bnCells[i]->setColour(TextButton::buttonOnColourId, Colours::darkgrey);
    //    bnCells[i]->setColour(TextButton::buttonColourId, Colours::grey);
    //    bnCells[i]->setColour(TextButton::textColourOnId, Colours::black);
    //    bnCells[i]->setColour(TextButton::textColourOffId, Colours::black);
    //    bnCells[i]->addListener(this);
    //}
    CurrentCell = -1;
    //removeDigits(45);       //  45 seems insoluable.
    createButtons();
    //
    lbStatus.reset(new Label("lbStatus"));
    lbStatus->setFont(Font(HEIGHT / (N + N)));
    lbStatus->setColour(Label::textColourId, Colours::black);
    lbStatus->setJustificationType(Justification::centred);
    addAndMakeVisible(lbStatus.get());

    IniReg.reset(new IniFile(INIFILENAME));
    String iniString = IniReg->Read(CHECKINGSTATE, "1");
    cbChecking->setToggleState(iniString.compare("1") == 0, dontSendNotification);
    bnCheck->setVisible(cbChecking->getToggleState());
    iniString = IniReg->Read(HILIGHTING, "1");
    cbCellHilight->setToggleState(iniString.compare("1") == 0, dontSendNotification);
    iniString = IniReg->Read(SHOWERRORS, "1");
    cbErrorHilight->setToggleState(iniString.compare("1") == 0, dontSendNotification);
    iniString = IniReg->Read(TIDYNOTES, "1");
    cbNotesTidy->setToggleState(iniString.compare("1") == 0, dontSendNotification);
    setSize(WIDTH, HEIGHT);
}

Sudoku::~Sudoku()
{
    IniReg.reset(new IniFile(INIFILENAME));
    IniReg->SetReg(CHECKINGSTATE, cbChecking->getToggleState() ? "1" : "0");
    IniReg->SetReg(HILIGHTING, cbCellHilight->getToggleState() ? "1" : "0");
    IniReg->SetReg(SHOWERRORS, cbErrorHilight->getToggleState() ? "1" : "0");
    IniReg.reset();
}

void Sudoku::paint (juce::Graphics& g)
{
    g.fillAll ((juce::Colours::lightgrey));   // clear the background

    g.setColour (juce::Colours::black);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    auto bounds = getBounds();
    bounds.setHeight(HEIGHT);
    bounds.setWidth(WIDTH);
    int     size;
    int sizeW = bounds.getWidth() / (N + 2);
    int sizeH = bounds.getHeight() / (N + 2);
    if (sizeW > sizeH)
        size = sizeH;
    else
        size = sizeW;
    auto area = bounds;
    area.removeFromTop(size-2);
    area.removeFromLeft(size-2);
    auto box1 = area.removeFromLeft((N * size) + 4);
    auto box2 = box1.removeFromTop((N * size) + 4);
    auto keep = box2;
    g.drawRect (box2, 4);
    box2.setWidth(2);
    box2.setX(box2.getX() + size+1);
    g.drawRect(box2, 2);
    box2.setX(box2.getX() + size);
    g.drawRect(box2, 2);
    box2.setX(box2.getX() + size-1);
    box2.setWidth(4);
    g.drawRect(box2, 4);
    box2.setWidth(2);
    box2.setX(box2.getX() + size + 1);
    g.drawRect(box2, 2);
    box2.setX(box2.getX() + size);
    g.drawRect(box2, 2);
    box2.setX(box2.getX() + size-1);
    box2.setWidth(4);
    g.drawRect(box2, 4);
    box2.setWidth(2);
    box2.setX(box2.getX() + size + 1);
    g.drawRect(box2, 2);
    box2.setX(box2.getX() + size);
    g.drawRect(box2, 2);
    //
    box2 = keep;
    box2.setHeight(2);
    box2.setY(box2.getY() + size + 1);
    g.drawRect(box2, 2);
    box2.setY(box2.getY() + size);
    g.drawRect(box2, 2);
    box2.setHeight(4);
    box2.setY(box2.getY() + size - 1);
    g.drawRect(box2, 4);
    box2.setHeight(2);
    box2.setY(box2.getY() + size + 1);
    g.drawRect(box2, 2);
    box2.setY(box2.getY() + size);
    g.drawRect(box2, 2);
    box2.setHeight(4);
    box2.setY(box2.getY() + size - 1);
    g.drawRect(box2, 4);
    box2.setHeight(2);
    box2.setY(box2.getY() + size + 1);
    g.drawRect(box2, 2);
    box2.setY(box2.getY() + size);
    g.drawRect(box2, 2);
}

bool Sudoku::keyPressed(const juce::KeyPress& key)
{
    if (CurrentCell < 0)
        return true;
    ModifierKeys mods = key.getModifiers();
    if (mods.isAnyModifierKeyDown() || mods.isAnyMouseButtonDown())
        return true;
    if (!bnCells[CurrentCell]->isUnknown())
        return false;
    juce_wchar ch = key.getTextCharacter();
    if (ch >= '1' && ch <= '9')
    {
        int number = ch - '0';
        handleNumberEntry(number);
    }
    return false;
}

void Sudoku::buttonClicked(Button* buttonThatWasClicked)
{

    if (buttonThatWasClicked == bnQuit.get())
        handleQuit();
    else if (buttonThatWasClicked == cbChecking.get())
        handleCheckboxes();
    else if (buttonThatWasClicked == cbCellHilight.get())
        handleHilightChange();
    else if (buttonThatWasClicked == cbErrorHilight.get())
        handleCheckboxes();
    else if (buttonThatWasClicked == bnClear.get())
        handleClear();
    else if (buttonThatWasClicked == bnCheck.get())
        handleCheck();
    else if (buttonThatWasClicked == bnNew.get())
        handleNew();
    else if (buttonThatWasClicked == bnOne.get())
        handleNumberEntry(1);
    else if (buttonThatWasClicked == bnTwo.get())
        handleNumberEntry(2);
    else if (buttonThatWasClicked == bnThree.get())
        handleNumberEntry(3);
    else if (buttonThatWasClicked == bnFour.get())
        handleNumberEntry(4);
    else if (buttonThatWasClicked == bnFive.get())
        handleNumberEntry(5);
    else if (buttonThatWasClicked == bnSix.get())
        handleNumberEntry(6);
    else if (buttonThatWasClicked == bnSeven.get())
        handleNumberEntry(7);
    else if (buttonThatWasClicked == bnEight.get())
        handleNumberEntry(8);
    else if (buttonThatWasClicked == bnNine.get())
        handleNumberEntry(9);
    else
    {
        for (int i=0 ; i<N*N ; i++)
            if (buttonThatWasClicked == bnCells[i].get())
            {
                handleCurrentCell(i);
                return;
            }
    }
}

bool Sudoku::isInSquare(int square, int number)
{
    int topLeftR = (square / NS) * NS;
    int topLeftC = (square % NS) * NS;
    for (int r = 0; r < NS; r++)
        for (int c = 0; c < NS; c++)
            if (bnCells[((topLeftR + r) * N) + topLeftC + c]->getCurrentValue() == number)
                return true;
    return false;
}

bool Sudoku::isInRow(int row, int number)
{
    for (int i=0 ; i<N ; i++)
        if (bnCells[(row * N) + i]->getCurrentValue() == number)
            return true;
    return false;
}

bool Sudoku::isInCol(int col, int number)
{
    for (int i = 0; i < N; i++)
        if (bnCells[col + (i * N)]->getCurrentValue() == number)
            return true;
    return false;

}

//int Sudoku::countPossibles(int cell, int* posibilities)
//{
//    bool    possibles[N];
//    
//    for (int i = 0; i < N; i++)
//        possibles[i] = true;
//
//    int row = cell / N;
//    int col = cell % N;
//    int square = cellToSquare(cell);
//    for (int i = 0; i < N; i++)
//    {
//        if (isInSquare(square, i + 1))
//            possibles[i] = false;
//        else if (isInRow(row, i+1))
//            possibles[i] = false;
//        else if (isInCol(col, i+1))
//            possibles[i] = false;
//    }
//    int posibleCount = 0;
//    for (int i = 0; i < N; i++)
//        if (possibles[i])
//        {
//            posibilities[posibleCount++] = i + 1;
//        }
//    return posibleCount;
//}

void Sudoku::handleCurrentCell(int newCurrent)
{
    bool    prevToggleState = false;

    lbStatus->setText("", dontSendNotification);
    if (CurrentCell >= 0)
    {
	    hilightRow(CurrentRow, false);
	    hilightCol(CurrentCol, false);
	    hilightSquare(cellToSquare(CurrentCell), false);
	    hilightNumbers (CurrentNumber, false);
	    prevToggleState = bnCells[CurrentCell]->getToggleState();
	    bnCells[CurrentCell]->setToggleState(false, dontSendNotification);
    }

    CurrentCell = newCurrent;
    CurrentNumber = bnCells[CurrentCell]->isUnknown() ? 0 : bnCells[CurrentCell]->getActualValue();
    CurrentRow = CurrentCell / N;
    CurrentCol = CurrentCell % N;
    if (hilightingEnabled())
    {
        hilightSquare(cellToSquare(CurrentCell), true);
        hilightRow(CurrentRow, true);
        hilightCol(CurrentCol, true);
        hilightNumbers(bnCells[CurrentCell]->getCurrentValue(), true);
    }
    bnCells[CurrentCell]->setToggleState(true, dontSendNotification);
    repaint();
}

void Sudoku::handleNumberEntry(int Number)
{
    if (CurrentCell < 0)
        return;
    lbStatus->setText("", dontSendNotification);
    if (bnNotes->getToggleState() == false)
    {
        bnCells[CurrentCell]->setCurrentValue(Number);
        checkCompletedNumbers();
        if (solved())
            setSolved();
        else if (cbNotesTidy->getToggleState())
            tidyNotes();
    }
    else
    {
        bnCells[CurrentCell]->toggleNote(Number);
        repaint();
    }
}

int Sudoku::countCompletedNumbers(int number)
{
    int count = 0;
    for (int i = 0; i < N * N; i++)
    {
        if (bnCells[i]->getCurrentValue() == number)
            count++;
    }
    return count;
}

void Sudoku::checkCompletedNumbers()
{
    bnOne->setVisible(countCompletedNumbers(1) != 9);
    bnTwo->setVisible(countCompletedNumbers(2) != 9);
    bnThree->setVisible(countCompletedNumbers(3) != 9);
    bnFour->setVisible(countCompletedNumbers(4) != 9);
    bnFive->setVisible(countCompletedNumbers(5) != 9);
    bnSix->setVisible(countCompletedNumbers(6) != 9);
    bnSeven->setVisible(countCompletedNumbers(7) != 9);
    bnEight->setVisible(countCompletedNumbers(8) != 9);
    bnNine->setVisible(countCompletedNumbers(9) != 9);
}

bool Sudoku::solved()
{
    for (int i = 0; i < N * N; i++)
    {
        if (bnCells[i]->isUnknown() || bnCells[i]->isWrong())
            return false;
    }
    return true;
}

void Sudoku::tidyNotes()
{
    int note = bnCells[CurrentCell]->getCurrentValue();
    int row = CurrentRow * N;
	for (int i = 0; i < N; i++)
	{
		if (bnCells[row+i]->getNote(note))
			bnCells[row+i]->clearNote(note);
    }
    int col = CurrentCol;
    for (int i = 0; i < N; i++)
    {
        if (bnCells[col + (i * N)]->getNote(note))
            bnCells[col + (i * N)]->clearNote(note);
    }
    int square = cellToSquare(CurrentCell);
    int topLeftR = (square / NS) * NS;
    int topLeftC = (square % NS) * NS;
    for (int r = 0; r < NS; r++)
        for (int c = 0; c < NS; c++)
        {
            int idx = ((topLeftR + r) * N) + topLeftC + c;
            if (bnCells[idx]->getNote(note))
                bnCells[idx]->clearNote(note);
        }
    repaint();
}

void Sudoku::setSolved()
{
    if (CurrentCell >= 0)
    {
        hilightRow(CurrentRow, false);
        hilightCol(CurrentCol, false);
        hilightSquare(cellToSquare(CurrentCell), false);
        bnCells[CurrentCell]->setToggleState(false, dontSendNotification);
    }
    bnOne->setVisible(false);
    bnTwo->setVisible(false);
    bnThree->setVisible(false);
    bnFour->setVisible(false);
    bnFive->setVisible(false);
    bnSix->setVisible(false);
    bnSeven->setVisible(false);
    bnEight->setVisible(false);
    bnNine->setVisible(false);
    bnClear->setVisible(false);
    for (int i = 0; i < N * N; i++)
        bnCells[i]->setEnabled(false);
    lbStatus->setText("Solved!", dontSendNotification);
}

bool Sudoku::hilightingEnabledNote(int number)
{
    if (!cbCellHilight->getToggleState())
        return false;
    return number == CurrentNumber;
}
void Sudoku::handleHilightChange()
{
    if (CurrentCell < 0)
        return;
    handleCurrentCell(CurrentCell);
}

void Sudoku::handleCheckboxes()
{
    if (cbErrorHilight->getToggleState())
    {
        cbChecking->setToggleState(false, dontSendNotification);
    }
    bnCheck->setVisible(cbChecking->getToggleState());
}

void Sudoku::handleCheck()
{
    char    msg[32];
    int errCt = 0;

    for (int i = 0; i < N * N; i++)
    {
        if (bnCells[i]->isEnabled() && bnCells[i]->isWrong())
            errCt++;
    }
    if (errCt == 0)
        sprintf(msg, "No errors");
    else if (errCt == 1)
        sprintf(msg, "1 cell is wrong");
    else
        sprintf(msg, "%d cells are wrong", errCt);
    lbStatus->setText(msg, dontSendNotification);
}

void Sudoku::handleNew()
{
    grid.reset (new SudokuGrid());
    hold.reset(new SudokuGrid(grid.get()));
    hold->removeDigits(NumberToRemove);
    skel.reset(new SudokuGrid(hold.get()));
    for (int i = 0; i < N * N; i++)
    {
        int val = grid->getValue(i / N, i % N);
        int xval = skel->getValue(i / N, i % N);
        bnCells[i]->resetCell(val);
        if (xval == 0)
            bnCells[i]->setUnknown();
    }
    CurrentCell = -1;
}

void Sudoku::handleClear()
{
    if (bnNotes->getToggleState() == false)
    {
        bnCells[CurrentCell]->setCurrentValue(0);
        bnCells[CurrentCell]->setButtonText("");
        checkCompletedNumbers();
    }
}

void Sudoku::handleQuit()
{
    JUCEApplication::getInstance()->systemRequestedQuit();
}

void Sudoku::hilightRow(int row, bool state)
{
    Colour hue = Colours::grey;
    if (state)
        hue = Colour::greyLevel(LIGHTNESS);
    for (int i = 0; i < N; i++)
        bnCells[(row * N) + i]->setColour(TextButton::buttonColourId, hue);
}

void Sudoku::hilightNumbers(int number, bool state)
{
    Colour hue = Colours::grey;
    if (state)
        hue = Colour::greyLevel(LIGHTNESS);
    for (int i = 0; i < N * N; i++)
    {
        if (!bnCells[i]->isUnknown() && bnCells[i]->getActualValue() == number)
            bnCells[i]->setColour(TextButton::buttonColourId, hue);
    }
}

void Sudoku::hilightCol(int col, bool state)
{
    Colour hue = Colours::grey;
    if (state)
        hue = Colour::greyLevel(LIGHTNESS);
    for (int i = 0; i < N; i++)
        bnCells[(i * N) + col]->setColour(TextButton::buttonColourId, hue);
}

void Sudoku::hilightSquare(int square, bool state)
{
    Colour hue = Colours::grey;
    if (state)
        hue = Colour::greyLevel(LIGHTNESS);
    int topLeftR = (square / NS) * NS;
    int topLeftC = (square % NS) * NS;
    for (int r = 0; r < NS; r++)
        for (int c=0 ; c<NS ; c++)
            bnCells[((topLeftR + r) * N) + topLeftC + c]->setColour(TextButton::buttonColourId, hue);
}

int  Sudoku::cellToSquare(int cell)
{
    int row = cell / N;
    int col = cell % N;

    return ((row / NS) * NS) + (col / NS);
}

void Sudoku::resized()
{
    auto bounds = getBounds();
    int     size;
    int sizeW = bounds.getWidth() / (N + 6);
    int sizeH = bounds.getHeight() / (N + 2);
    if (sizeW > sizeH)
        size = sizeH;
    else
        size = sizeW;
    auto area = bounds;
    auto label = area.removeFromTop(size);
    label.removeFromLeft(size);
    lbStatus->setBounds (label.removeFromLeft(size*N));
    area.removeFromLeft(size);
    int idx = 0;
    for (int r = 0; r < N; r++)
    {
        auto row = area.removeFromTop(size);
        for (int c = 0; c < N; c++)
        {
            bnCells[idx++]->setBounds(row.removeFromLeft(size).reduced(2));
        }
        if (r == 0)
        {
            row.removeFromLeft(size);
            bnOne->setBounds(row.removeFromLeft(size).reduced(4));
            bnTwo->setBounds(row.removeFromLeft(size).reduced(4));
            bnThree->setBounds(row.removeFromLeft(size).reduced(4));
        }
        if (r == 1)
        {
            row.removeFromLeft(size);
            bnFour->setBounds(row.removeFromLeft(size).reduced(4));
            bnFive->setBounds(row.removeFromLeft(size).reduced(4));
            bnSix->setBounds(row.removeFromLeft(size).reduced(4));
        }
        if (r == 2)
        {
            row.removeFromLeft(size);
            bnSeven->setBounds(row.removeFromLeft(size).reduced(4));
            bnEight->setBounds(row.removeFromLeft(size).reduced(4));
            bnNine->setBounds(row.removeFromLeft(size).reduced(4));
        }
        if (r == 3)
        {
            row.removeFromLeft(size);
            row.removeFromLeft(size);
            bnClear->setBounds(row.removeFromLeft(size).reduced(4));
        }
    }
    auto quit = area.removeFromLeft(size * 2).reduced(4);
    quit.removeFromTop(size / 5);
    quit.removeFromBottom(size / 5);
    bnQuit->setBounds(quit);
    auto check = area.removeFromLeft(size * 5);
    check.removeFromLeft(size + size / 2);
    check.removeFromRight(size + size / 2);
    check.removeFromTop(size / 5);
    check.removeFromBottom(size / 5);
    bnCheck->setBounds(check.reduced(4));
    auto notes = area.removeFromLeft(size * 2).reduced(4);
    notes.removeFromTop(size / 5);
    notes.removeFromBottom(size / 5);
    bnNotes->setBounds(notes);
    area = bounds;
    area.removeFromTop(size * 5);
    area.removeFromLeft(size * 11);
    cbCellHilight->setBounds(area.removeFromTop(size / 2));
    cbChecking->setBounds(area.removeFromTop(size / 2));
    cbErrorHilight->setBounds(area.removeFromTop(size / 2));
    cbNotesTidy->setBounds(area.removeFromTop(size / 2));
    auto buts = area.removeFromBottom(size);
    buts.removeFromRight(4);
    buts.removeFromTop(size / 5);
    buts.removeFromBottom(size / 5);
    auto bnew = buts.removeFromRight(size * 2);
    bnew.setY(notes.getY());
    bnNew->setBounds(bnew);
}

void Sudoku::createButtons()
{
    cbChecking.reset(new ToggleButton("Checking allowed"));
    addAndMakeVisible(cbChecking.get());
    cbChecking->addListener(this);
    cbChecking->setColour(ToggleButton::textColourId, Colours::black);
    cbChecking->setColour(ToggleButton::tickColourId, Colours::black);
    //
    cbCellHilight.reset(new ToggleButton("Hilighting on"));
    addAndMakeVisible(cbCellHilight.get());
    cbCellHilight->addListener(this);
    cbCellHilight->setColour(ToggleButton::textColourId, Colours::black);
    cbCellHilight->setColour(ToggleButton::tickColourId, Colours::black);
    //
    cbErrorHilight.reset(new ToggleButton("Show errors"));
    addAndMakeVisible(cbErrorHilight.get());
    cbErrorHilight->addListener(this);
    cbErrorHilight->setColour(ToggleButton::textColourId, Colours::black);
    cbErrorHilight->setColour(ToggleButton::tickColourId, Colours::black);
    //
    cbNotesTidy.reset(new ToggleButton("Tidy notes"));
    addAndMakeVisible(cbNotesTidy.get());
    cbNotesTidy->addListener(this);
    cbNotesTidy->setColour(ToggleButton::textColourId, Colours::black);
    cbNotesTidy->setColour(ToggleButton::tickColourId, Colours::black);
    //
    bnCheck.reset(new TextButton("Check"));
    addAndMakeVisible(bnCheck.get());
    bnCheck->setColour(TextButton::buttonOnColourId, Colours::red);
    bnCheck->setColour(TextButton::buttonColourId, Colours::green);
    bnCheck->addListener(this);
    //
    bnQuit.reset(new TextButton("Quit"));
    addAndMakeVisible(bnQuit.get());
    bnQuit->setColour(TextButton::buttonOnColourId, Colours::red);
    bnQuit->setColour(TextButton::buttonColourId, Colours::green);
    bnQuit->addListener(this);
    //
    bnNotes.reset(new TextButton("Notes"));
    addAndMakeVisible(bnNotes.get());
    bnNotes->setColour(TextButton::buttonOnColourId, Colours::red);
    bnNotes->setColour(TextButton::buttonColourId, Colours::green);
    bnNotes->setClickingTogglesState(true);
    bnNotes->addListener(this);
    //
    bnNew.reset(new TextButton("New"));
    addAndMakeVisible(bnNew.get());
    bnNew->setColour(TextButton::buttonOnColourId, Colours::red);
    bnNew->setColour(TextButton::buttonColourId, Colours::green);
    bnNew->addListener(this);

    //
    bnOne.reset(new TextButton("1"));
    addAndMakeVisible(bnOne.get());
    bnOne->setColour(TextButton::buttonOnColourId, Colours::red);
    bnOne->setColour(TextButton::buttonColourId, Colours::palegreen);
    bnOne->setColour(TextButton::textColourOnId, Colours::black);
    bnOne->setColour(TextButton::textColourOffId, Colours::black);
    bnOne->setLookAndFeel((LookAndFeel*)&nbLnF);
    bnOne->addListener(this);
    //
    bnTwo.reset(new TextButton("2"));
    addAndMakeVisible(bnTwo.get());
    bnTwo->setColour(TextButton::buttonOnColourId, Colours::red);
    bnTwo->setColour(TextButton::buttonColourId, Colours::palegreen);
    bnTwo->setColour(TextButton::textColourOnId, Colours::black);
    bnTwo->setColour(TextButton::textColourOffId, Colours::black);
    bnTwo->setLookAndFeel((LookAndFeel*)&nbLnF);
    bnTwo->addListener(this);
    //
    bnThree.reset(new TextButton("3"));
    addAndMakeVisible(bnThree.get());
    bnThree->setColour(TextButton::buttonOnColourId, Colours::red);
    bnThree->setColour(TextButton::buttonColourId, Colours::palegreen);
    bnThree->setColour(TextButton::textColourOnId, Colours::black);
    bnThree->setColour(TextButton::textColourOffId, Colours::black);
    bnThree->setLookAndFeel((LookAndFeel*)&nbLnF);
    bnThree->addListener(this);
    //
    bnFour.reset(new TextButton("4"));
    addAndMakeVisible(bnFour.get());
    bnFour->setColour(TextButton::buttonOnColourId, Colours::red);
    bnFour->setColour(TextButton::buttonColourId, Colours::palegreen);
    bnFour->setColour(TextButton::textColourOnId, Colours::black);
    bnFour->setColour(TextButton::textColourOffId, Colours::black);
    bnFour->addListener(this);
    bnFour->setLookAndFeel((LookAndFeel*)&nbLnF);
    bnFour->addListener(this);
    //
    bnFive.reset(new TextButton("5"));
    addAndMakeVisible(bnFive.get());
    bnFive->setColour(TextButton::buttonOnColourId, Colours::red);
    bnFive->setColour(TextButton::buttonColourId, Colours::palegreen);
    bnFive->setColour(TextButton::textColourOnId, Colours::black);
    bnFive->setColour(TextButton::textColourOffId, Colours::black);
    bnFive->setLookAndFeel((LookAndFeel*)&nbLnF);
    bnFive->addListener(this);
    //
    bnSix.reset(new TextButton("6"));
    addAndMakeVisible(bnSix.get());
    bnSix->setColour(TextButton::buttonOnColourId, Colours::red);
    bnSix->setColour(TextButton::buttonColourId, Colours::palegreen);
    bnSix->setColour(TextButton::textColourOnId, Colours::black);
    bnSix->setColour(TextButton::textColourOffId, Colours::black);
    bnSix->setLookAndFeel((LookAndFeel*)&nbLnF);
    bnSix->addListener(this);
    //
    bnSeven.reset(new TextButton("7"));
    addAndMakeVisible(bnSeven.get());
    bnSeven->setColour(TextButton::buttonOnColourId, Colours::red);
    bnSeven->setColour(TextButton::buttonColourId, Colours::palegreen);
    bnSeven->setColour(TextButton::textColourOnId, Colours::black);
    bnSeven->setColour(TextButton::textColourOffId, Colours::black);
    bnSeven->setLookAndFeel((LookAndFeel*)&nbLnF);
    bnSeven->addListener(this);
    //
    bnEight.reset(new TextButton("8"));
    addAndMakeVisible(bnEight.get());
    bnEight->setColour(TextButton::buttonOnColourId, Colours::red);
    bnEight->setColour(TextButton::buttonColourId, Colours::palegreen);
    bnEight->setColour(TextButton::textColourOnId, Colours::black);
    bnEight->setColour(TextButton::textColourOffId, Colours::black);
    bnEight->setLookAndFeel((LookAndFeel*)&nbLnF);
    bnEight->addListener(this);
    //
    bnNine.reset(new TextButton("9"));
    addAndMakeVisible(bnNine.get());
    bnNine->setColour(TextButton::buttonOnColourId, Colours::red);
    bnNine->setColour(TextButton::buttonColourId, Colours::palegreen);
    bnNine->setColour(TextButton::textColourOnId, Colours::black);
    bnNine->setColour(TextButton::textColourOffId, Colours::black);
    bnNine->setLookAndFeel((LookAndFeel*)&nbLnF);
    bnNine->addListener(this);
    //
    bnClear.reset(new TextButton("clr"));
    addAndMakeVisible(bnClear.get());
    bnClear->setColour(TextButton::buttonOnColourId, Colours::red);
    bnClear->setColour(TextButton::buttonColourId, Colours::palegreen);
    bnClear->setColour(TextButton::textColourOnId, Colours::black);
    bnClear->setColour(TextButton::textColourOffId, Colours::black);
    bnClear->setLookAndFeel((LookAndFeel*)&nbLnF);
    bnClear->addListener(this);
}


