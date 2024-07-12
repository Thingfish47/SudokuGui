/*
  ==============================================================================

    IniFile.cpp
    Created: 7 Feb 2022 10:22:18am
    Author:  godfrey.small

  ==============================================================================
*/

#include "IniFile.h"
#include "windows.h"
#include "juceheader.h"


IniFile::IniFile(std::string IniFileName)
{
    IfileName = IniFileName;
    Base = File::getSpecialLocation(File::userApplicationDataDirectory);
    std::string fullpath = Base.getFullPathName().toStdString() + IfileName;
    Base = *(new File(fullpath));
    ReadAll();
}

IniFile::~IniFile()
{
    if (OutOfDate)
        WriteAll();
    Ifile = nullptr;
    Ofile = nullptr;
    vItems = nullptr;
}

void IniFile::SetReg(std::string name, std::string value)
{
    std::string Value;
    std::unique_ptr<RegItem> R;

    for (std::vector<RegItem>::iterator it = vItems->begin(); it != vItems->end(); ++it)
    {
        if (it->Name.compare(name) == 0)    //  Match found
        {
            if (it->Value.compare(value) != 0)
            {
                it->Value = value;
                OutOfDate = true;
            }
            return;
        }
    }
    R.reset(new RegItem);
    R->Name = name;
    R->Value = value;
    vItems->push_back(*R);
    OutOfDate = true;
    return;
}

std::string IniFile::Read(std::string name, std::string default)
{
    std::string Value;
    std::unique_ptr<RegItem> R;

    for (std::vector<RegItem>::iterator it = vItems->begin(); it != vItems->end(); ++it)
    {
        if (it->Name.compare(name) == 0)    //  Match found
            return (it->Value);
    }
    R.reset(new RegItem);
    R->Name = name;
    R->Value = default;
    vItems->push_back(*R);
    return default;
}

void IniFile::ReadAll()
{
    std::unique_ptr<RegItem> R;

    vItems.reset (new std::vector<RegItem>);
    File Dpath = Base.getParentDirectory();
    Dpath.createDirectory();
    Ifile.reset(new FileInputStream(Base));
    if (!Ifile->openedOk())
    {
        DBG("Open failed " << GetLastError());
        return;
    }
    while (!Ifile->isExhausted())
    {
        R.reset(new RegItem);
        String str = Ifile->readNextLine();
        int Idx = str.indexOf(" , ");
        String field = str.substring(0, Idx);
        R->Name = field.trim().toStdString();
        field = str.substring(Idx + 3);
        R->Value = field.trim().toStdString();
        vItems->push_back(*R);
        DBG(str);
    }
    OutOfDate = false;

    Ifile = nullptr;
}

void IniFile::WriteAll()
{
    std::stringstream ss;

    if (!OutOfDate)
        return;
    File Dpath = Base.getParentDirectory();
    Dpath.createDirectory();
    Ofile.reset(new FileOutputStream(Base));
    if (Ofile->openedOk())
    {
        Ofile->setPosition(0);
        Ofile->truncate();
    }
    for (std::vector<RegItem>::iterator it = vItems->begin(); it != vItems->end(); ++it)
    {
        RegItem R = *it;
        ss << R.Name << " , " << R.Value;
        std::string s = ss.str();
        DBG("Write INI : " << s);
        s.append("\n");
        Ofile->writeText(s, false, false, nullptr);
        ss.str("");
    }
    Ofile->flush();
    Ofile = nullptr;
    OutOfDate = false;
}