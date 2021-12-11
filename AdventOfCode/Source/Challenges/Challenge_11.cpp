#include "Challenge_11.h"
#include "../Helpers/FileHelpers.h"



////////////////////////////////////////////////////////////////////////////////////////////////////
std::string const CChallenge_11::sm_inputFilePath = "Inputs/Input_Challenge_11.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_11::Run_FirstPart()
{
    size_t const lastLineIdx = m_lineCount - 1;
    size_t const lastColumnIdx = m_columnCount - 1;

    int flashCount = 0;
    for (size_t stepIdx = 0; stepIdx < 100; ++stepIdx)
    {
        // Increase everything by 1
        for (size_t lineIdx = 1; lineIdx < lastLineIdx; ++lineIdx)
        {
            for (size_t columnIdx = 1; columnIdx < lastColumnIdx; ++columnIdx)
            {
                ++m_energyLevels[lineIdx][columnIdx];
                m_energyLevelsChanged[lineIdx][columnIdx] = true;
            }
        }

        // Increase energy levels around flashing energy levels
        bool isStable;
        do 
        {
            isStable = true;

            for (size_t lineIdx = 1; lineIdx < lastLineIdx; ++lineIdx)
            {
                for (size_t columnIdx = 1; columnIdx < lastColumnIdx; ++columnIdx)
                {
                    if (m_energyLevelsChanged[lineIdx][columnIdx])
                    {
                        if (m_energyLevels[lineIdx][columnIdx] > 9)
                        {
                            for (int lineOffset = -1; lineOffset <= 1; ++lineOffset)
                            {
                                for (int columnOffset = -1; columnOffset <= 1; ++columnOffset)
                                {
                                    if (lineOffset != 0 || columnOffset != 0)
                                    {
                                        size_t const subLineIdx = lineIdx + lineOffset;
                                        size_t const subColumnIdx = columnIdx + columnOffset;
                                        if (m_energyLevels[subLineIdx][subColumnIdx] <= 9)
                                        {
                                            ++m_energyLevels[subLineIdx][subColumnIdx];
                                            m_energyLevelsChanged[subLineIdx][subColumnIdx] = true;
                                            isStable = false;
                                        }
                                    }
                                }
                            }
                        }

                        m_energyLevelsChanged[lineIdx][columnIdx] = false;
                    }
                }
            }
        }
        while (!isStable);

        // Reset flashers to 0
        for (size_t lineIdx = 1; lineIdx < lastLineIdx; ++lineIdx)
        {
            for (size_t columnIdx = 1; columnIdx < lastColumnIdx; ++columnIdx)
            {
                if (m_energyLevels[lineIdx][columnIdx] > 9)
                {
                    m_energyLevels[lineIdx][columnIdx] = 0;
                    ++flashCount;
                }
            }
        }
    }

    std::cout << "Number of flashes after 100 steps: " << flashCount << std::endl;
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_11::Run_SecondPart()
{
    size_t const lastLineIdx = m_lineCount - 1;
    size_t const lastColumnIdx = m_columnCount - 1;
    size_t const totalElemCount = (m_lineCount - 2) * (m_columnCount - 2);

    size_t stepCount = 0;
    while (stepCount != -1)
    {
        ++stepCount;

        // Increase everything by 1
        for (size_t lineIdx = 1; lineIdx < lastLineIdx; ++lineIdx)
        {
            for (size_t columnIdx = 1; columnIdx < lastColumnIdx; ++columnIdx)
            {
                ++m_energyLevels[lineIdx][columnIdx];
                m_energyLevelsChanged[lineIdx][columnIdx] = true;
            }
        }

        // Increase energy levels around flashing energy levels
        bool isStable;
        do
        {
            isStable = true;

            for (size_t lineIdx = 1; lineIdx < lastLineIdx; ++lineIdx)
            {
                for (size_t columnIdx = 1; columnIdx < lastColumnIdx; ++columnIdx)
                {
                    if (m_energyLevelsChanged[lineIdx][columnIdx])
                    {
                        if (m_energyLevels[lineIdx][columnIdx] > 9)
                        {
                            for (int lineOffset = -1; lineOffset <= 1; ++lineOffset)
                            {
                                for (int columnOffset = -1; columnOffset <= 1; ++columnOffset)
                                {
                                    if (lineOffset != 0 || columnOffset != 0)
                                    {
                                        size_t const subLineIdx = lineIdx + lineOffset;
                                        size_t const subColumnIdx = columnIdx + columnOffset;
                                        if (m_energyLevels[subLineIdx][subColumnIdx] <= 9)
                                        {
                                            ++m_energyLevels[subLineIdx][subColumnIdx];
                                            m_energyLevelsChanged[subLineIdx][subColumnIdx] = true;
                                            isStable = false;
                                        }
                                    }
                                }
                            }
                        }

                        m_energyLevelsChanged[lineIdx][columnIdx] = false;
                    }
                }
            }
        }
        while (!isStable);

        // Reset flashers to 0 and count them
        size_t flashCount = 0;
        for (size_t lineIdx = 1; lineIdx < lastLineIdx; ++lineIdx)
        {
            for (size_t columnIdx = 1; columnIdx < lastColumnIdx; ++columnIdx)
            {
                if (m_energyLevels[lineIdx][columnIdx] > 9)
                {
                    m_energyLevels[lineIdx][columnIdx] = 0;
                    ++flashCount;
                }
            }
        }

        if (flashCount == totalElemCount)
        {
            break;
        }
    }

    std::cout << "All flashing step: " << stepCount << std::endl;
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_11::SetUp_Common()
{
    std::vector<std::string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_lineCount = lines.size() + 2;
    m_columnCount = lines[0].size() + 2;

    m_energyLevels = new int* [m_lineCount];
    m_energyLevelsChanged = new bool* [m_columnCount];
    for (size_t lineIdx = 0; lineIdx < m_lineCount; ++lineIdx)
    {
        m_energyLevels[lineIdx] = new int[m_columnCount];
        m_energyLevelsChanged[lineIdx] = new bool[m_columnCount];
        for (size_t columnIdx = 0; columnIdx < m_columnCount; ++columnIdx)
        {
            if (lineIdx == 0 || 
                lineIdx == m_lineCount - 1 ||
                columnIdx == 0 || 
                columnIdx == m_columnCount - 1)
            {
                m_energyLevels[lineIdx][columnIdx] = -1;
            }
            else
            {
                int const energy = static_cast<int>(lines[lineIdx - 1][columnIdx - 1] - '0');
                m_energyLevels[lineIdx][columnIdx] = energy;
            }

            m_energyLevelsChanged[lineIdx][columnIdx] = false;
        }
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_11::CleanUp_Common()
{
    for (size_t lineIdx = 0; lineIdx < m_lineCount; ++lineIdx)
    {
        delete m_energyLevels[lineIdx];
    }
    delete m_energyLevels;

    for (size_t lineIdx = 0; lineIdx < m_lineCount; ++lineIdx)
    {
        delete m_energyLevelsChanged[lineIdx];
    }
    delete m_energyLevelsChanged;

    return EErrorCode::Success;
}
