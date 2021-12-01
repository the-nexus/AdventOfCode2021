#include "Challenge_01.h"
#include "../Helpers/FileHelpers.h"



////////////////////////////////////////////////////////////////////////////////////////////////////
std::string const CChallenge_01::sm_inputFilePath = "Inputs/Input_Challenge_01.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_01::SetUp_FirstPart()
{
    std::vector<std::string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_depths.reserve(lines.size());
    for (std::string const& line : lines)
    {
        int const depth = atoi(line.c_str());
        m_depths.push_back(depth);
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_01::Run_FirstPart()
{
    unsigned int largerMeasurementCount = 0;
    if (!m_depths.empty())
    {
        int lastDepth = m_depths[0];
        for (unsigned int idx = 1; idx < m_depths.size(); ++idx)
        {
            if (m_depths[idx] > lastDepth)
            {
                ++largerMeasurementCount;
            }

            lastDepth = m_depths[idx];
        }
    }

    std::cout << "Larger measurement count: " << largerMeasurementCount << std::endl;
    return EErrorCode::Success;
}

EErrorCode CChallenge_01::CleanUp_FirstPart()
{
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_01::SetUp_SecondPart()
{
    std::vector<std::string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_depths.reserve(lines.size());
    for (std::string const& line : lines)
    {
        int const depth = atoi(line.c_str());
        m_depths.push_back(depth);
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_01::Run_SecondPart()
{
    unsigned int const windowSize = 3;
    unsigned int const depthSumCount = static_cast<unsigned int>(m_depths.size()) - (windowSize - 1);

    std::vector<int> depthSums;
    depthSums.reserve(depthSumCount);

    for (unsigned int windowStartIdx = 0; windowStartIdx < depthSumCount; ++windowStartIdx)
    {
        int depthSum = 0;
        for (unsigned int idx = windowStartIdx; idx < windowStartIdx + windowSize; ++idx)
        {
            depthSum += m_depths[idx];
        }

        depthSums.push_back(depthSum);
    }

    unsigned int largerMeasurementSumCount = 0;
    if (!depthSums.empty())
    {
        int lastDepthSum = depthSums[0];
        for (unsigned int idx = 1; idx < depthSums.size(); ++idx)
        {
            if (depthSums[idx] > lastDepthSum)
            {
                ++largerMeasurementSumCount;
            }

            lastDepthSum = depthSums[idx];
        }
    }

    std::cout << "Larger measurement sum count: " << largerMeasurementSumCount << std::endl;
    return EErrorCode::Success;
}

EErrorCode CChallenge_01::CleanUp_SecondPart()
{
    return EErrorCode::Success;
}
