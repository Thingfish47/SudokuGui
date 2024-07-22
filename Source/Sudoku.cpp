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
    CurrentCell = -1;
    //
    for (int i = 0; i < N * N; i++)
    {
        bnCells[i].reset(new Cell(this, 0));
        addAndMakeVisible(bnCells[i].get());
        bnCells[i]->addListener(this);
        //
        bnCells[i]->setLookAndFeel((LookAndFeel*)&sbLnF);
        bnCells[i]->setColour(TextButton::buttonOnColourId, Colours::darkgrey);
        bnCells[i]->setColour(TextButton::buttonColourId, Colours::grey);
        bnCells[i]->setColour(TextButton::textColourOnId, Colours::black);
        bnCells[i]->setColour(TextButton::textColourOffId, Colours::black);
    }
    lbStatus.reset(new Label("lbStatus"));
    lbStatus->setFont(Font(HEIGHT / (N + N)));
    lbStatus->setColour(Label::textColourId, Colours::black);
    lbStatus->setJustificationType(Justification::centred);
    addAndMakeVisible(lbStatus.get());

    createButtons();
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
    iniString = IniReg->Read(RIGHTCLICK, "1");
    cbNotesToggle->setToggleState(iniString.compare("1") == 0, dontSendNotification);
    addMouseListener(this, true);
    handleNew();
    setSize(WIDTH, HEIGHT);
}

Sudoku::~Sudoku()
{
    IniReg.reset(new IniFile(INIFILENAME));
    IniReg->SetReg(CHECKINGSTATE, cbChecking->getToggleState() ? "1" : "0");
    IniReg->SetReg(HILIGHTING, cbCellHilight->getToggleState() ? "1" : "0");
    IniReg->SetReg(SHOWERRORS, cbErrorHilight->getToggleState() ? "1" : "0");
    IniReg->SetReg(RIGHTCLICK, cbNotesToggle->getToggleState() ? "1" : "0");
    IniReg->SetReg(TIDYNOTES, cbNotesTidy->getToggleState() ? "1" : "0");
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

void Sudoku::timerCallback()
{
    lbStatus->setText("", dontSendNotification);
    stopTimer();
}

void Sudoku::mouseUp(const MouseEvent& event)
{
    if (event.mods.isRightButtonDown())
    {
        if (cbNotesToggle->getToggleState() == false)
            return;
        // Handle the right mouse button up event here
        DBG("Mouse up");
        if (bnNotes->getToggleState() == false)
            bnNotes->setToggleState(true, dontSendNotification);
        else
            bnNotes->setToggleState(false, dontSendNotification);
        bnClear->setVisible(bnNotes->getToggleState() ? false : true);
    }
}

bool Sudoku::keyPressed(const juce::KeyPress& key)
{
    if (CurrentCell < 0)
        return true;
    ModifierKeys mods = key.getModifiers();
    if (mods.isAnyModifierKeyDown() || mods.isAnyMouseButtonDown())
    {
        DBG("Modifiers down");
        return true;
    }
    juce_wchar ch = key.getTextCharacter();
    if (ch >= '1' && ch <= '9')
    {
        //if (!bnCells[CurrentCell]->isUnknown())
        //{
        //    DBG("CurrentCell Not unknown");
        //    return false;
        //}
        int number = ch - '0';
        handleNumberEntry(number);
    }
    return false;
}

void Sudoku::buttonClicked(Button* buttonThatWasClicked)
{

    if (buttonThatWasClicked == bnQuit.get())
        handleQuit();
    else if (buttonThatWasClicked == bnEnter.get())
    {
        bnLoad->setVisible(bnEnter->getToggleState());
        bnSave->setVisible(bnEnter->getToggleState());
    }
    else if (buttonThatWasClicked == bnNotes.get())
        bnClear->setVisible(bnNotes->getToggleState() ? false : true);
    else if (buttonThatWasClicked == bnSave.get())
        handleSave();
    else if (buttonThatWasClicked == bnLoad.get())
        handleLoad();
    else if (buttonThatWasClicked == cbShowPairs.get())
        handleFindPairs();
    else if (buttonThatWasClicked == cbChecking.get())
        handleCheckboxes();
    else if (buttonThatWasClicked == cbCellHilight.get())
        handleHilightChange();
    else if (buttonThatWasClicked == cbErrorHilight.get())
        handleCheckboxes();
    else if (buttonThatWasClicked == cbShowPossibles.get())
        handleAllPossible();
    else if (buttonThatWasClicked == bnClear.get())
        handleClear();
    else if (buttonThatWasClicked == bnCheck.get())
        handleCheck();
    else if (buttonThatWasClicked == bnNew.get())
        handleNew();
    else if (buttonThatWasClicked == bnReset.get())
        handleReset();
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

void Sudoku::findAllPossible (int cell)
{
    for (int i = 0; i < N; i++)
        bnCells[cell]->clearNote(i+1);
    if (!bnCells[cell]->isUnknown())
        return;
    if (bnCells[cell]->getCurrentValue())
        return;
    for (int i = 0; i < N; i++)
    {
        if (isPossible(cell, i+1))
            bnCells[cell]->toggleNote(i+1);
    }
}

bool Sudoku::isPossible (int cell, int number)
{
    int row = cell / N;
    int col = cell % N;
    int square = cellToSquare(cell);

    if (isInSquare(square, number))
        return false;
    if (isInRow(row, number))
        return false;
    if (isInCol(col, number))
        return false;
    return true;
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

    //lbStatus->setText("", dontSendNotification);
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
    if (bnNotes->getToggleState() == false)
    {
        if (bnEnter->getToggleState() == true)
            bnCells[CurrentCell]->setActualValue(Number);
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
        bool check = bnCells[i]->checkCorrect();
        if (!check)
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

void Sudoku::handleReset()
{
    lbStatus->setText("", dontSendNotification);
    for (int i = 0; i < N * N; i++)
    {
        bnCells[i]->setEnabled(true);
        if (bnCells[i]->isUnknown())
        {
            bnCells[i]->setCurrentValue(0);
            bnCells[i]->clearAllNotes();
        }
    }
    checkCompletedNumbers();
    repaint();
}

void Sudoku::handleLoad()
{
    String Key;
    String Value;
    SaveReg.reset(new IniFile(SAVEFILENAME));
    for (int i = 0; i < N * N; i++)
    {
        Key = "CELL" + String(i + 1);
        Value = SaveReg->Read(Key.toStdString(), "0");
        int val = Value[0] - '0';
        bnCells[i]->resetCell(val);
    }
    SaveReg.reset();
    repaint();
}

void Sudoku::handleSave()
{
    String Key;
    String Value;
    SaveReg.reset(new IniFile(SAVEFILENAME));
    for (int i = 0; i < N * N; i++)
    {
        Key = "CELL" + String(i + 1);
        Value = String(bnCells[i]->getActualValue());
        SaveReg->SetReg(Key.toStdString(), Value.toStdString());
    }
    SaveReg->WriteAll();
    SaveReg.reset();
}

void Sudoku::handleCheck()
{
    char    msg[32];
    int errCt = 0;

    for (int i = 0; i < N * N; i++)
    {
        if (bnCells[i]->isUnknown() && bnCells[i]->isWrong())
            errCt++;
    }
    if (errCt == 0)
        sprintf(msg, "No errors");
    else if (errCt == 1)
        sprintf(msg, "1 cell is wrong");
    else
        sprintf(msg, "%d cells are wrong", errCt);
    lbStatus->setText(msg, dontSendNotification);
    startTimer(3000);
}

void Sudoku::handleNew()
{
    lbStatus->setText("", dontSendNotification);
    grid.reset(new SudokuGrid());
    hold.reset(new SudokuGrid(grid.get()));
    hold->removeDigits(NumberToRemove);
    //skel.reset(new SudokuGrid(hold.get()));
    for (int i = 0; i < N * N; i++)
    {
        int val = grid->getValue(i / N, i % N);
        int xval = hold->getValue(i / N, i % N);
        bnCells[i]->resetCell(val);
        bnCells[i]->setEnabled(true);
        if (xval == 0)
            bnCells[i]->setUnknown();
    }
    bnOne->setVisible(true);
    bnTwo->setVisible(true);
    bnThree->setVisible(true);
    bnFour->setVisible(true);
    bnFive->setVisible(true);
    bnSix->setVisible(true);
    bnSeven->setVisible(true);
    bnEight->setVisible(true);
    bnNine->setVisible(true);
    bnClear->setVisible(bnNotes->getToggleState() ? false : true);
    CurrentCell = -1;
}

void Sudoku::handleAllPossible()
{
    DBG("all possible");
    for (int cell = 0; cell < N * N; cell++)
        findAllPossible(cell);
    cbShowPossibles->setToggleState(false, dontSendNotification);
    repaint();
}

void Sudoku::printGrid()
{
    for (int row = 0; row < N; row++)
    {
        String RowTxt;
        for (int i = 0; i < N; i++)
        {
            int val = bnCells[getIdxxRow(i, row)]->getCurrentValue();
            RowTxt += String(val) + " ";
        }
        DBG(RowTxt);
    }
}


void Sudoku::handleFindPairs()
{
    cbShowPairs->setToggleState(false, dontSendNotification);
    DBG("handleFindPairs");

    for (int cell = 0; cell < N * N; cell++)
        matchedPairs[cell].reset();
 
    printGrid();
    for (int cell = 0; cell < N * N; cell++)
    {
        if (!bnCells[cell]->isUnknown())
            continue;
        if (bnCells[cell]->countNotes() == 2)
        {
            matchedPairs[cell].reset (new Pair (bnCells[cell]->getPair()));
#ifdef PAIRSDEBUG
            DBG("cell " << cell << " has pair " << matchedPairs[cell]->get1st() << " ," << matchedPairs[cell]->get2nd());
#endif // PAIRSDEBUG
        }
	}
    for (int row=0 ; row<N ; row++)
        if (checkPairsRow(row))
        {
            int brkpt1 = 0;
        }
    for (int col=0 ; col<N ; col++)
        if (checkPairsCol(col))
        {
            int brkpt1 = 0;
        }
    for (int sqr=0 ; sqr <N ; sqr++)
        if (checkPairsSquare(sqr))
        {
            int brkpt1 = 0;
        }
//    for (int Id = 0; Id < N; Id++)
//    {
//        onlyInRow(Id);
//        onlyInCol(Id);
//        onlyInSquare(Id);
//        checkPairsRow(Id);
//        checkPairsCol(Id);
//        checkPairsSquare(Id);
//    }
////    repaint();
    int     Brkpt = 0;
//    int solveCt = enterSolos();
//    while (solveCt)
//        solveCt = enterSolos();
//    repaint();
}

bool Sudoku::checkPairsRow(int row)
{
    const int Base = row * N;
    int IdxA = 0;
    int IdxB = 0;
    for (int i = 0; i < N-1; i++)
    {
        IdxA = getIdxxRow(i, row);
        if (matchedPairs[IdxA].get() == nullptr)
            continue;
        for (int j = i + 1; j < N; j++)
        {
            IdxB = getIdxxRow(j, row);
            if (matchedPairs[IdxB].get() == nullptr)
                continue;
            if (*matchedPairs[IdxA] == *matchedPairs[IdxB])
            {
                DBG("Matched Pair (" << matchedPairs[IdxA]->get1st() << "," << matchedPairs[IdxA]->get2nd() << ") in Row " << row + 1);
                return true;
                //for (int Idx = 0; Idx < N; Idx++)
                //{
                //    if (!bnCells[Base + Idx]->isUnknown() || bnCells[Base + Idx]->getCurrentValue() != 0)
                //        continue;
                //    if (Base + Idx == IdxA || Base + Idx == IdxB)
                //        continue;
                //    if (bnCells[Base + Idx]->getNote(matchedPairs[IdxA]->get1st()))
                //    {
                //        bnCells[Base + Idx]->clearNote(matchedPairs[IdxA]->get1st());
                //        DBG("Row clear " << matchedPairs[IdxA]->get1st() << " in " << row+1 << "," << Idx+1);
                //    }
                //    if (bnCells[Base + Idx]->getNote(matchedPairs[IdxA]->get2nd()))
                //    {
                //        bnCells[Base + Idx]->clearNote(matchedPairs[IdxA]->get2nd());
                //        DBG("Row clear " << matchedPairs[IdxA]->get2nd() << " in " << row+1 << "," << Idx+1);
                //    }
                //}
            }
        }
    }
    return false;
}

bool Sudoku::checkPairsCol(int col)
{
    int IdxA = 0;
    int IdxB = 0;
    for (int i = 0; i < N - 1; i++)
    {
        IdxA = getIdxxCol(i, col);
        if (matchedPairs[IdxA].get() == nullptr)
            continue;
        for (int j = i + 1; j < N; j++)
        {
            IdxB = getIdxxCol(j, col);
            if (matchedPairs[IdxB].get() == nullptr)
                continue;
            if (*matchedPairs[IdxA] == *matchedPairs[IdxB])
            {
                DBG("Matched Pair (" << matchedPairs[IdxA]->get1st() << "," << matchedPairs[IdxA]->get2nd() << ") in Col " << col + 1);
                return true;
                //for (int Idx = 0; Idx < N; Idx++)
                //{
                //    if (!bnCells[Base + (Idx * N)]->isUnknown() || bnCells[Base + (Idx * N)]->getCurrentValue() != 0)
                //        continue;
                //    if (Base + (Idx * N) == IdxA || Base + (Idx * N) == IdxB)
                //        continue;
                //    if (bnCells[Base + (Idx * N)]->getNote(matchedPairs[IdxA]->get1st()))
                //    {
                //        bnCells[Base + (Idx * N)]->clearNote(matchedPairs[IdxA]->get1st());
                //        DBG("Col clear " << matchedPairs[IdxA]->get1st() << " in " << Idx+1 << "," << col+1);
                //    }
                //    if (bnCells[Base + Idx]->getNote(matchedPairs[IdxA]->get2nd()))
                //    {
                //        bnCells[Base + Idx]->clearNote(matchedPairs[IdxA]->get2nd());
                //        DBG("Col clear " << matchedPairs[IdxA]->get2nd() << " in " << Idx+1 << "," << col+1);
                //    }
                //}
            }
        }
    }
    return false;
}

bool Sudoku::checkPairsSquare(int square)
{
    int IdxA = 0;
    int IdxB = 0;
    for (int i = 0; i < N - 1; i++)
    {
        IdxA = getIdxxSqr(i, square);
        if (matchedPairs[IdxA].get() == nullptr)
            continue;
        for (int j = i + 1; j < N; j++)
        {
            IdxB = getIdxxSqr(j, square);
            if (matchedPairs[IdxB].get() == nullptr)
                continue;
            if (*matchedPairs[IdxA] == *matchedPairs[IdxB])
            {
                DBG("Matched Pair (" << matchedPairs[IdxA]->get1st() << "," << matchedPairs[IdxA]->get2nd() << ") in Square " << square + 1);
                return true;

            }
        }
    }
    return false;
}

void Sudoku::onlyInRow(int row)
{
    int     RowBase = row * N;
    int     Idx = 0;

    for (int i = 0; i < N; i++)
    {
        int ct = 0;
        for (int cell = 0; cell < N; cell++)
        {
            if (!bnCells[RowBase + cell]->isUnknown())
                continue;
            if (bnCells[RowBase + cell]->getCurrentValue() != 0)
                continue;
            bool hit = bnCells[RowBase + cell]->getNote(i + 1);
            if (hit)
            {
                Idx = RowBase + cell;
                ct++;
            }
        }
        if (ct == 1)
        {
            CurrentCell = Idx;
            CurrentRow = CurrentCell / N;
            CurrentCol = CurrentCell % N;
            handleNumberEntry(i+1);

            DBG("OnlyInRow " << i + 1 << " in " << (Idx / N) + 1 << "," << (Idx % N) + 1);
        }
    }
}
void Sudoku::onlyInCol(int col)
{
    const int     Col = col;
    int     Idx = 0;

    for (int i = 0; i < N; i++)
    {
        int ct = 0;
        for (int cell = 0; cell < N; cell++)
        {
            if (!bnCells[Col + (cell * N)]->isUnknown())
                continue;
            if (bnCells[Col + (cell * N)]->getCurrentValue() != 0)
                continue;
            bool hit = bnCells[Col + (cell * N)]->getNote(i + 1);
            if (hit)
            {
                Idx = Col + (cell * N);
                ct++;
            }
        }
        if (ct == 1)
        {
            CurrentCell = Idx;
            CurrentRow = CurrentCell / N;
            CurrentCol = CurrentCell % N;
            handleNumberEntry(i + 1);

            DBG("OnlyInCol " << i + 1 << " in " << (Idx / N) + 1 << "," << (Idx % N) + 1);
        }
    }
}
void Sudoku::onlyInSquare(int square)
{
    int topLeftR = (square / NS) * NS;
    int topLeftC = (square % NS) * NS;
    int Idx = 0;

    for (int number = 0; number < N; number++)
    {
        //  check each number
        int ct = 0;
        for (int r = 0; r < NS; r++)
        {
            for (int c = 0; c < NS; c++)
            {
                int Ndx = ((topLeftR + r) * N) + topLeftC + c;
                if (!bnCells[Ndx]->isUnknown())
                    continue;
                if (bnCells[Ndx]->getCurrentValue() != 0)
                    continue;
                bool hit = bnCells[Ndx]->getNote(number + 1);
                if (hit)
                {
                    Idx = Ndx;
                    ct++;
                }
            }
        }
        if (ct == 1)
        {
            CurrentCell = Idx;
            CurrentRow = CurrentCell / N;
            CurrentCol = CurrentCell % N;
            handleNumberEntry(number + 1);

            DBG("OnlyInSqu " << square << " : " << number + 1 << " in " << CurrentRow + 1 << "," << CurrentCol + 1);
        }
    }
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

int Sudoku::enterSolos()
{
    int entered = 0;
    for (int cell = 0; cell < N * N; cell++)
    {
        if (!bnCells[cell]->isUnknown() || bnCells[cell]->getCurrentValue() != 0)
            continue;
        if (bnCells[cell]->countNotes() == 1)
        {
            int     number = 0;
            for (int i = 0; i < N; i++)
            {
                if (bnCells[cell]->getNote(i + 1))
                {
                    number = i + 1;
                    break;
                }
            }
            CurrentCell = cell;
            CurrentRow = CurrentCell / N;
            CurrentCol = CurrentCell % N;
            handleNumberEntry(number);
            DBG("Enter Solo " << number << " in " << CurrentRow + 1 << "," << CurrentCol + 1);
            entered++;
        }
    }
    repaint();
    return entered;
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

Cell* Sudoku::getCellxCol(int x, int Col)
{
    Cell* reply = nullptr;
    int     Idx;

    if (x < N && x >= 0)    //  Valid index
    {
        Idx = (x * N) + Col;
        reply = bnCells[Idx].get();
    }
    return reply;
}

Cell* Sudoku::getCellxRow(int x, int Row)
{
    Cell* reply = nullptr;
    int     Idx;

    if (x < N && x >= 0)    //  Valid index
    {
        Idx = (Row * N) + x;
        reply = bnCells[Idx].get();
    }
    return reply;
}

Cell* Sudoku::getCellxSqr(int x, int Square)
{
    Cell*   reply = nullptr;
    int     Idx;

    if (x < N && x >= 0)    //  Valid index
    {
        //int topLeftR = (square / NS) * NS;
        //int topLeftC = (square % NS) * NS;
        //Idx = (topLeftR * N) + topLeftC;
        Idx = SqIdx[Square];
        Idx += (x / NS) * N;
        Idx += (x % NS);
    }
    DBG("Square " << Square << " [" << x << "] is Index " << Idx);
    return reply;
}
int Sudoku::getIdxxCol(int x, int Col)
{
    return (x * N) + Col;
}

int Sudoku::getIdxxRow(int x, int Row)
{
    return (Row * N) + x;
}

int Sudoku::getIdxxSqr(int x, int Square)
{
    int Idx = SqIdx[Square];
    Idx += (x / NS) * N;
    Idx += (x % NS);
    return Idx;
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
    label.removeFromTop(size / 5);
    label.removeFromBottom(size / 5);
    label.removeFromLeft(size / 5);
    bnEnter->setBounds(label.removeFromLeft(size + (size / 3)).reduced(4));
    label.removeFromLeft(size / 5);
    bnLoad->setBounds(label.removeFromLeft(size + (size / 3)).reduced(4));
    label.removeFromLeft(size / 5);
    bnSave->setBounds(label.removeFromLeft(size + (size / 3)).reduced(4));
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
    auto buts = bounds;
    buts = buts.removeFromBottom(size);
    buts.removeFromTop(size / 5);
    buts.removeFromBottom(size / 5);
    buts.removeFromLeft(size);
    auto quit = buts.removeFromLeft(size * 2).reduced(4);
    bnQuit->setBounds(quit);
    buts.removeFromLeft(size);
    buts.removeFromLeft(size / 2);
    auto check = buts.removeFromLeft(size * 2);
    bnCheck->setBounds(check.reduced(4));
    buts.removeFromLeft(size);
    buts.removeFromLeft(size / 2);
    auto notes = buts.removeFromLeft(size * 2);
    bnNotes->setBounds(notes.reduced(4));
    buts.removeFromLeft(size / 2);
    auto brst = buts.removeFromLeft(size * 3 / 2);
    bnReset->setBounds(brst.reduced(4));
    buts.removeFromLeft(size / 2);
    auto bnew = buts.removeFromLeft(size * 3 / 2);
    bnNew->setBounds(bnew.reduced(4));

    area = bounds;
    area.removeFromTop(size * 5);
    area.removeFromLeft(size * 11);
    cbCellHilight->setBounds(area.removeFromTop(size / 2));
    cbChecking->setBounds(area.removeFromTop(size / 2));
    cbErrorHilight->setBounds(area.removeFromTop(size / 2));
    cbNotesTidy->setBounds(area.removeFromTop(size / 2));
    cbNotesToggle->setBounds(area.removeFromTop(size / 2));
    cbShowPossibles->setBounds(area.removeFromTop(size / 2));
    cbShowPairs->setBounds(area.removeFromTop(size / 2));
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
    cbNotesToggle.reset(new ToggleButton("right click toggles notes"));
    addAndMakeVisible(cbNotesToggle.get());
    cbNotesToggle->addListener(this);
    cbNotesToggle->setColour(ToggleButton::textColourId, Colours::black);
    cbNotesToggle->setColour(ToggleButton::tickColourId, Colours::black);
    //
    cbShowPossibles.reset(new ToggleButton("find possibles"));
    addAndMakeVisible(cbShowPossibles.get());
    cbShowPossibles->addListener(this);
    cbShowPossibles->setColour(ToggleButton::textColourId, Colours::black);
    cbShowPossibles->setColour(ToggleButton::tickColourId, Colours::black);
    //
    cbShowPairs.reset(new ToggleButton("find pairs"));
    addAndMakeVisible(cbShowPairs.get());
    cbShowPairs->addListener(this);
    cbShowPairs->setColour(ToggleButton::textColourId, Colours::black);
    cbShowPairs->setColour(ToggleButton::tickColourId, Colours::black);
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
    bnLoad.reset(new TextButton("Load"));
    addAndMakeVisible(bnLoad.get());
    bnLoad->setColour(TextButton::buttonOnColourId, Colours::red);
    bnLoad->setColour(TextButton::buttonColourId, Colours::green);
    bnLoad->setVisible(false);
    bnLoad->addListener(this);
    //
    bnSave.reset(new TextButton("Save"));
    addAndMakeVisible(bnSave.get());
    bnSave->setColour(TextButton::buttonOnColourId, Colours::red);
    bnSave->setColour(TextButton::buttonColourId, Colours::green);
    bnSave->setVisible(false);
    bnSave->addListener(this);
    //
    bnEnter.reset(new TextButton("Enter"));
    addAndMakeVisible(bnEnter.get());
    bnEnter->setColour(TextButton::buttonOnColourId, Colours::red);
    bnEnter->setColour(TextButton::buttonColourId, Colours::green);
    bnEnter->setClickingTogglesState(true);
    bnEnter->addListener(this);
    //
    bnReset.reset(new TextButton("Reset"));
    addAndMakeVisible(bnReset.get());
    bnReset->setColour(TextButton::buttonOnColourId, Colours::red);
    bnReset->setColour(TextButton::buttonColourId, Colours::green);
    bnReset->addListener(this);

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


