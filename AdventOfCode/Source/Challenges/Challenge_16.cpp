#include "Challenge_16.h"
#include "../Helpers/FileHelpers.h"

#define BIT_MASK_1 0b00000001
#define BIT_MASK_3 0b00000111
#define BIT_MASK_4 0b00001111
#define BIT_MASK_5 0b00011111
#define BIT_MASK_8 0b11111111
#define BIT_MASK_8 0b11111111

////////////////////////////////////////////////////////////////////////////////////////////////////
std::string const CChallenge_16::sm_inputFilePath = "Inputs/Input_Challenge_16.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_16::Run_FirstPart()
{
    CPacket const* mainPacket = m_packets[0];
    uint64_t const versionSum = mainPacket->GetVersionSum();
    //packet->Print("");

    std::cout << "Version sum: " << versionSum << std::endl;
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_16::Run_SecondPart()
{
    CPacket const* mainPacket = m_packets[0];
    uint64_t const messageResult = mainPacket->GetMessageResult();
    //packet->Print("");

    std::cout << "Message result: " << messageResult << std::endl;
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_16::SetUp_Common()
{
    std::string line;
    EErrorCode const readErrorCode = FileHelper::ReadFirstLine(sm_inputFilePath, line);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    std::vector<uint8_t> messageBytes;
    messageBytes.resize(line.size() / 2, 0b0000);

    for (size_t hexIdx = 0; hexIdx < line.size(); hexIdx += 2)
    {
        messageBytes[hexIdx / 2] = GetByteFromHex(line[hexIdx], line[hexIdx + 1]);
    }

    size_t byteIdx = 0;
    while (byteIdx < messageBytes.size())
    {
        int remainingBitsInByte = 8;
        m_packets.push_back(CPacket::MakePacket(messageBytes, byteIdx, remainingBitsInByte));

        if (remainingBitsInByte < 8)
        {
            ++byteIdx;
        }
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_16::CleanUp_Common()
{
    for (CPacket* packet : m_packets)
    {
        delete packet;
    }
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t CChallenge_16::GetByteFromHex(char const leftHexValue, char const rightHexValue)
{
    return (GetBitsFromHex(leftHexValue) << 4) | GetBitsFromHex(rightHexValue);
}

uint8_t CChallenge_16::GetBitsFromHex(char const hexValue)
{
    switch (hexValue)
    {
    case '0': return 0b0000;
    case '1': return 0b0001;
    case '2': return 0b0010;
    case '3': return 0b0011;
    case '4': return 0b0100;
    case '5': return 0b0101;
    case '6': return 0b0110;
    case '7': return 0b0111;
    case '8': return 0b1000;
    case '9': return 0b1001;
    case 'A': return 0b1010;
    case 'B': return 0b1011;
    case 'C': return 0b1100;
    case 'D': return 0b1101;
    case 'E': return 0b1110;
    case 'F': return 0b1111;

    default: return 0b0000;
    }
}

uint64_t CChallenge_16::GetBits(std::vector<uint8_t> const& messageBytes, size_t& byteIdx, int& remainingBitsInByte, int const bitCount)
{
    uint64_t bitMask = BIT_MASK_8 >> (8 - remainingBitsInByte);
    uint64_t resultBits = messageBytes[byteIdx];

    if (bitCount < remainingBitsInByte)
    {
        remainingBitsInByte -= bitCount;

        bitMask = bitMask >> remainingBitsInByte;
        resultBits = resultBits >> remainingBitsInByte;
    }
    else
    {
        int remainingBitCount = bitCount - remainingBitsInByte;
        remainingBitsInByte = 8;
        ++byteIdx;

        while (remainingBitCount >= 8)
        {
            bitMask = (bitMask << 8) | BIT_MASK_8;
            resultBits = (resultBits << 8) | messageBytes[byteIdx];

            ++byteIdx;
            remainingBitCount -= 8;
        }

        if (remainingBitCount > 0)
        {
            bitMask = (bitMask << 8) | BIT_MASK_8;
            resultBits = (resultBits << 8) | messageBytes[byteIdx];

            remainingBitsInByte -= remainingBitCount;
            bitMask = bitMask >> remainingBitsInByte;
            resultBits = resultBits >> remainingBitsInByte;
        }
    }

    return resultBits & bitMask;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
CChallenge_16::CPacket* CChallenge_16::CPacket::MakePacket(std::vector<uint8_t> const& messageBytes, size_t& byteIdx, int& remainingBitsInByte)
{
    uint8_t const packetVersion = static_cast<uint8_t>(GetBits(messageBytes, byteIdx, remainingBitsInByte, 3));
    uint8_t const packetType = static_cast<uint8_t>(GetBits(messageBytes, byteIdx, remainingBitsInByte, 3));

    CPacket* createdPacket;
    switch (packetType)
    {
    case 0b000: createdPacket = new CPacket_Operator_Sum(); break;
    case 0b001: createdPacket = new CPacket_Operator_Product(); break;
    case 0b010: createdPacket = new CPacket_Operator_Minimum(); break;
    case 0b011: createdPacket = new CPacket_Operator_Maximum(); break;
    case 0b100: createdPacket = new CPacket_Value(); break;
    case 0b101: createdPacket = new CPacket_Operator_GreaterThan(); break;
    case 0b110: createdPacket = new CPacket_Operator_LessThan(); break;
    case 0b111: createdPacket = new CPacket_Operator_EqualTo(); break;

    default: createdPacket = nullptr; break;
    }

    createdPacket->m_version = packetVersion;
    createdPacket->m_type = packetType;
    createdPacket->Initialize(messageBytes, byteIdx, remainingBitsInByte);
    return createdPacket;
}




////////////////////////////////////////////////////////////////////////////////////////////////////
void CChallenge_16::CPacket_Value::Initialize(std::vector<uint8_t> const& messageBytes, size_t& byteIdx, int& remainingBitsInByte)
{
    bool isDone = false;
    while (!isDone)
    {
        uint8_t const fiveBitsValue = static_cast<uint8_t>(GetBits(messageBytes, byteIdx, remainingBitsInByte, 5));
        m_litteralValue = (m_litteralValue << 4) | (fiveBitsValue & BIT_MASK_4);
        isDone = ((fiveBitsValue >> 4) & BIT_MASK_1) == 0;
    }
}

void CChallenge_16::CPacket_Value::Print(std::string const& prefix) const
{
    std::cout << prefix << "val " << m_litteralValue << " v" << static_cast<int>(m_version) << std::endl;
}

uint64_t CChallenge_16::CPacket_Value::GetVersionSum() const
{
    return m_version;
}

uint64_t CChallenge_16::CPacket_Value::GetMessageResult() const
{
    return m_litteralValue;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
CChallenge_16::CPacket_Operator::~CPacket_Operator()
{
    for (CPacket* subPacket : m_subPackets)
    {
        delete subPacket;
    }
}

void CChallenge_16::CPacket_Operator::Initialize(std::vector<uint8_t> const& messageBytes, size_t& byteIdx, int& remainingBitsInByte)
{
    bool const isLengthBitCount = GetBits(messageBytes, byteIdx, remainingBitsInByte, 1) == 0;
    if (isLengthBitCount)
    {
        uint64_t const bitCount = GetBits(messageBytes, byteIdx, remainingBitsInByte, 15);

        uint64_t const startBitPos = (byteIdx + 1) * 8 - remainingBitsInByte;
        uint64_t currentBitPos = startBitPos;

        while (currentBitPos < startBitPos + bitCount)
        {
            m_subPackets.push_back(CPacket::MakePacket(messageBytes, byteIdx, remainingBitsInByte));
            currentBitPos = (byteIdx + 1) * 8 - remainingBitsInByte;
        }
    }
    else
    {
        uint64_t const packetCount = GetBits(messageBytes, byteIdx, remainingBitsInByte, 11);
        for (uint64_t packetIdx = 0; packetIdx < packetCount; ++packetIdx)
        {
            m_subPackets.push_back(CPacket::MakePacket(messageBytes, byteIdx, remainingBitsInByte));
        }
    }
}

void CChallenge_16::CPacket_Operator::Print(std::string const& prefix) const
{
    std::cout << prefix << "ope " << static_cast<int>(m_type) << " v" << static_cast<int>(m_version) << std::endl;
    for (CPacket const* subPacket : m_subPackets)
    {
        subPacket->Print(prefix + "    ");
    }
}

uint64_t CChallenge_16::CPacket_Operator::GetVersionSum() const
{
    uint64_t versionSum = m_version;
    for (CPacket const* subPacket : m_subPackets)
    {
        versionSum += subPacket->GetVersionSum();
    }

    return versionSum;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
uint64_t CChallenge_16::CPacket_Operator_Sum::GetMessageResult() const
{
    uint64_t messageResultsSum = 0;
    for (CPacket const* subPacket : m_subPackets)
    {
        messageResultsSum += subPacket->GetMessageResult();
    }
    return messageResultsSum;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
uint64_t CChallenge_16::CPacket_Operator_Product::GetMessageResult() const
{
    uint64_t messageResultsProduct = 1;
    for (CPacket const* subPacket : m_subPackets)
    {
        messageResultsProduct *= subPacket->GetMessageResult();
    }
    return messageResultsProduct;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
uint64_t CChallenge_16::CPacket_Operator_Minimum::GetMessageResult() const
{
    uint64_t minimumMessageResult = m_subPackets[0]->GetMessageResult();
    for (size_t subPacketIdx = 1; subPacketIdx < m_subPackets.size(); ++subPacketIdx)
    {
        uint64_t const subPacketMessageResult = m_subPackets[subPacketIdx]->GetMessageResult();
        if (subPacketMessageResult < minimumMessageResult)
        {
            minimumMessageResult = subPacketMessageResult;
        }
    }
    return minimumMessageResult;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
uint64_t CChallenge_16::CPacket_Operator_Maximum::GetMessageResult() const
{
    uint64_t maximumMessageResult = m_subPackets[0]->GetMessageResult();
    for (size_t subPacketIdx = 1; subPacketIdx < m_subPackets.size(); ++subPacketIdx)
    {
        uint64_t const subPacketMessageResult = m_subPackets[subPacketIdx]->GetMessageResult();
        if (subPacketMessageResult > maximumMessageResult)
        {
            maximumMessageResult = subPacketMessageResult;
        }
    }
    return maximumMessageResult;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
uint64_t CChallenge_16::CPacket_Operator_GreaterThan::GetMessageResult() const
{
    return m_subPackets[0]->GetMessageResult() > m_subPackets[1]->GetMessageResult() ? 1 : 0;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
uint64_t CChallenge_16::CPacket_Operator_LessThan::GetMessageResult() const
{
    return m_subPackets[0]->GetMessageResult() < m_subPackets[1]->GetMessageResult() ? 1 : 0;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
uint64_t CChallenge_16::CPacket_Operator_EqualTo::GetMessageResult() const
{
    return m_subPackets[0]->GetMessageResult() == m_subPackets[1]->GetMessageResult() ? 1 : 0;
}
