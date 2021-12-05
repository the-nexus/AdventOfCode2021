#pragma once

#include "Common/Challenge.h"

class CChallenge_05 : public CChallenge
{
private:
    struct SPoint
    {
        SPoint() {}
        SPoint(int const x, int const y) : m_x(x), m_y(y) {}
        void Initialize(std::string const& pointStr);

        int m_x = 0;
        int m_y = 0;
    };

    struct SLine
    {
        SLine() {}
        void Initialize(std::string const& lineStr);

        inline bool IsHorizontal() const { return m_firstPoint.m_y == m_secondPoint.m_y; }
        inline bool IsVertical() const { return m_firstPoint.m_x == m_secondPoint.m_x; }
        void GetMinMaxX(int& outMinX, int& outMaxX) const;
        void GetMinMaxY(int& outMinY, int& outMaxY) const;

        SPoint m_firstPoint;
        SPoint m_secondPoint;
    };

    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    EErrorCode SetUp_Common();
    EErrorCode CleanUp_Common();

    void AddLineToGrid(SLine const& ventLine, bool shouldAddDiagonals);
    int GetGridIntersectionCount() const;

    static std::string const sm_inputFilePath;
    std::vector<SLine> m_ventLines;

    SPoint m_gridSize = SPoint(0, 0);
    SPoint m_gridAnchor = SPoint(INT_MAX, INT_MAX);

    int** m_ventGrid = nullptr;
};
