/*
  ==============================================================================

    IniFile.h
    Created: 7 Feb 2022 10:22:18am
    Author:  godfrey.small

  ==============================================================================
*/

#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include "JuceHeader.h"

#pragma pack(push)
#pragma pack(1)

struct RegItem
{
    std::string Name;
    std::string Value;
};

using namespace juce;

#pragma pack(pop)


class IniFile
{
public:
    IniFile(std::string);
    ~IniFile();

    std::string Read(std::string name, std::string Default);
    void SetReg(std::string name, std::string Value);
    void WriteAll();
private:
    void ReadAll();
private:        //  Data
    File        Base;
    bool        OutOfDate;
    std::string             IfileName;
    std::unique_ptr<FileInputStream>   Ifile;
    std::unique_ptr<FileOutputStream>  Ofile;
    std::unique_ptr <std::vector<RegItem>>   vItems;
};