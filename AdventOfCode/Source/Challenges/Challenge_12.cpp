#include "Challenge_12.h"
#include "../Helpers/FileHelpers.h"



////////////////////////////////////////////////////////////////////////////////////////////////////
std::string const CChallenge_12::sm_inputFilePath = "Inputs/Input_Challenge_12.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_12::Run_FirstPart()
{
    int const uniquePathCount = CountUniquePaths(*m_startNode, false);
    std::cout << "Unique paths: " << uniquePathCount << std::endl;
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_12::Run_SecondPart()
{
    int const uniquePathCount = CountUniquePaths(*m_startNode, true);
    std::cout << "Unique paths: " << uniquePathCount << std::endl;
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_12::SetUp_Common()
{
    std::vector<std::string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_nodes.reserve(lines.size() + 2);

    m_startNode = &GetOrCreateNode("start");
    m_endNode = &GetOrCreateNode("end");
    m_startNode->m_visitCount = 2;

    for (std::string const& line : lines)
    {
        std::vector<std::string> args;
        FileHelper::SplitLine(line, "-", args);

        SGraphNode& srcNode = GetOrCreateNode(args[0]);
        SGraphNode& dstNode = GetOrCreateNode(args[1]);

        srcNode.m_linkedNodes.push_back(&dstNode);
        dstNode.m_linkedNodes.push_back(&srcNode);
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_12::CleanUp_Common()
{
    for (SGraphNode* node : m_nodes)
    {
        delete node;
    }
    m_nodes.clear();

    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
CChallenge_12::SGraphNode& CChallenge_12::GetOrCreateNode(std::string const& nodeName)
{
    // Return the node if it exists
    auto srcIndexIt = m_nodeIndices.find(nodeName);
    if (srcIndexIt != m_nodeIndices.end())
    {
        return *m_nodes[srcIndexIt->second];
    }

    // Create a new node
    size_t const index = m_nodes.size();
    bool const isSingleVisitOnly = nodeName[0] >= 'a' && nodeName[0] <= 'z';

    m_nodeIndices.emplace(nodeName, index);
    m_nodes.push_back(new SGraphNode(index, isSingleVisitOnly));

    return *m_nodes.back();
}

int CChallenge_12::CountUniquePaths(CChallenge_12::SGraphNode& startNode, bool const canVisitMinorNodeTwice) const
{
    int uniquePathCount = 0;
    for (SGraphNode* dstNode : startNode.m_linkedNodes)
    {
        if (dstNode->m_index == m_endNode->m_index)
        {
            // Reached the end, count path
            ++uniquePathCount;
        }
        else if (dstNode->m_isMinorNode)
        {
            // Visit a minor node, count visits to avoid passing more often than permitted
            if (dstNode->m_visitCount == 0 || (canVisitMinorNodeTwice && dstNode->m_visitCount == 1))
            {
                ++dstNode->m_visitCount;
                uniquePathCount += CountUniquePaths(*dstNode, canVisitMinorNodeTwice && dstNode->m_visitCount < 2);
                --dstNode->m_visitCount;
            }
        }
        else
        {
            // Visit a major node
            uniquePathCount += CountUniquePaths(*dstNode, canVisitMinorNodeTwice);
        }
    }
    return uniquePathCount;
}
