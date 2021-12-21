#include "Challenge_21.h"
#include "../Helpers/FileHelpers.h"



////////////////////////////////////////////////////////////////////////////////////////////////////
std::string const CChallenge_21::sm_inputFilePath = "Inputs/Input_Challenge_21.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_21::Run_FirstPart()
{
    int const diceRollCountPerRound = 3;

    int diceRollCount = 0;
    int dieValue = 0;

    int firstPlayerPoints = 0;
    int firstPlayerPos = m_firstPlayerStartingPosition;

    int secondPlayerPoints = 0;
    int secondPlayerPos = m_secondPlayerStartingPosition;

    bool isFirstPlayerTurn = true;
    while (firstPlayerPos < 1000 && secondPlayerPoints < 1000)
    {
        diceRollCount += diceRollCountPerRound;
        int sumToAdd = diceRollCountPerRound;
        for (int rollIdx = 0; rollIdx < diceRollCountPerRound; ++rollIdx)
        {
            sumToAdd += dieValue;
            dieValue = (dieValue + 1) % 100;
        }

        if (isFirstPlayerTurn)
        {
            firstPlayerPos = (firstPlayerPos + sumToAdd) % 10;
            firstPlayerPos += firstPlayerPos + 1;
            isFirstPlayerTurn = false;
        }
        else
        {
            secondPlayerPos = (secondPlayerPos + sumToAdd) % 10;
            secondPlayerPoints += secondPlayerPos + 1;
            isFirstPlayerTurn = true;
        }
    }

    int loserPoints = firstPlayerPos < secondPlayerPoints ? firstPlayerPos : secondPlayerPoints;
    int const loserScore = loserPoints * diceRollCount;
    std::cout << "Loser's score: " << loserScore << std::endl;
    return EErrorCode::NotImplemented;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_21::Run_SecondPart()
{
    std::vector<std::pair<int, int>> resultProbabilities;
    resultProbabilities.reserve(7);
    resultProbabilities.emplace_back(3, 1);
    resultProbabilities.emplace_back(4, 3);
    resultProbabilities.emplace_back(5, 6);
    resultProbabilities.emplace_back(6, 7);
    resultProbabilities.emplace_back(7, 6);
    resultProbabilities.emplace_back(8, 3);
    resultProbabilities.emplace_back(9, 1);

    unsigned long long firstPlayerWinCount = 0;
    unsigned long long secondPlayerWinCount = 0;

    DoQuantumDieRoll(
        resultProbabilities,
        1,
        true,
        0,
        m_firstPlayerStartingPosition,
        0,
        m_secondPlayerStartingPosition,
        firstPlayerWinCount,
        secondPlayerWinCount
    );

    unsigned long long const winningPlayerCount = firstPlayerWinCount > secondPlayerWinCount ? firstPlayerWinCount : secondPlayerWinCount;
    std::cout << "Largest win count: " << winningPlayerCount << std::endl;
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_21::SetUp_Common()
{
    std::vector<std::string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    std::vector<std::string> firstPlayerArgs;
    FileHelper::SplitLine(lines[0], ": ", firstPlayerArgs);
    m_firstPlayerStartingPosition = atoi(firstPlayerArgs[1].c_str()) - 1;

    std::vector<std::string> secondPlayerArgs;
    FileHelper::SplitLine(lines[1], ": ", secondPlayerArgs);
    m_secondPlayerStartingPosition = atoi(secondPlayerArgs[1].c_str()) - 1;

    return EErrorCode::Success;
}

void CChallenge_21::DoQuantumDieRoll(
    std::vector<std::pair<int, int>> const& diceResultProbabilities,
    unsigned long long const universeCount,
    bool const isFirstPlayerActive,
    int const activePlayerPoints,
    int const activePlayerPos,
    int const otherPlayerPoints,
    int const otherPlayerPos,
    unsigned long long& outFirstPlayerWinCount,
    unsigned long long& outSecondPlayerWinCount
)
{
    for (std::pair<int, int> resultProbability : diceResultProbabilities)
    {
        int const nextActivePlayerPos = (activePlayerPos + resultProbability.first) % 10;
        int const nextActivePlayerNextPoints = activePlayerPoints + nextActivePlayerPos + 1;
        unsigned long long const nextUniverseCount = universeCount * resultProbability.second;

        if (nextActivePlayerNextPoints >= 21)
        {
            if (isFirstPlayerActive)
            {
                outFirstPlayerWinCount += nextUniverseCount;
            }
            else
            {
                outSecondPlayerWinCount += nextUniverseCount;
            }
        }
        else
        {
            DoQuantumDieRoll(
                diceResultProbabilities,
                nextUniverseCount,
                !isFirstPlayerActive,
                otherPlayerPoints,
                otherPlayerPos,
                nextActivePlayerNextPoints,
                nextActivePlayerPos,
                outFirstPlayerWinCount,
                outSecondPlayerWinCount
            );
        }
    }
}
