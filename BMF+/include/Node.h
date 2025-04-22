#ifndef NODE_H
#define NODE_H

#include <vector>

class Node
{
public:
    Node();
    Node(int id);
    Node(int id, const std::vector<int> nodeMaskBuffer);
    ~Node();

    // Returns the mask corresponding to the queryId.
    int getQueryMask(int queryId);

    void updateMaskByBit(int featureID, int oper);

    void addMask(int newMask);

    int getID();

    int getFirstMiddleIndex();
    
    int getFirstLeafIndex();

private:
    int id;
    std::vector<unsigned int> nodeMask;
    std::vector<int> middleIndex;
    std::vector<int> leafIndex;
};

#endif // NODE_H
