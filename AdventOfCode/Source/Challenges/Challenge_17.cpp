#include "Challenge_17.h"
#include "../Helpers/FileHelpers.h"



////////////////////////////////////////////////////////////////////////////////////////////////////
std::string const CChallenge_17::sm_inputFilePath = "Inputs/Input_Challenge_17.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_17::Run_FirstPart()
{
    // With a constant acceleration, the y coordinate will always come back to 0 at some point
    // Find the height delta from 0 to min Y which should give the largest step possible to hit the box
    // Then use that to reverse to the peek position

    int const deltaY = -m_targetMinY;
    int const initialSpeedY = deltaY - 1;

    int currentSpeedY = initialSpeedY;
    int currentPosY = 0;
    while (currentSpeedY > 0)
    {
        currentPosY += currentSpeedY;
        currentSpeedY -= 1;
    }

    std::cout << "Max height: " << currentPosY << std::endl;
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_17::Run_SecondPart()
{
    int minDeltaTimeX = 0;
    int posX = m_targetMinX;
    int minSpeedX = 0;
    while (posX > 0)
    {
        ++minSpeedX;
        posX -= minSpeedX;
    }

    int const maxSpeedX = m_targetMaxX;
    int const maxSpeedY = -m_targetMinY - 1; // hit the lowest Y in the biggest step possible
    int const minSpeedY = m_targetMinY; // hit the lowest Y in 1 step

    std::vector<std::pair<int, int>> validInitialVelocities;
    validInitialVelocities.reserve((maxSpeedX - minSpeedX) * (maxSpeedY - minSpeedY));

    for (int initialSpeedY = minSpeedY; initialSpeedY <= maxSpeedY; ++initialSpeedY)
    {
        for (int initialSpeedX = minSpeedX; initialSpeedX <= maxSpeedX; ++initialSpeedX)
        {
            int posX = 0;
            int posY = 0;
            int speedX = initialSpeedX;
            int speedY = initialSpeedY;

            bool keepSimulating = true;
            while (keepSimulating)
            {
                posX += speedX;
                posY += speedY;

                --speedY;
                if (speedX > 0)
                {
                    --speedX;
                }

                if (posX > m_targetMaxX || posY < m_targetMinY)
                {
                    keepSimulating = false;
                }
                else if (posX >= m_targetMinX && posY <= m_targetMaxY)
                {
                    validInitialVelocities.emplace_back(initialSpeedX, initialSpeedY);
                    keepSimulating = false;
                }
            }
        }
    }

    std::cout << "Number of valid initial velocities: " << validInitialVelocities.size() << std::endl;
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_17::SetUp_Common()
{
    std::string line;
    EErrorCode const readErrorCode = FileHelper::ReadFirstLine(sm_inputFilePath, line);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    std::vector<std::string> lineSplit;
    FileHelper::SplitLine(line, "x=", lineSplit);

    std::vector<std::string> coordsLineSplit;
    FileHelper::SplitLine(lineSplit[1], ", y=", coordsLineSplit);

    std::vector<std::string> coordsLineX;
    FileHelper::SplitLine(coordsLineSplit[0], "..", coordsLineX);

    std::vector<std::string> coordsLineY;
    FileHelper::SplitLine(coordsLineSplit[1], "..", coordsLineY);

    m_targetMinX = atoi(coordsLineX[0].c_str());
    m_targetMaxX = atoi(coordsLineX[1].c_str());
    m_targetMinY = atoi(coordsLineY[0].c_str());
    m_targetMaxY = atoi(coordsLineY[1].c_str());

    return EErrorCode::Success;
}
