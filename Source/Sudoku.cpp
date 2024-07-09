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
    grid.reset (new SudokuGrid());
    for (int i = 0; i < N * N; i++)
    {
        Squares[i].reset(new TextButton());
        addAndMakeVisible(Squares[i].get());
        int val = grid->getValue(i / N, i % N);
        String number = String(val);
        Squares[i]->setButtonText(number);
        Squares[i]->setClickingTogglesState(true);
        Squares[i]->setColour(TextButton::buttonOnColourId, Colours::darkgrey);
        Squares[i]->setColour(TextButton::buttonColourId, Colours::grey);
        Squares[i]->setColour(TextButton::textColourOnId, Colours::black);
        Squares[i]->setColour(TextButton::textColourOffId, Colours::black);
        Squares[i]->addListener(this);
        Squares[i]->setRadioGroupId(1);

    }
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
}


