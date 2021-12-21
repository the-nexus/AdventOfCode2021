#pragma once

#include "Common/Challenge.h"

class CChallenge_21 : public CChallenge
{
private:
    virtual EErrorCode SetUp_FirstPart() override { return SetUp_Common(); }
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override { return EErrorCode::Success; }

    virtual EErrorCode SetUp_SecondPart() override { return SetUp_Common(); }
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override { return EErrorCode::Success; }

    EErrorCode SetUp_Common();

    void DoQuantumDieRoll(
        std::vector<std::pair<int, int>> const& diceResultProbabilities,
        unsigned long long const universeCount,
        bool const isFirstPlayerActive,
        int const activePlayerPoints,
        int const activePlayerPos,
        int const otherPlayerPoints,
        int const otherPlayerPos,
        unsigned long long& outFirstPlayerWinCount,
        unsigned long long& outSecondPlayerWinCount
    );

    static std::string const sm_inputFilePath;
    int m_firstPlayerStartingPosition = 0;
    int m_secondPlayerStartingPosition = 0;
};
