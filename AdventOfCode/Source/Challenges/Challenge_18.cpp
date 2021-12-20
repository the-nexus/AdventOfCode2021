#include "Challenge_18.h"
#include "../Helpers/FileHelpers.h"



////////////////////////////////////////////////////////////////////////////////////////////////////
std::string const CChallenge_18::sm_inputFilePath = "Inputs/Input_Challenge_18.txt";
CChallenge_18::CObjectPool<CChallenge_18::CNode_Value> CChallenge_18::sm_valueNodePool = CChallenge_18::CObjectPool<CChallenge_18::CNode_Value>();
CChallenge_18::CObjectPool<CChallenge_18::CNode_Pair> CChallenge_18::sm_pairNodePool = CChallenge_18::CObjectPool<CChallenge_18::CNode_Pair>();



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_18::Run_FirstPart()
{
    CNode* numberNode = m_snailNumbers[0];
    do
    {
        numberNode->Print();
        std::cout << std::endl;
    } while (numberNode->TryReduce());

    std::cout << "No reductions remain!" << std::endl;
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_18::Run_SecondPart()
{
    return EErrorCode::NotImplemented;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_18::SetUp_Common()
{
    std::vector<std::string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_snailNumbers.reserve(lines.size());
    for (std::string const& line : lines)
    {
        size_t cursorIdx = 0;
        m_snailNumbers.push_back(CNode::MakeNode(line, cursorIdx, nullptr));
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_18::CleanUp_Common()
{
    for (CNode* snailNumber : m_snailNumbers)
    {
        delete snailNumber;
    }

    CChallenge_18::GetValueNodePool().Clear();
    CChallenge_18::GetPairNodePool().Clear();
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
CChallenge_18::CNode* CChallenge_18::CNode::MakeNode(std::string const& lineStr, size_t& cursorIdx, CNode* parentNode)
{
    CNode* createdNode = nullptr;
    if (lineStr[cursorIdx] == '[')
    {
        createdNode = CChallenge_18::GetPairNodePool().AcquireObject();
    }
    else
    {
        createdNode = CChallenge_18::GetValueNodePool().AcquireObject();
    }

    if (parentNode)
    {
        createdNode->m_parentNode = parentNode;
        createdNode->m_nestingDepth = parentNode->m_nestingDepth + 1;
    }

    createdNode->Initialize(lineStr, cursorIdx);
    return createdNode;
}

void CChallenge_18::CNode::Reset()
{
    m_parentNode = nullptr;
    m_nestingDepth = 0;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
void CChallenge_18::CNode_Value::Print() const
{
    std::cout << m_value;
}

void CChallenge_18::CNode_Value::Initialize(std::string const& lineStr, size_t& cursorIdx)
{
    m_value = static_cast<uint64_t>(lineStr[cursorIdx] - '0');
    ++cursorIdx;
}

void CChallenge_18::CNode_Value::Reset()
{
    CNode::Reset();
    m_value = 0;
}

bool CChallenge_18::CNode_Value::TryReduce()
{
    // Try to split the value node
    if (m_value >= 10)
    {
        CNode_Pair* parentPairNode = dynamic_cast<CNode_Pair*>(m_parentNode);
        bool const wasLeftChildNode = this == parentPairNode->m_leftChildNode;

        uint64_t const valueToExplode = m_value;
        uint64_t const leftExplodedValue = valueToExplode / 2;
        uint64_t const rightExplodedValue = valueToExplode - leftExplodedValue;

        CNode_Pair* createdPairNode = CChallenge_18::GetPairNodePool().AcquireObject();
        createdPairNode->m_nestingDepth = parentPairNode->m_nestingDepth + 1;
        (wasLeftChildNode ? parentPairNode->m_leftChildNode : parentPairNode->m_rightChildNode) = createdPairNode;

        CNode_Value* leftValueNode = this;
        createdPairNode->m_leftChildNode = leftValueNode;
        leftValueNode->m_parentNode = createdPairNode;
        leftValueNode->m_nestingDepth = createdPairNode->m_nestingDepth + 1;
        leftValueNode->m_value = leftExplodedValue;


        CNode_Value* rightValueNode = CChallenge_18::GetValueNodePool().AcquireObject();
        createdPairNode->m_rightChildNode = rightValueNode;
        rightValueNode->m_parentNode = createdPairNode;
        rightValueNode->m_nestingDepth = createdPairNode->m_nestingDepth + 1;
        rightValueNode->m_value = rightExplodedValue;

        return true;
    }

    return false;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
CChallenge_18::CNode_Pair::~CNode_Pair()
{
    delete m_leftChildNode;
    delete m_rightChildNode;
}

void CChallenge_18::CNode_Pair::Print() const
{
    std::cout << '[';
    m_leftChildNode->Print();
    std::cout << ',';
    m_rightChildNode->Print();
    std::cout << ']';
}

void CChallenge_18::CNode_Pair::Initialize(std::string const& lineStr, size_t& cursorIdx)
{
    ++cursorIdx; // Skip '['
    m_leftChildNode = CNode::MakeNode(lineStr, cursorIdx, this);
    ++cursorIdx; // Skip ','
    m_rightChildNode = CNode::MakeNode(lineStr, cursorIdx, this);
    ++cursorIdx; // Skip ']'
}

void CChallenge_18::CNode_Pair::Reset()
{
    CNode::Reset();
    m_leftChildNode = nullptr;
    m_rightChildNode = nullptr;
}

bool CChallenge_18::CNode_Pair::TryReduce()
{
    // Try to reduce the children
    if (m_leftChildNode->TryReduce() || m_rightChildNode->TryReduce())
    {
        return true;
    }
    // Try to explode the pair node
    else if (m_nestingDepth >= 4 && m_leftChildNode->IsValue() && m_rightChildNode->IsValue())
    {
        // Add the values to their direct neighbour values
        CNode_Value* leftValueNode = dynamic_cast<CNode_Value*>(m_leftChildNode);
        CNode_Value* rightValueNode = dynamic_cast<CNode_Value*>(m_rightChildNode);

        if (CNode_Value* leftNeighbourValueNode = FindFirstNeighbourValueNode(true))
        {
            leftNeighbourValueNode->m_value += leftValueNode->m_value;
        }

        if (CNode_Value* rightNeighbourValueNode = FindFirstNeighbourValueNode(false))
        {
            rightNeighbourValueNode->m_value += rightValueNode->m_value;
        }

        // Return the nodes and reset the tree
        CNode_Pair* parentPairNode = dynamic_cast<CNode_Pair*>(m_parentNode);
        bool const wasLeftChildNode = this == parentPairNode->m_leftChildNode;
        (wasLeftChildNode ? parentPairNode->m_leftChildNode : parentPairNode->m_rightChildNode) = leftValueNode;

        leftValueNode->m_parentNode = parentPairNode;
        leftValueNode->m_nestingDepth = parentPairNode->m_nestingDepth + 1;
        leftValueNode->m_value = 0;

        CChallenge_18::GetValueNodePool().ReturnObject(rightValueNode);
        CChallenge_18::GetPairNodePool().ReturnObject(this);
        return true;
    }

    return false;
}

CChallenge_18::CNode_Value* CChallenge_18::CNode_Pair::FindFirstNeighbourValueNode(bool const toLeft)
{
    CNode_Pair* parentNode = dynamic_cast<CNode_Pair*>(m_parentNode);
    CNode_Pair* childNode = this;

    bool targetNodeFound = false;
    do
    {
        if (!parentNode)
        {
            break;
        }

        if (toLeft)
        {
            if (parentNode->m_leftChildNode != childNode)
            {
                targetNodeFound = true;
                break;
            }
        }
        else
        {
            if (parentNode->m_rightChildNode != childNode)
            {
                targetNodeFound = true;
                break;
            }
        }

        childNode = parentNode;
        parentNode = dynamic_cast<CNode_Pair*>(parentNode->m_parentNode);

    } while (!targetNodeFound);

    if (!targetNodeFound)
    {
        return nullptr;
    }

    CNode* potentialValueNode = toLeft ? parentNode->m_leftChildNode : parentNode->m_rightChildNode;
    while (!potentialValueNode->IsValue())
    {
        potentialValueNode = toLeft ? parentNode->m_rightChildNode : parentNode->m_leftChildNode;
    }

    return dynamic_cast<CNode_Value*>(potentialValueNode);
}
