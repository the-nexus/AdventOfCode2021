#pragma once

#include "Common/Challenge.h"

class CChallenge_17 : public CChallenge
{
private:
    virtual EErrorCode SetUp_FirstPart() override { return SetUp_Common(); }
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override { return EErrorCode::Success; }

    virtual EErrorCode SetUp_SecondPart() override { return SetUp_Common(); }
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override { return EErrorCode::Success; }

    EErrorCode SetUp_Common();

    int m_targetMinX = 0;
    int m_targetMaxX = 0;
    int m_targetMinY = 0;
    int m_targetMaxY = 0;

    static std::string const sm_inputFilePath;
};
