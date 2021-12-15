#include "Challenge_14.h"
#include "../Helpers/FileHelpers.h"
#include <algorithm>



////////////////////////////////////////////////////////////////////////////////////////////////////
std::string const CChallenge_14::sm_inputFilePath = "Inputs/Input_Challenge_14.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_14::Run_FirstPart()
{
    int const iterationCount = 10;

    std::vector<unsigned long long> symbolCounts;
    symbolCounts.resize(26, 0);

    for (size_t templateIdx = 0; templateIdx < m_template.size(); ++templateIdx)
    {
        ++symbolCounts[static_cast<size_t>(m_template[templateIdx] - 'A')];
    }

    std::string keyBuffer = "##";
    for (size_t templateIdx = 1; templateIdx < m_template.size(); ++templateIdx)
    {
        CountSymbolAfterIterations(iterationCount, m_template[templateIdx - 1], m_template[templateIdx], symbolCounts, keyBuffer);
    }

    std::sort(symbolCounts.begin(), symbolCounts.end());
    unsigned long long quantityDelta = 0;
    for (unsigned long long const symbolCount : symbolCounts)
    {
        if (symbolCount != 0)
        {
            quantityDelta = symbolCounts.back() - symbolCount;
            break;
        }
    }

    std::cout << "Quantity delta: " << quantityDelta << std::endl;
    return EErrorCode::Success;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_14::Run_SecondPart()
{

    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_14::SetUp_Common()
{
    std::vector<std::string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_template = lines[0];

    for (size_t ruleIdx = 2; ruleIdx < lines.size(); ++ruleIdx)
    {
        std::vector<std::string> args;
        FileHelper::SplitLine(lines[ruleIdx], " -> ", args);
        m_rules.emplace(args[0], args[1][0]);
    }

    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
void CChallenge_14::CountSymbolAfterIterations(int const iterationCount, char const leftSymbol, char const rightSymbol, std::vector<unsigned long long>& symbolCounts, std::string& keyBuffer)
{
    if (iterationCount > 0)
    {
        keyBuffer[0] = leftSymbol;
        keyBuffer[1] = rightSymbol;

        auto const it = m_rules.find(keyBuffer);
        if (it != m_rules.cend())
        {
            int const remainingIterationCount = iterationCount - 1;
            char middleSymbol = it->second;

            CountSymbolAfterIterations(remainingIterationCount, leftSymbol, middleSymbol, symbolCounts, keyBuffer);
            ++symbolCounts[static_cast<size_t>(middleSymbol - 'A')];
            CountSymbolAfterIterations(remainingIterationCount, middleSymbol, rightSymbol, symbolCounts, keyBuffer);
        }
    }
}
