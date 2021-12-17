#pragma once

#include "Common/Challenge.h"

class CChallenge_16 : public CChallenge
{
private:
    class CPacket
    {
    public:
        static CPacket* MakePacket(std::vector<uint8_t> const& messageBytes, size_t& byteIdx, int& remainingBitsInByte);

        virtual void Initialize(std::vector<uint8_t> const& messageBytes, size_t& byteIdx, int& remainingBitsInByte) = 0;
        virtual void Print(std::string const& prefix) const = 0;
        virtual uint64_t GetVersionSum() const = 0;
        virtual uint64_t GetMessageResult() const = 0;

        uint8_t m_version = 0b000;
        uint8_t m_type = 0b000;
    };

    class CPacket_Value : public CPacket
    {
    public:
        virtual void Initialize(std::vector<uint8_t> const& messageBytes, size_t& byteIdx, int& remainingBitsInByte) override;
        virtual void Print(std::string const& prefix) const override;
        virtual uint64_t GetVersionSum() const override;
        virtual uint64_t GetMessageResult() const override;

    protected:
        uint64_t m_litteralValue = 0;
    };

    class CPacket_Operator : public CPacket
    {
    public:
        ~CPacket_Operator();
        virtual void Initialize(std::vector<uint8_t> const& messageBytes, size_t& byteIdx, int& remainingBitsInByte) override;
        virtual void Print(std::string const& prefix) const override;
        virtual uint64_t GetVersionSum() const override;

    protected:
        std::vector<CPacket*> m_subPackets;
    };

    class CPacket_Operator_Sum : public CPacket_Operator
    {
        virtual uint64_t GetMessageResult() const override;
    };

    class CPacket_Operator_Product : public CPacket_Operator
    {
        virtual uint64_t GetMessageResult() const override;
    };

    class CPacket_Operator_Minimum : public CPacket_Operator
    {
        virtual uint64_t GetMessageResult() const override;
    };

    class CPacket_Operator_Maximum : public CPacket_Operator
    {
        virtual uint64_t GetMessageResult() const override;
    };

    class CPacket_Operator_GreaterThan : public CPacket_Operator
    {
        virtual uint64_t GetMessageResult() const override;
    };

    class CPacket_Operator_LessThan : public CPacket_Operator
    {
        virtual uint64_t GetMessageResult() const override;
    };

    class CPacket_Operator_EqualTo : public CPacket_Operator
    {
        virtual uint64_t GetMessageResult() const override;
    };

    virtual EErrorCode SetUp_FirstPart() override { return SetUp_Common(); }
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override { return CleanUp_Common(); }

    virtual EErrorCode SetUp_SecondPart() override { return SetUp_Common(); }
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override { return CleanUp_Common(); }

    EErrorCode SetUp_Common();
    EErrorCode CleanUp_Common();

    static uint8_t GetByteFromHex(char const leftHexValue, char const rightHexValue);
    static uint8_t GetBitsFromHex(char const hexValue);
    static uint64_t GetBits(std::vector<uint8_t> const& messageBytes, size_t& currentByteIdx, int& remainingBitsInByte, int const bitCount);

    static std::string const sm_inputFilePath;
    std::vector<CPacket*> m_packets;
};
