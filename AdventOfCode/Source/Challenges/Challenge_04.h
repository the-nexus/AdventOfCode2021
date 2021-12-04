#pragma once

#include "Common/Challenge.h"
#include <map>

#define BINGO_CARD_SIZE 5 

class CChallenge_04 : public CChallenge
{
private:
    class CBingoCard
    {

    public:
        CBingoCard();

        bool HasValue(int const value, unsigned int& outX, unsigned int& outY) const;
        void SetValueAt(unsigned int const x, unsigned int const y, int const value);
        void SetWasPulledAt(unsigned int const x, unsigned int const y, bool const wasPulled);

        bool HasLineWinAt(unsigned int const y) const;
        bool HasColumnWinAt(unsigned int const x) const;

        int GetSumOfUnpulledValues() const;

        void SetHasWon(bool const hasWon) { m_hasWon = hasWon; }
        bool HasWon() const { return m_hasWon; }

    private:
        std::map<int, std::pair<unsigned int, unsigned int>> m_valueMap;
        int m_valueGrid[BINGO_CARD_SIZE][BINGO_CARD_SIZE];
        bool m_wasPulledGrid[BINGO_CARD_SIZE][BINGO_CARD_SIZE];
        bool m_hasWon = false;
    };

    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    EErrorCode SetUp_Common();
    EErrorCode CleanUp_Common();

    static std::string const sm_inputFilePath;
    std::vector<int> m_values;
    std::vector<CBingoCard*> m_bingoCards;
};
