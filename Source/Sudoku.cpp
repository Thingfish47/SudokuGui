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
    grid.reset (new SudokuGrid());
    for (int i = 0; i < N * N; i++)
    {
        int val = grid->getValue(i / N, i % N);
        bnCells[i].reset(new SudokuButton(val));
        bnCells[i]->setCurrentValue(val);
        addAndMakeVisible(bnCells[i].get());
        bnCells[i]->setLookAndFeel((LookAndFeel *) &LnF);
        //String txt = String(val);
        //Squares[i]->setButtonText(txt);
        bnCells[i]->setClickingTogglesState(true);
        bnCells[i]->setColour(TextButton::buttonOnColourId, Colours::darkgrey);
        bnCells[i]->setColour(TextButton::buttonColourId, Colours::grey);
        bnCells[i]->setColour(TextButton::textColourOnId, Colours::black);
        bnCells[i]->setColour(TextButton::textColourOffId, Colours::black);
        bnCells[i]->addListener(this);
        bnCells[i]->setRadioGroupId(1);
    }
    removeDigits(2);
    createButtons();
    lbStatus.reset(new Label("lbStatus"));
    lbStatus->setFont(Font(HEIGHT / (N + 2)));
    lbStatus->setColour(Label::textColourId, Colours::black);
    lbStatus->setJustificationType(Justification::centred);
    addAndMakeVisible(lbStatus.get());

    //IniReg.reset(new IniFile(INIFILENAME));
    //String iniHeight = IniReg->Read(SCREENHEIGHT, "600");
    //String iniWidth = IniReg->Read(SCREENWIDTH, "800");
    //Height = atoi(iniHeight.toStdString().c_str());
    //Width = atoi(iniWidth.toStdString().c_str());
    setSize(WIDTH, HEIGHT);
}

Sudoku::~Sudoku()
{
    //char    buff[8];
    //auto bounds = getBounds();
    //Width = bounds.getWidth();
    //Height = bounds.getHeight();
    //IniReg.reset(new IniFile(INIFILENAME));
    //IniReg->SetReg(SCREENHEIGHT, _itoa(Height, buff, 10));
    //IniReg->SetReg(SCREENWIDTH, _itoa(Width, buff, 10));
    //DBG("Write Width = " << Width << ", height = " << Height);
    //IniReg.reset();
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
    if (CurrentSquare < 0)
        return true;
    ModifierKeys mods = key.getModifiers();
    if (mods.isAnyModifierKeyDown() || mods.isAnyMouseButtonDown())
        return true;
    if (!bnCells[CurrentSquare]->isUnknown())
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
    else if (buttonThatWasClicked == bnClear.get())
        handleClear();
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
                CurrentSquare = i;
                handleCurrentSquare();
                return;
            }
    }
}


void Sudoku::removeDigits(int numToRemove)
{
    while (numToRemove > 0)
    {
        int cellId = rand() % (N * N);
        if (!bnCells[cellId]->isUnknown())
        {
            bnCells[cellId]->setUnknown();
            numToRemove--;
        }
    }
}

void Sudoku::handleCurrentSquare()
{
    DBG ("CurrentSquare " << CurrentSquare << "  " << CurrentSquare / N << " " << CurrentSquare % N);
}

void Sudoku::handleNumberEntry(int Number)
{
    if (CurrentSquare < 0)
        return;
    if (bnNotes->getToggleState() == false)
    {
        bnCells[CurrentSquare]->setCurrentValue(Number);
        if (solved())
            setSolved();
    }
    else
    {
        bnCells[CurrentSquare]->toggleNote(Number);
        repaint();
    }
}

bool Sudoku::solved()
{
    for (int i = 0; i < N * N; i++)
    {
        if (bnCells[i]->isUnknown())
            return false;
    }
    return true;
}

void Sudoku::setSolved()
{
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

void Sudoku::handleClear()
{
    if (bnNotes->getToggleState() == false)
        bnCells[CurrentSquare]->setUnknown();
}

void Sudoku::handleQuit()
{
    JUCEApplication::getInstance()->systemRequestedQuit();
}

void Sudoku::resized()
{
    // Get the current width and height
    auto bounds1 = getBounds();
    auto bounds = getBounds();
    DBG(bounds.getWidth() << " " << bounds.getHeight());
    bounds.setHeight(HEIGHT);
    bounds.setWidth(WIDTH);
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
    area.removeFromLeft(size * 5);
    auto notes = area.removeFromLeft(size * 2).reduced(4);
    notes.removeFromTop(size / 5);
    notes.removeFromBottom(size / 5);
    bnNotes->setBounds(notes);
}

void Sudoku::createButtons()
{
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
    bnOne.reset(new TextButton("1"));
    addAndMakeVisible(bnOne.get());
    bnOne->setColour(TextButton::buttonOnColourId, Colours::red);
    bnOne->setColour(TextButton::buttonColourId, Colours::palegreen);
    bnOne->setColour(TextButton::textColourOnId, Colours::black);
    bnOne->setColour(TextButton::textColourOffId, Colours::black);
    bnOne->addListener(this);
    //
    bnTwo.reset(new TextButton("2"));
    addAndMakeVisible(bnTwo.get());
    bnTwo->setColour(TextButton::buttonOnColourId, Colours::red);
    bnTwo->setColour(TextButton::buttonColourId, Colours::palegreen);
    bnTwo->setColour(TextButton::textColourOnId, Colours::black);
    bnTwo->setColour(TextButton::textColourOffId, Colours::black);
    bnTwo->addListener(this);
    //
    bnThree.reset(new TextButton("3"));
    addAndMakeVisible(bnThree.get());
    bnThree->setColour(TextButton::buttonOnColourId, Colours::red);
    bnThree->setColour(TextButton::buttonColourId, Colours::palegreen);
    bnThree->setColour(TextButton::textColourOnId, Colours::black);
    bnThree->setColour(TextButton::textColourOffId, Colours::black);
    bnThree->addListener(this);
    //
    bnFour.reset(new TextButton("4"));
    addAndMakeVisible(bnFour.get());
    bnFour->setColour(TextButton::buttonOnColourId, Colours::red);
    bnFour->setColour(TextButton::buttonColourId, Colours::palegreen);
    bnFour->setColour(TextButton::textColourOnId, Colours::black);
    bnFour->setColour(TextButton::textColourOffId, Colours::black);
    bnFour->addListener(this);
    //
    bnFive.reset(new TextButton("5"));
    addAndMakeVisible(bnFive.get());
    bnFive->setColour(TextButton::buttonOnColourId, Colours::red);
    bnFive->setColour(TextButton::buttonColourId, Colours::palegreen);
    bnFive->setColour(TextButton::textColourOnId, Colours::black);
    bnFive->setColour(TextButton::textColourOffId, Colours::black);
    bnFive->addListener(this);
    //
    bnSix.reset(new TextButton("6"));
    addAndMakeVisible(bnSix.get());
    bnSix->setColour(TextButton::buttonOnColourId, Colours::red);
    bnSix->setColour(TextButton::buttonColourId, Colours::palegreen);
    bnSix->setColour(TextButton::textColourOnId, Colours::black);
    bnSix->setColour(TextButton::textColourOffId, Colours::black);
    bnSix->addListener(this);
    //
    bnSeven.reset(new TextButton("7"));
    addAndMakeVisible(bnSeven.get());
    bnSeven->setColour(TextButton::buttonOnColourId, Colours::red);
    bnSeven->setColour(TextButton::buttonColourId, Colours::palegreen);
    bnSeven->setColour(TextButton::textColourOnId, Colours::black);
    bnSeven->setColour(TextButton::textColourOffId, Colours::black);
    bnSeven->addListener(this);
    //
    bnEight.reset(new TextButton("8"));
    addAndMakeVisible(bnEight.get());
    bnEight->setColour(TextButton::buttonOnColourId, Colours::red);
    bnEight->setColour(TextButton::buttonColourId, Colours::palegreen);
    bnEight->setColour(TextButton::textColourOnId, Colours::black);
    bnEight->setColour(TextButton::textColourOffId, Colours::black);
    bnEight->addListener(this);
    //
    bnNine.reset(new TextButton("9"));
    addAndMakeVisible(bnNine.get());
    bnNine->setColour(TextButton::buttonOnColourId, Colours::red);
    bnNine->setColour(TextButton::buttonColourId, Colours::palegreen);
    bnNine->setColour(TextButton::textColourOnId, Colours::black);
    bnNine->setColour(TextButton::textColourOffId, Colours::black);
    bnNine->addListener(this);
    //
    bnClear.reset(new TextButton("clr"));
    addAndMakeVisible(bnClear.get());
    bnClear->setColour(TextButton::buttonOnColourId, Colours::red);
    bnClear->setColour(TextButton::buttonColourId, Colours::palegreen);
    bnClear->setColour(TextButton::textColourOnId, Colours::black);
    bnClear->setColour(TextButton::textColourOffId, Colours::black);
    bnClear->addListener(this);
}


