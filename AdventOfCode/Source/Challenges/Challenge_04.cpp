#include "Challenge_04.h"
#include "../Helpers/FileHelpers.h"



////////////////////////////////////////////////////////////////////////////////////////////////////
std::string const CChallenge_04::sm_inputFilePath = "Inputs/Input_Challenge_04.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_04::SetUp_FirstPart()
{
    return SetUp_Common();
}

EErrorCode CChallenge_04::Run_FirstPart()
{
    CBingoCard const* bingoCardThatWon = nullptr;
    unsigned int valueIdx = 0;

    for (; valueIdx < m_values.size(); ++valueIdx)
    {
        int const value = m_values[valueIdx];

        for (CBingoCard* bingoCard : m_bingoCards)
        {
            unsigned int x = BINGO_CARD_SIZE;
            unsigned int y = BINGO_CARD_SIZE;

            if (bingoCard->HasValue(value, x, y))
            {
                bingoCard->SetWasPulledAt(x, y, true);

                if (bingoCard->HasLineWinAt(y) || bingoCard->HasColumnWinAt(x))
                {
                    bingoCardThatWon = bingoCard;
                    break;
                }
            }
        }

        if (bingoCardThatWon)
        {
            break;
        }
    }

    int score = 0;
    if (bingoCardThatWon)
    {
        int const sumOfUnpulledValues = bingoCardThatWon->GetSumOfUnpulledValues();
        int const valueThatWon = m_values[valueIdx];
        score = sumOfUnpulledValues * valueThatWon;
    }

    std::cout << "Final score: " << score << std::endl;
    return EErrorCode::Success;
}

EErrorCode CChallenge_04::CleanUp_FirstPart()
{
    return CleanUp_Common();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_04::SetUp_SecondPart()
{
    return SetUp_Common();
}

EErrorCode CChallenge_04::Run_SecondPart()
{
    CBingoCard const* lastBingoCardThatWon = nullptr;
    unsigned int valueIdx = 0;

    for (; valueIdx < m_values.size(); ++valueIdx)
    {
        bool hasAnyCardLeft = false;
        int const value = m_values[valueIdx];

        for (CBingoCard* bingoCard : m_bingoCards)
        {
            if (!bingoCard->HasWon())
            {
                unsigned int x = BINGO_CARD_SIZE;
                unsigned int y = BINGO_CARD_SIZE;

                if (bingoCard->HasValue(value, x, y))
                {
                    bingoCard->SetWasPulledAt(x, y, true);

                    if (bingoCard->HasLineWinAt(y) || bingoCard->HasColumnWinAt(x))
                    {
                        bingoCard->SetHasWon(true);
                        lastBingoCardThatWon = bingoCard;
                    }
                }

                if (!bingoCard->HasWon())
                {
                    hasAnyCardLeft = true;
                }
            }
        }

        if (!hasAnyCardLeft)
        {
            break;
        }
    }

    int score = 0;
    if (lastBingoCardThatWon)
    {
        int const sumOfUnpulledValues = lastBingoCardThatWon->GetSumOfUnpulledValues();
        int const valueThatWon = m_values[valueIdx];
        score = sumOfUnpulledValues * valueThatWon;
    }

    std::cout << "Final score: " << score << std::endl;
    return EErrorCode::Success;
}

EErrorCode CChallenge_04::CleanUp_SecondPart()
{
    return CleanUp_Common();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_04::SetUp_Common()
{
    std::vector<std::string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    std::vector<std::string> pulledValueStrings;
    FileHelper::SplitLine(lines[0], ",", pulledValueStrings);

    for (std::string const& valueString : pulledValueStrings)
    {
        m_values.push_back(atoi(valueString.c_str()));
    }

    unsigned int const bingoCardCount = (static_cast<unsigned int>(lines.size()) - 2) / (BINGO_CARD_SIZE + 1);
    m_bingoCards.reserve(bingoCardCount);

    unsigned int currentLineIdx = 2; // Start at the first line of the first grid
    while (currentLineIdx + BINGO_CARD_SIZE <= lines.size())
    {
        m_bingoCards.push_back(new CBingoCard());
        CBingoCard* bingoCard = m_bingoCards.back();

        for (unsigned int y = 0; y < BINGO_CARD_SIZE; ++y)
        {
            std::vector<std::string> lineValueStrings;
            FileHelper::SplitLine(lines[currentLineIdx + y], " ", lineValueStrings);

            unsigned int x = 0;
            for (std::string const& valueString : lineValueStrings)
            {
                if (!valueString.empty())
                {
                    int const value = atoi(valueString.c_str());
                    bingoCard->SetValueAt(x, y, value);
                    ++x;
                }
            }
        }

        currentLineIdx += BINGO_CARD_SIZE + 1; // +1 for separator line
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_04::CleanUp_Common()
{
    for (CBingoCard* bingoCard : m_bingoCards)
    {
        delete bingoCard;
    }

    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
CChallenge_04::CBingoCard::CBingoCard()
{
    for (unsigned int y = 0; y < BINGO_CARD_SIZE; ++y)
    {
        for (unsigned int x = 0; x < BINGO_CARD_SIZE; ++x)
        {
            m_valueGrid[y][x] = 0;
            m_wasPulledGrid[y][x] = false;
        }
    }
}

bool CChallenge_04::CBingoCard::HasValue(int const value, unsigned int& outX, unsigned int& outY) const
{
    auto const it = m_valueMap.find(value);
    if (it != m_valueMap.cend())
    {
        std::pair<unsigned int, unsigned int> const& coordinates = it->second;
        outX = coordinates.first;
        outY = coordinates.second;
        return true;
    }
    return false;
}

void CChallenge_04::CBingoCard::SetValueAt(unsigned int const x, unsigned int const y, int const value)
{
    m_valueMap[value] = std::pair<unsigned int, unsigned int>(x, y);
    m_valueGrid[y][x] = value;
}

void CChallenge_04::CBingoCard::SetWasPulledAt(unsigned int const x, unsigned int const y, bool const wasPulled)
{
    m_wasPulledGrid[y][x] = true;
}

bool CChallenge_04::CBingoCard::HasLineWinAt(unsigned int const y) const
{
    for (unsigned int x = 0; x < BINGO_CARD_SIZE; ++x)
    {
        if (!m_wasPulledGrid[y][x])
        {
            return false;
        }
    }
    return true;
}

bool CChallenge_04::CBingoCard::HasColumnWinAt(unsigned int const x) const
{
    for (unsigned int y = 0; y < BINGO_CARD_SIZE; ++y)
    {
        if (!m_wasPulledGrid[y][x])
        {
            return false;
        }
    }
    return true;
}

int CChallenge_04::CBingoCard::GetSumOfUnpulledValues() const
{
    int sum = 0;
    for (unsigned int y = 0; y < BINGO_CARD_SIZE; ++y)
    {
        for (unsigned int x = 0; x < BINGO_CARD_SIZE; ++x)
        {
            if (!m_wasPulledGrid[y][x])
            {
                sum += m_valueGrid[y][x];
            }
        }
    }
    return sum;
}
