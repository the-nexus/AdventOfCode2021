#include "Challenge_02.h"
#include "../Helpers/FileHelpers.h"



////////////////////////////////////////////////////////////////////////////////////////////////////
std::string const CChallenge_02::sm_inputFilePath = "Inputs/Input_Challenge_02.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_02::SetUp_FirstPart()
{
    return SetUp_Common();
}

EErrorCode CChallenge_02::Run_FirstPart()
{
    int x = 0;
    int y = 0;

    for (std::pair<EDirection, int> const& movementInstruction : m_movementInstructions)
    {
        switch (movementInstruction.first)
        {
        case EDirection::Up:
            y -= movementInstruction.second;
            break;

        case EDirection::Down:
            y += movementInstruction.second;
            break;

        case EDirection::Forward:
            x += movementInstruction.second;
            break;

        default:
            break;
        }
    }

    int const movementMultiplication = x * y;
    std::cout << "Movement axis multiplication: " << movementMultiplication << std::endl;
    return EErrorCode::Success;
}

EErrorCode CChallenge_02::CleanUp_FirstPart()
{
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_02::SetUp_SecondPart()
{
    return SetUp_Common();
}

EErrorCode CChallenge_02::Run_SecondPart()
{
    int x = 0;
    int y = 0;
    int aim = 0;

    for (std::pair<EDirection, int> const& movementInstruction : m_movementInstructions)
    {
        switch (movementInstruction.first)
        {
        case EDirection::Up:
            aim -= movementInstruction.second;
            break;

        case EDirection::Down:
            aim += movementInstruction.second;
            break;

        case EDirection::Forward:
            x += movementInstruction.second;
            y += movementInstruction.second * aim;
            break;

        default:
            break;
        }
    }

    int const movementMultiplication = x * y;
    std::cout << "Movement axis multiplication: " << movementMultiplication << std::endl;
    return EErrorCode::Success;
}

EErrorCode CChallenge_02::CleanUp_SecondPart()
{
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_02::SetUp_Common()
{
    std::vector<std::string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_movementInstructions.reserve(lines.size());
    for (std::string const& line : lines)
    {
        std::vector<std::string> args;
        FileHelper::SplitLine(line, " ", args);

        EDirection const movementDirection = GetDirectionFromString(args[0]);
        int const movementDistance = atoi(args[1].c_str());
        m_movementInstructions.emplace_back(movementDirection, movementDistance);
    }

    return EErrorCode::Success;
}

CChallenge_02::EDirection CChallenge_02::GetDirectionFromString(std::string const& directionStr)
{
    switch (directionStr[0])
    {
    case 'u': return EDirection::Up;
    case 'd': return EDirection::Down;
    case 'f': return EDirection::Forward;
    default: return EDirection::None;
    }
}
