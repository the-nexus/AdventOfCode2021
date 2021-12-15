#pragma once

#include "Common/Challenge.h"
#include <map>

class CChallenge_14 : public CChallenge
{
private:
    struct SRule
    {
        SRule(std::string const& inputRuleStr, char const outputChar)
            : m_inputRuleStr(inputRuleStr)
            , m_outputChar(outputChar)
        {}

        std::string m_inputRuleStr = "";
        char m_outputChar = '\0';
        unsigned long long m_newOccurences = 0;
        unsigned long long m_previousOccurences = 0;
        unsigned long long m_totalOccurences = 0;
        SRule* m_leftOutputRule = nullptr;
        SRule* m_rightOutputRule = nullptr;
    };

    virtual EErrorCode SetUp_FirstPart() override { return SetUp_Common(); }
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override { return EErrorCode::Success; }

    virtual EErrorCode SetUp_SecondPart() override { return SetUp_Common(); }
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override { return EErrorCode::Success; }

    EErrorCode SetUp_Common();
    EErrorCode CleanUp_Common();

    void IterateRules();
    void CountSymbolOccurences(std::vector<unsigned long long>& symbolCounts);
    unsigned long long GetSymbolMinMaxOccurenceDelta(std::vector<unsigned long long>& symbolCounts);

    static std::string const sm_inputFilePath;
    std::string m_baseTemplate = "";
    std::vector<SRule*> m_rules;
    std::map<std::string, SRule*> m_rulesMap;
};
