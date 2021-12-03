#pragma once

#include "Common/Challenge.h"

class CChallenge_03 : public CChallenge
{
private:
    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    static void SumBinaryDigits(std::vector<std::string> const& binaryNumbers, unsigned int const digitIdx, unsigned int& digitSum, unsigned int& majoritySum);
    static void FindNumberBySum(std::vector<std::string> const& binaryNumbers, unsigned int const digitIdx, bool const useMajority, std::string& outNumber);

    static std::string const sm_inputFilePath;
    std::vector<std::string> m_binaryNumbers;
};
