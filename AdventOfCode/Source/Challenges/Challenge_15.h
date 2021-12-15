#pragma once

#include "Common/Challenge.h"

class CChallenge_15 : public CChallenge
{
public:
    struct SCell
    {
        SCell(int const x, int const y, int const cellCost)
            : m_x(x)
            , m_y(y)
            , m_cost(cellCost)
        {}

        int m_cumulativeCost = 0;
        bool m_wasProcessed = false;
        SCell* m_parentCell = nullptr;

        int m_x = 0;
        int m_y = 0;
        int m_cost = 0;
    };

private:
    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override { return Run_Common(); }
    virtual EErrorCode CleanUp_FirstPart() override { return CleanUp_Common(); }

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override { return Run_Common(); }
    virtual EErrorCode CleanUp_SecondPart() override { return CleanUp_Common(); }

    EErrorCode Run_Common();
    EErrorCode CleanUp_Common();

    void AddCell(std::vector<SCell*>& pathOptions, SCell* parentCell, int const x, int const y);

    static std::string const sm_inputFilePath;

    int m_cellCountX = 0;
    int m_cellCountY = 0;
    SCell*** m_cells = nullptr;
    SCell* m_startCell = nullptr;
    SCell* m_endCell = nullptr;
};
