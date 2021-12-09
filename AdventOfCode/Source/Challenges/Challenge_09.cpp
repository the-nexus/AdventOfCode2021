#include "Challenge_09.h"
#include "../Helpers/FileHelpers.h"
#include <algorithm>


////////////////////////////////////////////////////////////////////////////////////////////////////
std::string const CChallenge_09::sm_inputFilePath = "Inputs/Input_Challenge_09.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_09::Run_FirstPart()
{
    int riskLevelSum = 0;

    for (size_t lineIdx = 1; lineIdx < m_heightmapHeight - 1; ++lineIdx)
    {
        for (size_t columnIdx = 1; columnIdx < m_heightmapWidth - 1; ++columnIdx)
        {
            int const pointHeight = m_heightmap[lineIdx][columnIdx];
            if (pointHeight < m_heightmap[lineIdx - 1][columnIdx] &&
                pointHeight < m_heightmap[lineIdx + 1][columnIdx] &&
                pointHeight < m_heightmap[lineIdx][columnIdx - 1] &&
                pointHeight < m_heightmap[lineIdx][columnIdx + 1])
            {
                riskLevelSum += 1 + pointHeight;
            }
        }
    }

    std::cout << "Sum of low points risk levels: " << riskLevelSum << std::endl;
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_09::Run_SecondPart()
{
    // 0.1 = 1/10, which statistically is the probability of pulling a 0 randomly from 0-9 digits on an even distribution
    // Lets reserve that much space based on the number of elements in the heightmap for performance reasons
    std::vector<std::pair<size_t, size_t>> lowPoints;
    lowPoints.reserve(static_cast<size_t>(m_heightmapWidth * m_heightmapHeight * 0.1));

    for (size_t lineIdx = 1; lineIdx < m_heightmapHeight - 1; ++lineIdx)
    {
        for (size_t columnIdx = 1; columnIdx < m_heightmapWidth - 1; ++columnIdx)
        {
            int const pointHeight = m_heightmap[lineIdx][columnIdx];
            if (pointHeight < m_heightmap[lineIdx - 1][columnIdx] &&
                pointHeight < m_heightmap[lineIdx + 1][columnIdx] &&
                pointHeight < m_heightmap[lineIdx][columnIdx - 1] &&
                pointHeight < m_heightmap[lineIdx][columnIdx + 1])
            {
                lowPoints.emplace_back(lineIdx, columnIdx);
            }
        }
    }

    std::vector<int> basinSizes;
    basinSizes.reserve(lowPoints.size());

    for (std::pair<size_t, size_t> const& lowPoint : lowPoints)
    {
        if (m_heightmap[lowPoint.first][lowPoint.second] != -1)
        {
            basinSizes.push_back(CountAndMarkBasin(lowPoint.first, lowPoint.second));
        }
    }

    std::sort(basinSizes.begin(), basinSizes.end());

    int const multipliedSizes = basinSizes[basinSizes.size() - 1] * basinSizes[basinSizes.size() - 2] * basinSizes[basinSizes.size() - 3];
    std::cout << "Multiplied top 3 sizes: " << multipliedSizes << std::endl;
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_09::SetUp_Common()
{
    std::vector<std::string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_heightmapHeight = lines.size() + 2;
    m_heightmapWidth = lines[0].size() + 2;

    m_heightmap = new int*[m_heightmapHeight];
    for (size_t lineIdx = 0; lineIdx < m_heightmapHeight; ++lineIdx)
    {
        m_heightmap[lineIdx] = new int[m_heightmapWidth];
        for (size_t columnIdx = 0; columnIdx < m_heightmapWidth; ++columnIdx)
        {
            bool const isBorder = lineIdx == 0 || columnIdx == 0 || lineIdx == m_heightmapHeight - 1 || columnIdx == m_heightmapWidth - 1;
            m_heightmap[lineIdx][columnIdx] = isBorder ? 9 : static_cast<int>(lines[lineIdx - 1][columnIdx - 1] - '0');
        }
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_09::CleanUp_Common()
{
    for (size_t lineIdx = 0; lineIdx < m_heightmapHeight; ++lineIdx)
    {
        delete m_heightmap[lineIdx];
    }
    delete m_heightmap;

    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
int CChallenge_09::CountAndMarkBasin(size_t const lineIdx, size_t const colIdx)
{
    if (m_heightmap[lineIdx][colIdx] == -1 || m_heightmap[lineIdx][colIdx] == 9)
    {
        return 0;
    }
    else
    {
        m_heightmap[lineIdx][colIdx] = -1;
        return 1 +
            CountAndMarkBasin(lineIdx, colIdx - 1) +
            CountAndMarkBasin(lineIdx, colIdx + 1) + 
            CountAndMarkBasin(lineIdx - 1, colIdx) +
            CountAndMarkBasin(lineIdx + 1, colIdx);
    }
}
