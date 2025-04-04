#include "../include/Node.h"

Node::Node(int tid)
{
    id = tid;
}

Node::Node(int tid, std::vector<int> nodeMaskBuffer)
{
    id = tid;
    for (int i = 0; i < nodeMaskBuffer.size(); i++)
    {
        nodeMask.push_back(nodeMaskBuffer[i]);
    }
}


Node::~Node() {} // 析构函数的实现

int Node::getQueryMask(int queryId)
{
    return nodeMask[queryId];
}

void Node::updateMaskByBit(int featureID, int oper)
{
    nodeMask[featureID] |= oper;
}

void Node::addMask(int newMask)
{
    nodeMask.push_back(newMask);
}

int Node::getID()
{
    return Node::id;
}

