#include "Challenge_07.h"
#include "../Helpers/FileHelpers.h"
#include <algorithm>



////////////////////////////////////////////////////////////////////////////////////////////////////
std::string const CChallenge_07::sm_inputFilePath = "Inputs/Input_Challenge_07.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_07::SetUp_FirstPart()
{
    std::string line;
    EErrorCode const readErrorCode = FileHelper::ReadFirstLine(sm_inputFilePath, line);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    std::vector<std::string> crabStrings;
    FileHelper::SplitLine(line, ",", crabStrings);

    m_crabs.reserve(crabStrings.size());
    for (std::string const& crabStr : crabStrings)
    {
        m_crabs.push_back(atoi(crabStr.c_str()));
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_07::Run_FirstPart()
{
    std::sort(m_crabs.begin(), m_crabs.end());

    int const medianIdx = static_cast<int>(m_crabs.size()) / 2;
    int const medianPosition = m_crabs[medianIdx];

    int requiredFuel = 0;
    for (int const crabPosition : m_crabs)
    {
        requiredFuel += static_cast<int>(fabs(medianPosition - crabPosition));
    }

    std::cout << "Minimum amount of fuel required to align all crabs at position " << medianPosition << ": " << requiredFuel << std::endl;
    return EErrorCode::Success;
}

EErrorCode CChallenge_07::CleanUp_FirstPart()
{
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_07::SetUp_SecondPart()
{
    std::string line;
    EErrorCode const readErrorCode = FileHelper::ReadFirstLine(sm_inputFilePath, line);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    std::vector<std::string> crabStrings;
    FileHelper::SplitLine(line, ",", crabStrings);

    m_crabs.reserve(crabStrings.size());
    for (std::string const& crabStr : crabStrings)
    {
        m_crabs.push_back(atoi(crabStr.c_str()));
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_07::Run_SecondPart()
{
    float averagePosition = 0.f;
    for (int const crabPosition : m_crabs)
    {
        averagePosition += crabPosition;
    }

    averagePosition /= static_cast<float>(m_crabs.size());
    int const flooredAveragePosition = static_cast<int>(floorf(averagePosition));
    int const ceiledAveragePosition = static_cast<int>(ceilf(averagePosition));

    int flooredRequiredFuel = 0;
    int ceiledRequiredFuel = 0;

    for (int const crabPosition : m_crabs)
    {
        int const flooredStepCount = static_cast<int>(fabs(flooredAveragePosition - crabPosition));
        for (int step = 0; step < flooredStepCount; ++step)
        {
            flooredRequiredFuel += step + 1;
        }

        int const ceiledStepCount = static_cast<int>(fabs(ceiledAveragePosition - crabPosition));
        for (int step = 0; step < ceiledStepCount; ++step)
        {
            ceiledRequiredFuel += step + 1;
        }
    }

    bool const isFlooredSolutionMinRequiredFuel = flooredRequiredFuel < ceiledRequiredFuel;
    int const minRequiredFuel = isFlooredSolutionMinRequiredFuel ? flooredRequiredFuel : ceiledRequiredFuel;
    int const minPosition = isFlooredSolutionMinRequiredFuel ? flooredAveragePosition : ceiledAveragePosition;

    std::cout << "Minimum amount of fuel required to align all crabs at position " << minPosition << ": " << minRequiredFuel << std::endl;
    return EErrorCode::Success;
}

EErrorCode CChallenge_07::CleanUp_SecondPart()
{
    return EErrorCode::Success;
}
