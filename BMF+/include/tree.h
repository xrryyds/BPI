#ifndef TREE_H
#define TREE_H

#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include "Node.h"
#include <fstream>
#include <string>
#include <cmath>
#include <tuple>
/*
乘法转为位运算
*/

class BMFTree
{
public:
    /***************TEST CODE*******************************/
    void Test_addNewData();

    void Test_Query();
    
    void Test_creatFeature();
    /*****************************************************/
    BMFTree();

    ~BMFTree();

    void query(std::vector<int> featureList,  std::vector<std::string> &resultData);
    
    void ArticulatedSearch(std::vector<int> featureList, long long token_i, std::vector<std::string> &resultData, long long &token_j);

    void showParam();

    void showTree();

    void init(int theNumberOfAllData);

    void updataTree(int theNumberOfAllData);

    void creatFeature(std::vector<std::string> featureName,std::vector<int> attribute,std::vector<std::string> keywords);

private:
    void saveParam();

    unsigned int HybridQuery(std::vector<unsigned int> maskList);

    void generateIndex(int featureID, std::vector<unsigned int> leafMaskBuffer, std::vector<unsigned int> middleMaskBuffer);

    void updataIndex(int featureID, std::vector<unsigned int> leafMaskBuffer, std::vector<unsigned int> middleMaskBuffer, int theNumberOfAllRoot, int middleID,int leafID);

    void putBitOn(int featureID, int dataID);

    void putBitOff(int featureID, int dataID);

    void inintParam();

    void updataParam();

    void readData(std::string fileName, int beginIndex, int endIndex, std::vector<std::string> &resultData);

    void readDataForQuery(std::string fileName, std::vector<int> indexList, std::vector<std::string> &resultData);

    bool evenIsOn(std::vector<std::string> data, int featureID); //*D

    void updateDataIntoFile(std::vector<std::vector<std::string>> data);

    int getMaskIndex(int root, int middle, int leaf, int feature,int middleIndex);
    
    void parseToken(long long token, unsigned int &featureID, unsigned int &offset_root, unsigned int &offset_middle, unsigned int &offset_leaf, unsigned int &offset_data);
    
    void genToken(int featureID, long long &token);

private:
    std::vector<std::vector<unsigned int>> BMFRootNode;
    std::vector<std::vector<unsigned int>> BMFMiddleNode;
    std::vector<std::vector<unsigned int>> BMFLeafNode;
    std::vector<std::string> featureSequence;
    std::vector<int> BMFAttribute;
    std::vector<std::string> BMFKeywords;
    std::vector<std::vector<int>> firstLeafIndex;
    std::vector<std::vector<int>> firstMiddleIndex;
    std::vector<std::string> EBF;
    std::string dataFileName;
    int theLengthOfEachData;
    int theNumberOfData;
    int theNumberOfRoot;
    int theNumberOfLeaf;
    int theNumberOfMiddleNode;
    int theNumberOfNode;
    int theNumberOfFeature;
    const int middleType  = 1;
    const int leafType = 2;
    const long long mask28 = (1LL << 28) - 1; 
    const long long mask20 = (1LL << 20) - 1; 
    const long long mask5 = (1LL << 5) - 1;   
};

#endif