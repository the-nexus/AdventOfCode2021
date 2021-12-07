#pragma once

#include "Common/Challenge.h"

class CChallenge_07 : public CChallenge
{
private:
    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    static std::string const sm_inputFilePath;
    int minCrab = INT_MAX;
    int maxCrab = 0;
    std::vector<int> m_crabs;
};
