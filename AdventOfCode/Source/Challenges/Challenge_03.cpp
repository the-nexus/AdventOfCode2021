#include "Challenge_03.h"
#include "../Helpers/FileHelpers.h"



////////////////////////////////////////////////////////////////////////////////////////////////////
std::string const CChallenge_03::sm_inputFilePath = "Inputs/Input_Challenge_03.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_03::SetUp_FirstPart()
{
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, m_binaryNumbers);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_03::Run_FirstPart()
{
    unsigned int gammaRate = 0;
    unsigned int epsilonRate = 0;

    if (!m_binaryNumbers.empty())
    {
        unsigned int binaryOffset = 1;
        unsigned int binaryDigitCount = static_cast<unsigned int>(m_binaryNumbers[0].size());

        for (unsigned int i = 0; i < binaryDigitCount; ++i)
        {
            unsigned int digitSum = 0;
            unsigned int majoritySum = 0;
            unsigned int const digitIdx = binaryDigitCount - i - 1;

            SumBinaryDigits(m_binaryNumbers, digitIdx, digitSum, majoritySum);

            if (digitSum >= majoritySum)
            {
                gammaRate += binaryOffset;
            }
            else
            {
                epsilonRate += binaryOffset;
            }

            binaryOffset *= 2;
        }
    }

    unsigned int const powerConsumption = gammaRate * epsilonRate;
    std::cout << "Power consumption: " << powerConsumption << std::endl;
    return EErrorCode::Success;
}

EErrorCode CChallenge_03::CleanUp_FirstPart()
{
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_03::SetUp_SecondPart()
{
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, m_binaryNumbers);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_03::Run_SecondPart()
{
    std::string majorityNumber = "";
    FindNumberBySum(m_binaryNumbers, 0, true, majorityNumber);

    std::string minorityNumber = "";
    FindNumberBySum(m_binaryNumbers, 0, false, minorityNumber);

    unsigned int oxygenGeneratorRating = 0;
    unsigned int co2ScrubberRating = 0;
    unsigned int binaryOffset = 1;
    unsigned int binaryDigitCount = static_cast<unsigned int>(m_binaryNumbers[0].size());

    for (unsigned int i = 0; i < binaryDigitCount; ++i)
    {
        unsigned int const digitIdx = binaryDigitCount - i - 1;

        if (majorityNumber[digitIdx] == '1')
        {
            oxygenGeneratorRating += binaryOffset;
        }

        if (minorityNumber[digitIdx] == '1')
        {
            co2ScrubberRating += binaryOffset;
        }

        binaryOffset *= 2;
    }

    unsigned int const lifeSupportRating = oxygenGeneratorRating * co2ScrubberRating;
    std::cout << "Life support rating: " << lifeSupportRating << std::endl;
    return EErrorCode::Success;
}

EErrorCode CChallenge_03::CleanUp_SecondPart()
{
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
void CChallenge_03::SumBinaryDigits(std::vector<std::string> const& binaryNumbers, unsigned int const digitIdx, unsigned int& digitSum, unsigned int& majoritySum)
{
    digitSum = 0;
    majoritySum = static_cast<unsigned int>(ceilf(binaryNumbers.size() / 2.f));

    for (std::string const& binaryNumber : binaryNumbers)
    {
        if (binaryNumber[digitIdx] == '1')
        {
            ++digitSum;
        }
    }
}

void CChallenge_03::FindNumberBySum(std::vector<std::string> const& binaryNumbers, unsigned int const digitIdx, bool const useMajority, std::string& outNumber)
{
    if (!binaryNumbers.empty())
    {
        if (binaryNumbers.size() == 1)
        {
            outNumber = binaryNumbers[0];
        }
        else if (digitIdx < static_cast<unsigned int>(binaryNumbers[0].size()))
        {
            unsigned int digitSum = 0;
            unsigned int majoritySum = 0;
            SumBinaryDigits(binaryNumbers, digitIdx, digitSum, majoritySum);

            bool const oneHasMajority = digitSum >= majoritySum;
            char const chosenChar = oneHasMajority == useMajority ? '1' : '0';

            std::vector<std::string> reducedBinaryNumbers;
            for (std::string const& binaryNumber : binaryNumbers)
            {
                if (binaryNumber[digitIdx] == chosenChar)
                {
                    reducedBinaryNumbers.push_back(binaryNumber);
                }
            }

            FindNumberBySum(reducedBinaryNumbers, digitIdx + 1, useMajority, outNumber);
        }
    }
}
