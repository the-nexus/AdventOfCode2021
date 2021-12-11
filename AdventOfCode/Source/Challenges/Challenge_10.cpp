#include "Challenge_10.h"
#include "../Helpers/FileHelpers.h"
#include <algorithm>



////////////////////////////////////////////////////////////////////////////////////////////////////
std::string const CChallenge_10::sm_inputFilePath = "Inputs/Input_Challenge_10.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_10::Run_FirstPart()
{
    int errorScore = 0;
    for (std::string const& line : m_lines)
    {
        size_t cursorIdx = 0;
        if (!ValidateLineForCorruptedChar(line, cursorIdx))
        {
            if (cursorIdx < line.size())
            {
                errorScore += GetClosingCharCorruptionScore(line[cursorIdx]);
            }
        }
    }

    std::cout << "Error score: " << errorScore << std::endl;
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_10::Run_SecondPart()
{
    std::vector<unsigned long long> autocompleteScores;
    for (std::string const& line : m_lines)
    {
        size_t cursorIdx = 0;
        if (!ValidateLineForCorruptedChar(line, cursorIdx))
        {
            if (cursorIdx >= line.size())
            {
                cursorIdx = 0;
                autocompleteScores.push_back(GetAutoCompleteScore(line, cursorIdx));
                std::cout << autocompleteScores.back() << std::endl;
            }
        }
    }

    std::sort(autocompleteScores.begin(), autocompleteScores.end());
    unsigned long long const middleScore = autocompleteScores[(autocompleteScores.size() / 2)];

    std::cout << "Auto-complete middle score: " << middleScore << std::endl;
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_10::SetUp_Common()
{
    return FileHelper::ReadLines(sm_inputFilePath, m_lines);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
bool CChallenge_10::ValidateLineForCorruptedChar(std::string const& line, size_t& cursorIdx)
{
    while (cursorIdx < line.size())
    {
        char const cursorChar = line[cursorIdx];
        if (IsOpeningChar(cursorChar))
        {
            ++cursorIdx;
            if (!ValidateLineForCorruptedChar(line, cursorIdx))
            {
                return false;
            }

            if (line[cursorIdx] != GetMatchingClosingChar(cursorChar))
            {
                return false;
            }
            ++cursorIdx;
        }
        else
        {
            return true;
        }
    }

    return true;
}

unsigned long long CChallenge_10::GetAutoCompleteScore(std::string const& line, size_t& cursorIdx)
{
    while (cursorIdx < line.size())
    {
        char const cursorChar = line[cursorIdx];
        if (IsOpeningChar(cursorChar))
        {
            ++cursorIdx;
            unsigned long long const subAutoCompleteScore = GetAutoCompleteScore(line, cursorIdx);
            if (cursorIdx >= line.size())
            {
                char const matchingClosingChar = GetMatchingClosingChar(cursorChar);
                return GetClosingCharAutoCompleteScore(matchingClosingChar) + subAutoCompleteScore * 5;
            }
            ++cursorIdx;
        }
        else
        {
            return 0;
        }
    }

    return 0;
}

bool CChallenge_10::IsOpeningChar(char const openingChar) const
{
    return openingChar == '('
        || openingChar == '['
        || openingChar == '{'
        || openingChar == '<';
}

char CChallenge_10::GetMatchingClosingChar(char const openingChar) const
{
    switch (openingChar)
    {
    case '(':
        return ')';
    case '[':
        return ']';
    case '{':
        return '}';
    case '<':
        return '>';
    default:
        return '\0';
    }
}

int CChallenge_10::GetClosingCharCorruptionScore(char const closingChar) const
{
    switch (closingChar)
    {
    case ')':
        return 3;
    case ']':
        return 57;
    case '}':
        return 1197;
    case '>':
        return 25137;
    default:
        return 0;
    }
}

int CChallenge_10::GetClosingCharAutoCompleteScore(char const closingChar) const
{
    switch (closingChar)
    {
    case ')':
        return 1;
    case ']':
        return 2;
    case '}':
        return 3;
    case '>':
        return 4;
    default:
        return 0;
    }
}
