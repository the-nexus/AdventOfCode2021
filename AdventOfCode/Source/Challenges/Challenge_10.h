#pragma once

#include "Common/Challenge.h"

class CChallenge_10 : public CChallenge
{
private:
    virtual EErrorCode SetUp_FirstPart() override { return SetUp_Common(); }
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override { return EErrorCode::Success; }

    virtual EErrorCode SetUp_SecondPart() override { return SetUp_Common(); }
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override { return EErrorCode::Success; }

    EErrorCode SetUp_Common();

    bool ValidateLineForCorruptedChar(std::string const& line, size_t& cursorIdx);
    unsigned long long GetAutoCompleteScore(std::string const& line, size_t& cursorIdx);
    bool IsOpeningChar(char const openingChar) const;
    char GetMatchingClosingChar(char const openingChar) const;
    int GetClosingCharCorruptionScore(char const closingChar) const;
    int GetClosingCharAutoCompleteScore(char const closingChar) const;

    static std::string const sm_inputFilePath;
    std::vector<std::string> m_lines;
};
