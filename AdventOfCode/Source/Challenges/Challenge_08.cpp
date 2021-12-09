#include "Challenge_08.h"
#include "../Helpers/FileHelpers.h"
#include <map>

#define SEGMENT_FLAG_A 0b00000001
#define SEGMENT_FLAG_B 0b00000010
#define SEGMENT_FLAG_C 0b00000100
#define SEGMENT_FLAG_D 0b00001000
#define SEGMENT_FLAG_E 0b00010000
#define SEGMENT_FLAG_F 0b00100000
#define SEGMENT_FLAG_G 0b01000000
#define SEGMENT_MASK   0b01111111

////////////////////////////////////////////////////////////////////////////////////////////////////
namespace GlobalFunctions
{
    std::map<char, uint8_t> MakeSegmentCharToFlagMap()
    {
        std::map<char, uint8_t> flagMap;
        flagMap['a'] = SEGMENT_FLAG_A;
        flagMap['b'] = SEGMENT_FLAG_B;
        flagMap['c'] = SEGMENT_FLAG_C;
        flagMap['d'] = SEGMENT_FLAG_D;
        flagMap['e'] = SEGMENT_FLAG_E;
        flagMap['f'] = SEGMENT_FLAG_F;
        flagMap['g'] = SEGMENT_FLAG_G;
        return flagMap;
    }

    std::map<uint8_t, char> MakeSegmentFlagToCharMap()
    {
        std::map<uint8_t, char> flagMap;
        flagMap[SEGMENT_FLAG_A] = 'a';
        flagMap[SEGMENT_FLAG_B] = 'b';
        flagMap[SEGMENT_FLAG_C] = 'c';
        flagMap[SEGMENT_FLAG_D] = 'd';
        flagMap[SEGMENT_FLAG_E] = 'e';
        flagMap[SEGMENT_FLAG_F] = 'f';
        flagMap[SEGMENT_FLAG_G] = 'g';
        return flagMap;
    }
}

std::map<char, uint8_t> const g_segmentCharToFlagMap = GlobalFunctions::MakeSegmentCharToFlagMap();
std::map<uint8_t, char> const g_segmentFlagToCharMap = GlobalFunctions::MakeSegmentFlagToCharMap();


////////////////////////////////////////////////////////////////////////////////////////////////////
std::string const CChallenge_08::sm_inputFilePath = "Inputs/Input_Challenge_08.txt";




////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_08::SetUp_FirstPart()
{
    return SetUp_Common();
}

EErrorCode CChallenge_08::Run_FirstPart()
{
    int totalMatchingDigits = 0;

    for (SSignalPattern& signalPattern : m_signalPatterns)
    {
        for (size_t signalIdx = 0; signalIdx < OUTPUT_SIGNAL_COUNT; ++signalIdx)
        {
            size_t const signalSize = signalPattern.m_outputSignals[signalIdx].m_signalStr.size();
            if (signalSize == 2 ||
                signalSize == 3 ||
                signalSize == 4 ||
                signalSize == 7)
            {
                ++totalMatchingDigits;
            }
        }
    }

    std::cout << "Matching digit count: " << totalMatchingDigits << std::endl;
    return EErrorCode::Success;
}

EErrorCode CChallenge_08::CleanUp_FirstPart()
{
    return EErrorCode::NotImplemented;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_08::SetUp_SecondPart()
{
    return SetUp_Common();
}

EErrorCode CChallenge_08::Run_SecondPart()
{
    int outputSignalSum = 0;

    for (SSignalPattern& signalPattern : m_signalPatterns)
    {
        signalPattern.SortCodex();
        outputSignalSum += signalPattern.GetOutputValue();
    }

    std::cout << "Output signal sum: " << outputSignalSum << std::endl;
    return EErrorCode::Success;
}

EErrorCode CChallenge_08::CleanUp_SecondPart()
{
    return EErrorCode::NotImplemented;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_08::SetUp_Common()
{
    std::vector<std::string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    size_t const lineCount = lines.size();
    m_signalPatterns.resize(lineCount);

    for (size_t lineIdx = 0; lineIdx < lineCount; ++lineIdx)
    {
        m_signalPatterns[lineIdx].Initialize(lines[lineIdx]);
    }

    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
void CChallenge_08::SSevenDigitDisplay::Initialize(std::string const& signalStr)
{
    m_signalStr = signalStr;
    for (char const segmentChar : signalStr)
    {
        m_flags |= g_segmentCharToFlagMap.at(segmentChar);
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////
void CChallenge_08::SSignalPattern::Initialize(std::string const& inputSignalStr)
{
    std::vector<std::string> patternParts;
    FileHelper::SplitLine(inputSignalStr, " | ", patternParts);

    // Parse codex signals string
    std::vector<std::string> codexSignals;
    FileHelper::SplitLine(patternParts[0], " ", codexSignals);

    for (size_t signalIdx = 0; signalIdx < CODEX_SIGNAL_COUNT; ++signalIdx)
    {
        m_codexSignals[signalIdx].Initialize(codexSignals[signalIdx]);
    }

    // Parse output signals string
    std::vector<std::string> outputSignals;
    FileHelper::SplitLine(patternParts[1], " ", outputSignals);

    for (size_t signalIdx = 0; signalIdx < OUTPUT_SIGNAL_COUNT; ++signalIdx)
    {
        m_outputSignals[signalIdx].Initialize(outputSignals[signalIdx]);
    }
}

void CChallenge_08::SSignalPattern::SortCodex()
{
    bool wasCodexIndexOrdered[CODEX_SIGNAL_COUNT];
    for (size_t signalIdx = 0; signalIdx < CODEX_SIGNAL_COUNT; ++signalIdx)
    {
        wasCodexIndexOrdered[signalIdx] = false;
    }

    // 1, 4, 7, 8
    for (size_t signalIdx = 0; signalIdx < CODEX_SIGNAL_COUNT;)
    {
        if (!wasCodexIndexOrdered[signalIdx])
        {
            size_t const signalSize = m_codexSignals[signalIdx].m_signalStr.size();

            if (signalSize == 2) // 1
            {
                wasCodexIndexOrdered[1] = true;
                if (signalIdx != 1)
                {
                    std::swap(m_codexSignals[signalIdx], m_codexSignals[1]);
                    continue;
                }
            }
            else if (signalSize == 4) // 4
            {
                wasCodexIndexOrdered[4] = true;
                if (signalIdx != 4)
                {
                    std::swap(m_codexSignals[signalIdx], m_codexSignals[4]);
                    continue;
                }
            }
            else if (signalSize == 3) // 7
            {
                wasCodexIndexOrdered[7] = true;
                if (signalIdx != 7)
                {
                    std::swap(m_codexSignals[signalIdx], m_codexSignals[7]);
                    continue;
                }
            }
            else if (signalSize == 7) // 8
            {
                wasCodexIndexOrdered[8] = true;
                if (signalIdx != 8)
                {
                    std::swap(m_codexSignals[signalIdx], m_codexSignals[8]);
                    continue;
                }
            }
        }

        ++signalIdx;
    }

    // 6
    std::string const& oneStr = m_codexSignals[1].m_signalStr;
    for (size_t signalIdx = 0; signalIdx < CODEX_SIGNAL_COUNT; ++signalIdx)
    {
        if (!wasCodexIndexOrdered[signalIdx])
        {
            if (m_codexSignals[signalIdx].m_signalStr.size() == 6)
            {
                if ((g_segmentCharToFlagMap.at(oneStr[0]) & m_codexSignals[signalIdx].m_flags) == 0)
                {
                    wasCodexIndexOrdered[6] = true;
                    if (signalIdx != 6)
                    {
                        std::swap(m_codexSignals[signalIdx], m_codexSignals[6]);
                    }
                    break;
                }
                else if ((g_segmentCharToFlagMap.at(oneStr[1]) & m_codexSignals[signalIdx].m_flags) == 0)
                {
                    wasCodexIndexOrdered[6] = true;
                    if (signalIdx != 6)
                    {
                        std::swap(m_codexSignals[signalIdx], m_codexSignals[6]);
                    }
                    break;
                }
            }
        }
    }

    // 9
    uint8_t const nineBottomFilter = ~(m_codexSignals[4].m_flags | m_codexSignals[7].m_flags) & SEGMENT_MASK;
    for (size_t signalIdx = 0; signalIdx < CODEX_SIGNAL_COUNT; ++signalIdx)
    {
        if (!wasCodexIndexOrdered[signalIdx])
        {
            if (m_codexSignals[signalIdx].m_signalStr.size() == 6)
            {
                uint8_t const nineBottomRemainder = nineBottomFilter & m_codexSignals[signalIdx].m_flags;
                auto const it = g_segmentFlagToCharMap.find(nineBottomRemainder);

                if (it != g_segmentFlagToCharMap.cend())
                {
                    wasCodexIndexOrdered[9] = true;
                    if (signalIdx != 9)
                    {
                        std::swap(m_codexSignals[signalIdx], m_codexSignals[9]);
                    }
                    break;
                }
            }
        }
    }

    // 0
    for (size_t signalIdx = 0; signalIdx < CODEX_SIGNAL_COUNT; ++signalIdx)
    {
        if (!wasCodexIndexOrdered[signalIdx])
        {
            if (m_codexSignals[signalIdx].m_signalStr.size() == 6)
            {
                wasCodexIndexOrdered[0] = true;
                if (signalIdx != 0)
                {
                    std::swap(m_codexSignals[signalIdx], m_codexSignals[0]);
                }
                break;
            }
        }
    }

    // 5
    uint8_t const fiveFlags = m_codexSignals[6].m_flags & m_codexSignals[9].m_flags & SEGMENT_MASK;
    for (size_t signalIdx = 0; signalIdx < CODEX_SIGNAL_COUNT; ++signalIdx)
    {
        if (!wasCodexIndexOrdered[signalIdx] && m_codexSignals[signalIdx].m_flags == fiveFlags)
        {
            wasCodexIndexOrdered[5] = true;
            if (signalIdx != 5)
            {
                std::swap(m_codexSignals[signalIdx], m_codexSignals[5]);
            }
            break;
        }
    }

    // 3 (and 2 because its the remaining one
    uint8_t const threeFlags = ~(((m_codexSignals[4].m_flags ^ m_codexSignals[9].m_flags) | m_codexSignals[1].m_flags) ^ m_codexSignals[0].m_flags) & SEGMENT_MASK;
    for (size_t signalIdx = 0; signalIdx < CODEX_SIGNAL_COUNT; ++signalIdx)
    {
        if (!wasCodexIndexOrdered[signalIdx] && m_codexSignals[signalIdx].m_flags == threeFlags)
        {
            wasCodexIndexOrdered[3] = true;
            wasCodexIndexOrdered[2] = true;
            if (signalIdx != 3)
            {
                std::swap(m_codexSignals[signalIdx], m_codexSignals[3]);
            }
            break;
        }
    }
}

int CChallenge_08::SSignalPattern::GetOutputValue() const
{
    int outputValue = 0;
    int digitOffset = 1;

    for (size_t outputSignalIdx = 0; outputSignalIdx < OUTPUT_SIGNAL_COUNT; ++outputSignalIdx)
    {
        uint8_t const digitFlags = m_outputSignals[OUTPUT_SIGNAL_COUNT - outputSignalIdx - 1].m_flags;

        for (size_t codexSignalIdx = 0; codexSignalIdx < CODEX_SIGNAL_COUNT; ++codexSignalIdx)
        {
            if (m_codexSignals[codexSignalIdx].m_flags == digitFlags)
            {
                outputValue += digitOffset * static_cast<int>(codexSignalIdx);
                digitOffset *= 10;
                break;
            }
        }
    }

    return outputValue;
}
