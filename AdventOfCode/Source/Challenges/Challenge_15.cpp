#include "Challenge_15.h"
#include "../Helpers/FileHelpers.h"
#include <algorithm>



////////////////////////////////////////////////////////////////////////////////////////////////////
struct CellSortByHighestCostPredicate
{
    bool operator() (CChallenge_15::SCell const* lhs, CChallenge_15::SCell const* rhs) const
    {
        return lhs->m_cumulativeCost > rhs->m_cumulativeCost;
    }
};



////////////////////////////////////////////////////////////////////////////////////////////////////
std::string const CChallenge_15::sm_inputFilePath = "Inputs/Input_Challenge_15.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_15::SetUp_FirstPart()
{
    std::vector<std::string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_cellCountY = static_cast<int>(lines.size());
    m_cellCountX = static_cast<int>(lines[0].size());

    m_cells = new SCell**[m_cellCountY];
    for (int y = 0; y < m_cellCountY; ++y)
    {
        m_cells[y] = new SCell*[m_cellCountX];
        for (int x = 0; x < m_cellCountX; ++x)
        {
            int const cellCost = static_cast<int>(lines[y][x] - '0');
            m_cells[y][x] = new SCell(x, y, cellCost);
        }
    }

    m_startCell = m_cells[0][0];
    m_endCell = m_cells[m_cellCountY - 1][m_cellCountX - 1];

    m_startCell->m_cost = 0;
    m_startCell->m_wasProcessed = true;

    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_15::SetUp_SecondPart()
{
    std::vector<std::string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    int const baseSizeY = static_cast<int>(lines.size());
    int const baseSizeX = static_cast<int>(lines[0].size());

    m_cellCountY = baseSizeY * 5;
    m_cellCountX = baseSizeX * 5;

    m_cells = new SCell**[m_cellCountY];
    for (int y = 0; y < baseSizeY; ++y)
    {
        for (int yOffset = 0; yOffset < 5; ++yOffset)
        {
            int const offsetedY = baseSizeY * yOffset + y;
            m_cells[offsetedY] = new SCell*[m_cellCountX];

            for (int x = 0; x < baseSizeX; ++x)
            {
                int const cellCost = static_cast<int>(lines[y][x] - '0');
                for (int xOffset = 0; xOffset < 5; ++xOffset)
                {
                    int const offsetedX = baseSizeX * xOffset + x;
                    int const offsetedCellCost = ((cellCost + xOffset + yOffset - 1) % 9) + 1;
                    m_cells[offsetedY][offsetedX] = new SCell(offsetedX, offsetedY, offsetedCellCost);
                }
            }
        }
    }

    m_startCell = m_cells[0][0];
    m_endCell = m_cells[m_cellCountY - 1][m_cellCountX - 1];

    m_startCell->m_cost = 0;
    m_startCell->m_wasProcessed = true;

    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_15::Run_Common()
{
    std::vector<SCell*> pathOptions;
    pathOptions.push_back(m_startCell);

    bool pathFound = false;
    while (pathOptions.size() > 0)
    {
        // Best cell is lowest cost cell
        SCell* bestCell = pathOptions.back();
        pathOptions.pop_back();

        if (bestCell == m_endCell)
        {
            pathFound = true;
            break;
        }

        AddCell(pathOptions, bestCell, bestCell->m_x - 1, bestCell->m_y);
        AddCell(pathOptions, bestCell, bestCell->m_x, bestCell->m_y - 1);
        AddCell(pathOptions, bestCell, bestCell->m_x + 1, bestCell->m_y);
        AddCell(pathOptions, bestCell, bestCell->m_x, bestCell->m_y + 1);

        // Sorting by highest cost will bring lower cost items to the back
        // (better for performance when removing items to avoid shifting memory)
        std::sort(pathOptions.begin(), pathOptions.end(), CellSortByHighestCostPredicate());
    }

    if (pathFound)
    {
        std::cout << "Path cost : " << m_endCell->m_cumulativeCost;
    }
    else
    {
        std::cout << "Path not found :(" << std::endl;
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_15::CleanUp_Common()
{
    for (size_t y = 0; y < m_cellCountY; ++y)
    {
        for (size_t x = 0; x < m_cellCountX; ++x)
        {
            delete m_cells[y][x];
        }
        delete m_cells[y];
    }
    delete m_cells;

    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
void CChallenge_15::AddCell(std::vector<CChallenge_15::SCell*>& pathOptions, CChallenge_15::SCell* parentCell, int const x, int const y)
{
    if (x >= 0 && x < m_cellCountX && y >= 0 && y < m_cellCountY)
    {
        SCell* cellToAdd = m_cells[y][x];
        if (!cellToAdd->m_wasProcessed)
        {
            int const manhattanDistance = (m_endCell->m_x - cellToAdd->m_x) + (m_endCell->m_y - cellToAdd->m_y);

            cellToAdd->m_cumulativeCost = cellToAdd->m_cost + parentCell->m_cumulativeCost;
            cellToAdd->m_wasProcessed = true;
            cellToAdd->m_parentCell = parentCell;

            pathOptions.push_back(cellToAdd);
        }
    }
}
