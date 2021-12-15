#pragma once

#include "Common/Challenge.h"
#include <map>

class CChallenge_14 : public CChallenge
{
private:
    virtual EErrorCode SetUp_FirstPart() override { return SetUp_Common(); }
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override { return EErrorCode::Success; }

    virtual EErrorCode SetUp_SecondPart() override { return SetUp_Common(); }
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override { return EErrorCode::Success; }

    EErrorCode SetUp_Common();

    void CountSymbolAfterIterations(int const iterationCount, char const leftSymbol, char const rightSymbol, std::vector<unsigned long long>& symbolCounts, std::string& keyBuffer);

    static std::string const sm_inputFilePath;
    std::map<std::string, char> m_rules;
    std::string m_template;
};
