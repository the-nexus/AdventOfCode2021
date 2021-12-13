#include "Challenge_13.h"
#include "../Helpers/FileHelpers.h"



////////////////////////////////////////////////////////////////////////////////////////////////////
std::string const CChallenge_13::sm_inputFilePath = "Inputs/Input_Challenge_13.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_13::Run_FirstPart()
{
    if (m_instructions[0].first == 'x')
    {
        FoldX(m_instructions[0].second);
    }
    else
    {
        FoldY(m_instructions[0].second);
    }

    int dotCount = 0;
    for (int y = 0; y < m_foldedSizeY; ++y)
    {
        for (int x = 0; x < m_foldedSizeX; ++x)
        {
            if (m_page[y][x])
            {
                ++dotCount;
            }
        }
    }

    std::cout << "Remaining dots after first fold: " << dotCount << std::endl;
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_13::Run_SecondPart()
{
    for (std::pair<char, int> const& instruction : m_instructions)
    {
        if (instruction.first == 'x')
        {
            FoldX(instruction.second);
        }
        else
        {
            FoldY(instruction.second);
        }
    }

    for (int y = 0; y < m_foldedSizeY; ++y)
    {
        for (int x = 0; x < m_foldedSizeX; ++x)
        {
            std::cout << (m_page[y][x] ? '#' : ' ');
        }
        std::cout << std::endl;
    }
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_13::SetUp_Common()
{
    std::vector<std::string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    size_t emptyLineIdx = 0;
    for (size_t lineIdxOffset = 0; lineIdxOffset < lines.size(); ++lineIdxOffset)
    {
        emptyLineIdx = lines.size() - lineIdxOffset - 1;
        if (lines[emptyLineIdx].empty())
        {
            break;
        }
    }

    int firstFoldPosX = -1;
    int firstFoldPosY = -1;

    for (size_t instructionLineIdx = emptyLineIdx + 1; instructionLineIdx < lines.size(); ++instructionLineIdx)
    {
        std::vector<std::string> args;
        FileHelper::SplitLine(lines[instructionLineIdx], "=", args);

        char const foldAxis = args[0].back();
        int const foldPos = atoi(args[1].c_str());
        m_instructions.emplace_back(foldAxis, foldPos);

        if (foldAxis == 'x')
        {
            if (firstFoldPosX == -1)
            {
                firstFoldPosX = foldPos;
            }
        }
        else
        {
            if (firstFoldPosY == -1)
            {
                firstFoldPosY = foldPos;
            }
        }
    }

    m_startingSizeX = firstFoldPosX * 2 + 1;
    m_startingSizeY = firstFoldPosY * 2 + 1;

    m_foldedSizeX = m_startingSizeX;
    m_foldedSizeY = m_startingSizeY;

    m_page = new bool*[m_startingSizeY];
    for (int y = 0; y < m_startingSizeY; ++y)
    {
        m_page[y] = new bool[m_startingSizeX];
        for (int x = 0; x < m_startingSizeX; ++x)
        {
            m_page[y][x] = false;
        }
    }

    for (size_t pointLineIdx = 0; pointLineIdx < emptyLineIdx; ++pointLineIdx)
    {
        std::vector<std::string> args;
        FileHelper::SplitLine(lines[pointLineIdx], ",", args);

        int const x = atoi(args[0].c_str());
        int const y = atoi(args[1].c_str());

        m_page[y][x] = true;
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_13::CleanUp_Common()
{
    for (int y = 0; y < m_startingSizeY; ++y)
    {
        delete m_page[y];
    }
    delete m_page;

    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
void CChallenge_13::FoldX(int const foldPosX)
{
    for (int y = 0; y < m_foldedSizeY; ++y)
    {
        for (int x = 0; x < foldPosX; ++x)
        {
            m_page[y][x] |= m_page[y][m_foldedSizeX - x - 1];
        }
    }

    m_foldedSizeX = foldPosX;
}

void CChallenge_13::FoldY(int const foldPosY)
{
    for (int y = 0; y < foldPosY; ++y)
    {
        for (int x = 0; x < m_foldedSizeX; ++x)
        {
            m_page[y][x] |= m_page[m_foldedSizeY - y - 1][x];
        }
    }

    m_foldedSizeY = foldPosY;
}
