#pragma once

#include "Common/Challenge.h"

class CChallenge_11 : public CChallenge
{
private:
    virtual EErrorCode SetUp_FirstPart() override { return SetUp_Common(); }
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override { return CleanUp_Common(); }

    virtual EErrorCode SetUp_SecondPart() override { return SetUp_Common(); }
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override { return CleanUp_Common(); }

    EErrorCode SetUp_Common();
    EErrorCode CleanUp_Common();

    static std::string const sm_inputFilePath;

    int** m_energyLevels = nullptr;
    bool** m_energyLevelsChanged = nullptr;

    size_t m_lineCount = 0;
    size_t m_columnCount = 0;
};
