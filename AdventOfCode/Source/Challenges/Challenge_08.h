#pragma once

#include "Common/Challenge.h"

#define CODEX_SIGNAL_COUNT 10
#define OUTPUT_SIGNAL_COUNT 4

class CChallenge_08 : public CChallenge
{
    struct SSevenDigitDisplay
    {
        void Initialize(std::string const& inputSignalStr);

        std::string m_signalStr = "";
        uint8_t m_flags = 0;
    };

    struct SSignalPattern
    {
        void Initialize(std::string const& inputSignalStr);

        void SortCodex();
        int GetOutputValue() const;

        SSevenDigitDisplay m_codexSignals[CODEX_SIGNAL_COUNT];
        SSevenDigitDisplay m_outputSignals[OUTPUT_SIGNAL_COUNT];
    };

private:
    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    EErrorCode SetUp_Common();

    static std::string const sm_inputFilePath;
    std::vector<SSignalPattern> m_signalPatterns;
};
