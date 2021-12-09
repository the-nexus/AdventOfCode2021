#pragma once

#include "Common/Challenge.h"

class CChallenge_09 : public CChallenge
{
private:
    virtual EErrorCode SetUp_FirstPart() override { return SetUp_Common(); }
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override { return CleanUp_Common(); }

    virtual EErrorCode SetUp_SecondPart() override { return SetUp_Common(); }
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override { return CleanUp_Common(); }

    EErrorCode SetUp_Common();
    EErrorCode CleanUp_Common();

    int CountAndMarkBasin(size_t const lineIdx, size_t const colIdx);

    static std::string const sm_inputFilePath;
    int** m_heightmap = nullptr;
    size_t m_heightmapWidth = 0;
    size_t m_heightmapHeight = 0;
};
