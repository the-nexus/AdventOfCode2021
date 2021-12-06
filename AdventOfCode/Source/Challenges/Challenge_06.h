#pragma once

#include "Common/Challenge.h"

#define NEW_FISH_START_DAY 8
#define OLD_FISH_START_DAY 6

class CChallenge_06 : public CChallenge
{
private:
    using FishArray = unsigned long long [NEW_FISH_START_DAY + 1];

    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    EErrorCode SetUp_Common();

    void ReproduceFishesForDays(int const totalDays);
    void GetNextDayReproducedFishes(FishArray& nextDayFishes) const;
    unsigned long long GetTotalFishCount() const;

    static std::string const sm_inputFilePath;
    FishArray m_fishes;
};
