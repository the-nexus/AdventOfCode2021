#pragma once

#include "Common/Challenge.h"

class CChallenge_13 : public CChallenge
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

    void FoldX(int const foldPosX);
    void FoldY(int const foldPosY);

    static std::string const sm_inputFilePath;
    std::vector<std::pair<char, int>> m_instructions;
    bool** m_page = nullptr;
    int m_startingSizeX = 0;
    int m_startingSizeY = 0;
    int m_foldedSizeX = 0;
    int m_foldedSizeY = 0;
};
