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

    grid.reset (new SudokuGrid());
    for (int i = 0; i < N * N; i++)
    {
        Squares[i].reset(new SudokuButton());
        addAndMakeVisible(Squares[i].get());
        int val = grid->getValue(i / N, i % N);
        String number = String(val);
        Squares[i]->setLookAndFeel((LookAndFeel *) &LnF);
        Squares[i]->setButtonText(number);
        Squares[i]->setClickingTogglesState(true);
        Squares[i]->setColour(TextButton::buttonOnColourId, Colours::darkgrey);
        Squares[i]->setColour(TextButton::buttonColourId, Colours::grey);
        Squares[i]->setColour(TextButton::textColourOnId, Colours::black);
        Squares[i]->setColour(TextButton::textColourOffId, Colours::black);
        Squares[i]->addListener(this);
        Squares[i]->setRadioGroupId(1);

    }
    bnQuit.reset(new TextButton("Quit"));
    addAndMakeVisible(bnQuit.get());
    bnQuit->setColour(TextButton::buttonOnColourId, Colours::red);
    bnQuit->setColour(TextButton::buttonColourId, Colours::green);
    bnQuit->addListener(this);

    IniReg.reset(new IniFile(INIFILENAME));
    String iniHeight = IniReg->Read(SCREENHEIGHT, "600");
    String iniWidth = IniReg->Read(SCREENWIDTH, "400");
    Height = atoi(iniHeight.toStdString().c_str());
    Width = atoi(iniWidth.toStdString().c_str());
    setSize(Width, Height);
}

Sudoku::~Sudoku()
{
    char    buff[8];
    auto bounds = getBounds();
    Width = bounds.getWidth();
    Height = bounds.getHeight();
    IniReg.reset(new IniFile(INIFILENAME));
    IniReg->SetReg(SCREENHEIGHT, _itoa(Height, buff, 10));
    IniReg->SetReg(SCREENWIDTH, _itoa(Width, buff, 10));
    DBG("Write Width = " << Width << ", height = " << Height);
    IniReg.reset();
}

void Sudoku::paint (juce::Graphics& g)
{
    g.fillAll ((juce::Colours::lightgrey));   // clear the background

    g.setColour (juce::Colours::black);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    auto bounds = getBounds();

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
    g.drawRect (box2, 4);
}

void Sudoku::buttonClicked(Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == bnQuit.get())
        handleQuit();
    else
    {
        for (int i=0 ; i<N*N ; i++)
            if (buttonThatWasClicked == Squares[i].get())
            {
                CurrentSquare = i;
                handleCurrentSquare();
                return;
            }
    }
}

void Sudoku::handleCurrentSquare()
{
    DBG (CurrentSquare << "  " << CurrentSquare / N << " " << CurrentSquare % N);
}
void Sudoku::handleQuit()
{
    JUCEApplication::getInstance()->systemRequestedQuit();
}

void Sudoku::resized()
{
    auto bounds = getBounds();
    
    int     size;
    int sizeW = bounds.getWidth() / (N + 2);
    int sizeH = bounds.getHeight() / (N + 2);
    if (sizeW > sizeH)
        size = sizeH;
    else
        size = sizeW;
    auto area = bounds;
    area.removeFromTop(size);
    area.removeFromLeft(size);
    int idx = 0;
    for (int r = 0; r < N; r++)
    {
        auto row = area.removeFromTop(size);
        for (int c = 0; c < N; c++)
        {
            Squares[idx++]->setBounds(row.removeFromLeft(size).reduced(2));
        }
    }
    auto quit = area.removeFromLeft(size * 2).reduced(4);
    quit.removeFromTop(size / 5);
    quit.removeFromBottom(size / 5);
    bnQuit->setBounds(quit);

}


