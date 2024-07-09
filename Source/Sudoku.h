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

using namespace juce;

#define     VERSION_LIT     "Ver 3.3.1"
#define     INIFILENAME     "\\Sudoku\\.ini"
#define     SCREENWIDTH "Width"
#define     SCREENHEIGHT "Height"

//==============================================================================
/*
*/
class Sudoku  : public juce::Component,
                public Button::Listener

{
public:
    Sudoku();
    ~Sudoku() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void buttonClicked(Button* buttonThatWasClicked) override;

private:
    std::unique_ptr<SudokuGrid>     grid;
    std::unique_ptr<IniFile>        IniReg;
    int                             Height;
    int                             Width;
    std::unique_ptr<TextButton>     Squares[N * N];
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sudoku)
};
