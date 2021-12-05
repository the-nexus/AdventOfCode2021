#include "Challenge_05.h"
#include "../Helpers/FileHelpers.h"



////////////////////////////////////////////////////////////////////////////////////////////////////
std::string const CChallenge_05::sm_inputFilePath = "Inputs/Input_Challenge_05.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_05::SetUp_FirstPart()
{
    return SetUp_Common();
}

EErrorCode CChallenge_05::Run_FirstPart()
{
    for (SLine const& ventLine : m_ventLines)
    {
        AddLineToGrid(ventLine, false);
    }

    std::cout << "Vent lines intersection count: " << GetGridIntersectionCount() << std::endl;
    return EErrorCode::Success;
}

EErrorCode CChallenge_05::CleanUp_FirstPart()
{
    return CleanUp_Common();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_05::SetUp_SecondPart()
{
    return SetUp_Common();
}

EErrorCode CChallenge_05::Run_SecondPart()
{
    for (SLine const& ventLine : m_ventLines)
    {
        AddLineToGrid(ventLine, true);
    }

    std::cout << "Vent lines intersection count: " << GetGridIntersectionCount() << std::endl;
    return EErrorCode::Success;
}

EErrorCode CChallenge_05::CleanUp_SecondPart()
{
    return CleanUp_Common();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_05::SetUp_Common()
{
    std::vector<std::string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_ventLines.reserve(lines.size());
    for (std::string const& line : lines)
    {
        m_ventLines.emplace_back();

        SLine& ventLine = m_ventLines.back();
        ventLine.Initialize(line);

        int minX, maxX, minY, maxY;
        ventLine.GetMinMaxX(minX, maxX);
        ventLine.GetMinMaxY(minY, maxY);

        if (minX < m_gridAnchor.m_x)
        {
            m_gridAnchor.m_x = minX;
        }

        if (minY < m_gridAnchor.m_y)
        {
            m_gridAnchor.m_y = minY;
        }

        if (maxX > m_gridSize.m_x)
        {
            m_gridSize.m_x = maxX;
        }

        if (maxY > m_gridSize.m_y)
        {
            m_gridSize.m_y = maxY;
        }
    }

    m_gridSize.m_x += 1 - m_gridAnchor.m_x;
    m_gridSize.m_y += 1 - m_gridAnchor.m_y;

    m_ventGrid = new int*[m_gridSize.m_y];
    for (int y = 0; y < m_gridSize.m_y; ++y)
    {
        m_ventGrid[y] = new int[m_gridSize.m_x];
        for (int x = 0; x < m_gridSize.m_x; ++x)
        {
            m_ventGrid[y][x] = 0;
        }
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_05::CleanUp_Common()
{
    for (int y = 0; y < m_gridSize.m_y; ++y)
    {
        delete m_ventGrid[y];
    }
    delete m_ventGrid;

    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
void CChallenge_05::AddLineToGrid(SLine const& ventLine, bool shouldAddDiagonals)
{
    if (ventLine.IsHorizontal())
    {
        int minX, maxX;
        ventLine.GetMinMaxX(minX, maxX);

        int const y = ventLine.m_firstPoint.m_y - m_gridAnchor.m_y;
        int const xEnd = maxX - m_gridAnchor.m_x;
        for (int x = minX - m_gridAnchor.m_x; x <= xEnd; ++x)
        {
            ++m_ventGrid[y][x];
        }
    }
    else if (ventLine.IsVertical())
    {
        int minY, maxY;
        ventLine.GetMinMaxY(minY, maxY);

        int const x = ventLine.m_firstPoint.m_x - m_gridAnchor.m_x;
        int const yEnd = maxY - m_gridAnchor.m_y;
        for (int y = minY - m_gridAnchor.m_y; y <= yEnd; ++y)
        {
            ++m_ventGrid[y][x];
        }
    }
    else if (shouldAddDiagonals)
    {
        bool const isFirstPointMinX = ventLine.m_firstPoint.m_x < ventLine.m_secondPoint.m_x;
        SPoint const& startPoint = isFirstPointMinX ? ventLine.m_firstPoint : ventLine.m_secondPoint;
        SPoint const& endPoint = isFirstPointMinX ? ventLine.m_secondPoint : ventLine.m_firstPoint;
        bool const isEndPointMaxY = endPoint.m_y > startPoint.m_y;

        int x = startPoint.m_x - m_gridAnchor.m_x;
        int y = startPoint.m_y - m_gridAnchor.m_y;
        int const endX = endPoint.m_x - m_gridAnchor.m_x;

        while (x <= endX)
        {
            ++m_ventGrid[y][x];

            ++x;
            isEndPointMaxY ? ++y : --y;
        }
    }
}

int CChallenge_05::GetGridIntersectionCount() const
{
    int intersectionCount = 0;

    for (int y = 0; y < m_gridSize.m_y; ++y)
    {
        for (int x = 0; x < m_gridSize.m_x; ++x)
        {
            if (m_ventGrid[y][x] > 1)
            {
                ++intersectionCount;
            }
        }
    }

    return intersectionCount;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
void CChallenge_05::SPoint::Initialize(std::string const& pointStr)
{
    std::vector<std::string> pointArgs;
    FileHelper::SplitLine(pointStr, ",", pointArgs);

    m_x = atoi(pointArgs[0].c_str());
    m_y = atoi(pointArgs[1].c_str());
}



////////////////////////////////////////////////////////////////////////////////////////////////////
void CChallenge_05::SLine::Initialize(std::string const& lineStr)
{
    std::vector<std::string> lineArgs;
    FileHelper::SplitLine(lineStr, " -> ", lineArgs);

    m_firstPoint.Initialize(lineArgs[0]);
    m_secondPoint.Initialize(lineArgs[1]);
}

void CChallenge_05::SLine::GetMinMaxX(int& outMinX, int& outMaxX) const
{
    if (m_firstPoint.m_x < m_secondPoint.m_x)
    {
        outMinX = m_firstPoint.m_x;
        outMaxX = m_secondPoint.m_x;
    }
    else
    {
        outMinX = m_secondPoint.m_x;
        outMaxX = m_firstPoint.m_x;
    }
}

void CChallenge_05::SLine::GetMinMaxY(int& outMinY, int& outMaxY) const
{
    if (m_firstPoint.m_y < m_secondPoint.m_y)
    {
        outMinY = m_firstPoint.m_y;
        outMaxY = m_secondPoint.m_y;
    }
    else
    {
        outMinY = m_secondPoint.m_y;
        outMaxY = m_firstPoint.m_y;
    }
}
