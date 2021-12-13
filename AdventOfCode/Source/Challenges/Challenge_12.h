#pragma once

#include "Common/Challenge.h"
#include <map>

class CChallenge_12 : public CChallenge
{
private:
    struct SGraphNode
    {
        SGraphNode(size_t const index, bool const isSingleVisitOnly)
            : m_index(index)
            , m_isMinorNode(isSingleVisitOnly)
        {}

        size_t m_index = 0;
        bool m_isMinorNode = false;
        int m_visitCount = 0;
        std::vector<SGraphNode*> m_linkedNodes;
    };

    virtual EErrorCode SetUp_FirstPart() override { return SetUp_Common(); }
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override { return CleanUp_Common(); }

    virtual EErrorCode SetUp_SecondPart() override { return SetUp_Common(); }
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override { return CleanUp_Common(); }

    EErrorCode SetUp_Common();
    EErrorCode CleanUp_Common();

    SGraphNode& GetOrCreateNode(std::string const& nodeName);
    int CountUniquePaths(SGraphNode& startNode, bool const canVisitMinorNodeTwice) const;

    static std::string const sm_inputFilePath;
    std::vector<SGraphNode*> m_nodes;
    std::map<std::string, size_t> m_nodeIndices;

    SGraphNode* m_startNode = nullptr;
    SGraphNode* m_endNode = nullptr;
};
