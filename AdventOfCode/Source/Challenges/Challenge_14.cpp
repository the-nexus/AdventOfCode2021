#include "Challenge_14.h"
#include "../Helpers/FileHelpers.h"
#include <algorithm>



////////////////////////////////////////////////////////////////////////////////////////////////////
std::string const CChallenge_14::sm_inputFilePath = "Inputs/Input_Challenge_14.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_14::Run_FirstPart()
{
    int const iterationCount = 10;
    for (int iterationIdx = 1; iterationIdx < iterationCount; ++iterationIdx)
    {
        IterateRules();
    }

    std::vector<unsigned long long> symbolCounts;
    CountSymbolOccurences(symbolCounts);

    unsigned long long const quantityDelta = GetSymbolMinMaxOccurenceDelta(symbolCounts);
    std::cout << "Quantity delta after " << iterationCount << " iterations: " << quantityDelta << std::endl;
    return EErrorCode::Success;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_14::Run_SecondPart()
{
    int const iterationCount = 40;
    for (int iterationIdx = 1; iterationIdx < iterationCount; ++iterationIdx)
    {
        IterateRules();
    }

    std::vector<unsigned long long> symbolCounts;
    CountSymbolOccurences(symbolCounts);

    unsigned long long const quantityDelta = GetSymbolMinMaxOccurenceDelta(symbolCounts);
    std::cout << "Quantity delta after " << iterationCount << " iterations: " << quantityDelta << std::endl;
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

    m_baseTemplate = lines[0];

    // Build rules
    m_rules.reserve(lines.size() - 2);
    for (size_t ruleIdx = 2; ruleIdx < lines.size(); ++ruleIdx)
    {
        std::vector<std::string> args;
        FileHelper::SplitLine(lines[ruleIdx], " -> ", args);

        m_rules.push_back(new SRule(args[0], args[1][0]));
        m_rulesMap[args[0]] = m_rules.back();
    }

    // Link input rules to their output rules
    std::string tempBuffer = "##";
    for (SRule* inputRule : m_rules)
    {
        // Find left output rule
        tempBuffer[0] = inputRule->m_inputRuleStr[0];
        tempBuffer[1] = inputRule->m_outputChar;

        auto leftOutputRuleIt = m_rulesMap.find(tempBuffer);
        if (leftOutputRuleIt != m_rulesMap.end())
        {
            inputRule->m_leftOutputRule = leftOutputRuleIt->second;
        }

        // Find right output rule
        tempBuffer[0] = inputRule->m_outputChar;
        tempBuffer[1] = inputRule->m_inputRuleStr[1];

        auto rightOutputRuleIt = m_rulesMap.find(tempBuffer);
        if (leftOutputRuleIt != m_rulesMap.end())
        {
            inputRule->m_rightOutputRule = rightOutputRuleIt->second;
        }
    }

    // Count initial occurences from template string
    for (size_t templateIdx = 1; templateIdx < m_baseTemplate.size(); ++templateIdx)
    {
        tempBuffer[0] = m_baseTemplate[templateIdx - 1];
        tempBuffer[1] = m_baseTemplate[templateIdx];

        auto ruleIt = m_rulesMap.find(tempBuffer);
        if (ruleIt != m_rulesMap.end())
        {
            ++ruleIt->second->m_previousOccurences;
            ++ruleIt->second->m_totalOccurences;
        }
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_14::CleanUp_Common()
{
    for (SRule* rule : m_rules)
    {
        delete rule;
    }

    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
void CChallenge_14::IterateRules()
{
    // Use the backup to iterate on rules
    for (SRule* rule : m_rules)
    {
        if (rule->m_leftOutputRule)
        {
            rule->m_leftOutputRule->m_newOccurences += rule->m_previousOccurences;
        }

        if (rule->m_rightOutputRule)
        {
            rule->m_rightOutputRule->m_newOccurences += rule->m_previousOccurences;
        }
    }

    // Backup previous occurences
    for (SRule* rule : m_rules)
    {
        rule->m_totalOccurences += rule->m_newOccurences;
        rule->m_previousOccurences = rule->m_newOccurences;
        rule->m_newOccurences = 0;
    }
}

void CChallenge_14::CountSymbolOccurences(std::vector<unsigned long long>& symbolCounts)
{
    symbolCounts.resize(26, 0);

    // Count the symbols in the base template
    for (char const templateChar : m_baseTemplate)
    {
        ++symbolCounts[static_cast<size_t>(templateChar - 'A')];
    }

    // Count the output symbol occurences in the rules
    for (SRule* rule : m_rules)
    {
        symbolCounts[static_cast<size_t>(rule->m_outputChar - 'A')] += rule->m_totalOccurences;
    }
}

unsigned long long CChallenge_14::GetSymbolMinMaxOccurenceDelta(std::vector<unsigned long long>& symbolCounts)
{
    std::sort(symbolCounts.begin(), symbolCounts.end());

    for (size_t symbolIdx = 0; symbolIdx < symbolCounts.size() - 1; ++symbolIdx)
    {
        if (symbolCounts[symbolIdx] > 0)
        {
            return symbolCounts.back() - symbolCounts[symbolIdx];
        }
    }

    return symbolCounts.back();
}
