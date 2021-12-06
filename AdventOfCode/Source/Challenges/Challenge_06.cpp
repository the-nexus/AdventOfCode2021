#include "Challenge_06.h"
#include "../Helpers/FileHelpers.h"



////////////////////////////////////////////////////////////////////////////////////////////////////
std::string const CChallenge_06::sm_inputFilePath = "Inputs/Input_Challenge_06.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_06::SetUp_FirstPart()
{
    return SetUp_Common();
}

EErrorCode CChallenge_06::Run_FirstPart()
{
    int const totalDays = 80;
    ReproduceFishesForDays(totalDays);
    std::cout << "Fish count after " << totalDays << " days: " << GetTotalFishCount() << std::endl;
    return EErrorCode::Success;
}

EErrorCode CChallenge_06::CleanUp_FirstPart()
{
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_06::SetUp_SecondPart()
{
    return SetUp_Common();
}

EErrorCode CChallenge_06::Run_SecondPart()
{
    int const totalDays = 256;
    ReproduceFishesForDays(totalDays);
    std::cout << "Fish count after " << totalDays << " days: " << GetTotalFishCount() << std::endl;
    return EErrorCode::Success;
}

EErrorCode CChallenge_06::CleanUp_SecondPart()
{
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_06::SetUp_Common()
{
    std::string line;
    EErrorCode const readErrorCode = FileHelper::ReadFirstLine(sm_inputFilePath, line);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    for (int day = 0; day <= NEW_FISH_START_DAY; ++day)
    {
        m_fishes[day] = 0;
    }

    std::vector<std::string> fishStrings;
    FileHelper::SplitLine(line, ",", fishStrings);

    for (std::string const& fishStr : fishStrings)
    {
        int const fishDays = atoi(fishStr.c_str());
        ++m_fishes[fishDays];
    }

    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
void CChallenge_06::ReproduceFishesForDays(int const totalDays)
{
    FishArray nextDayFishes;
    for (int currentDay = 0; currentDay < totalDays; ++currentDay)
    {
        GetNextDayReproducedFishes(nextDayFishes);

        std::copy(std::begin(nextDayFishes), std::end(nextDayFishes), std::begin(m_fishes));
    }
}

void CChallenge_06::GetNextDayReproducedFishes(FishArray& nextDayFishes) const
{
    for (int day = 0; day <= NEW_FISH_START_DAY; ++day)
    {
        nextDayFishes[day] = 0;
    }

    for (int day = 0; day <= NEW_FISH_START_DAY; ++day)
    {
        if (day == 0)
        {
            nextDayFishes[OLD_FISH_START_DAY] += m_fishes[0];
            nextDayFishes[NEW_FISH_START_DAY] += m_fishes[0];
        }
        else
        {
            nextDayFishes[day - 1] += m_fishes[day];
        }
    }
}

unsigned long long CChallenge_06::GetTotalFishCount() const
{
    unsigned long long fishCount = 0;
    for (int day = 0; day <= NEW_FISH_START_DAY; ++day)
    {
        fishCount += m_fishes[day];
    }
    return fishCount;
}
