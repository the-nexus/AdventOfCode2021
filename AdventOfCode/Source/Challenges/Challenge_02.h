#pragma once

#include "Common/Challenge.h"

class CChallenge_02 : public CChallenge
{
private:
    enum class EDirection
    {
        None,
        Up,
        Down,
        Forward
    };

    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    EErrorCode SetUp_Common();
    static EDirection GetDirectionFromString(std::string const& directionStr);

    static std::string const sm_inputFilePath;
    std::vector<std::pair<EDirection, int>> m_movementInstructions;
};
