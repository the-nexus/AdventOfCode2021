#pragma once

#include "Common/Challenge.h"

class CChallenge_18 : public CChallenge
{
public:
    class CNode
    {
    public:
        static CNode* MakeNode(std::string const& lineStr, size_t& cursorIdx, CNode* parentNode);
        virtual void Print() const = 0;
        virtual void Initialize(std::string const& lineStr, size_t& cursorIdx) = 0;
        virtual void Reset();

        virtual bool IsValue() const { return false; }
        virtual uint64_t GetValue() const { return 0; }

        virtual bool TryReduce() = 0;

        CNode* m_parentNode = nullptr;
        uint64_t m_nestingDepth = 0;
    };

    class CNode_Value : public CNode
    {
    public:
        virtual void Print() const override;
        virtual void Initialize(std::string const& lineStr, size_t& cursorIdx) override;
        virtual void Reset() override;

        virtual bool TryReduce() override;

        virtual bool IsValue() const override { return true; }
        virtual uint64_t GetValue() const override { return m_value; }

        uint64_t m_value = 0;
    };

    class CNode_Pair : public CNode
    {
    public:
        ~CNode_Pair();
        virtual void Print() const override;
        virtual void Initialize(std::string const& lineStr, size_t& cursorIdx) override;
        virtual void Reset() override;

        virtual bool TryReduce() override;

        CNode_Value* FindFirstNeighbourValueNode(bool const toLeft);

        CNode* m_leftChildNode = nullptr;
        CNode* m_rightChildNode = nullptr;
    };

    template <class T>
    class CObjectPool
    {
    public:
        void Clear()
        {
            for (T* pooledObject : m_objectPool)
            {
                delete pooledObject;
            }
            m_objectPool.clear();
        }

        void ReturnObject(T* returnedObject)
        {
            returnedObject->Reset();
            m_objectPool.push_back(returnedObject);
        }

        T* AcquireObject()
        {
            if (m_objectPool.size() > 0)
            {
                T* acquiredObject = m_objectPool.back();
                m_objectPool.pop_back();
                return acquiredObject;
            }
            else
            {
                return new T();
            }
        }

        std::vector<T*> m_objectPool;
    };

private:
    virtual EErrorCode SetUp_FirstPart() override { return SetUp_Common(); }
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override { return CleanUp_Common(); }

    virtual EErrorCode SetUp_SecondPart() override { return SetUp_Common(); }
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override { return CleanUp_Common(); }

    EErrorCode SetUp_Common();
    EErrorCode CleanUp_Common();

    static std::string const sm_inputFilePath;
    static CObjectPool<CNode_Value> sm_valueNodePool;
    static CObjectPool<CNode_Pair> sm_pairNodePool;

    std::vector<CNode*> m_snailNumbers;

public:
    static CObjectPool<CNode_Value>& GetValueNodePool() { return sm_valueNodePool; }
    static CObjectPool<CNode_Pair>& GetPairNodePool() { return sm_pairNodePool; }
};
