#include "../include/tree.h"
#include <bitset>
#include "../include/IO.h"
#include "../include/tools.h"
#include <chrono>
#include <cstdint>
using namespace std;
BMFTree::BMFTree()
{
}

BMFTree::~BMFTree()
{
    // 析构函数的实现
}

void BMFTree::inintParam()
{
    std::ifstream ifs;
    ifs.open("../file/paramFile/param.txt", std::ios::in);
    if (ifs.is_open())
    {
        ifs >> theNumberOfData;
        ifs >> theNumberOfFeature;
        ifs >> theLengthOfEachData;
        ifs >> dataFileName;
        theNumberOfLeaf = std::ceil(static_cast<float>(theNumberOfData) / static_cast<float>(32));
        theNumberOfMiddleNode = std::ceil(static_cast<float>(theNumberOfLeaf) / static_cast<float>(32));
        theNumberOfRoot = std::ceil(static_cast<float>(theNumberOfMiddleNode) / static_cast<float>(32));
        theNumberOfNode = theNumberOfLeaf + theNumberOfMiddleNode + theNumberOfRoot;
        ifs.close();
    }
    ifs.open("../file/paramFile/keywords.txt", std::ios::in);
    if (ifs.is_open())
    {
        std::string str;
        int tmp;
        for (int i = 0; i < theNumberOfFeature; i++)
        {
            ifs >> tmp;
            ifs >> str;
            BMFAttribute.push_back(tmp);
            BMFKeywords.push_back(str);
        }
        ifs.close();
    }

    ifs.open("../file/paramFile/featureName.txt", std::ios::in);
    if (ifs.is_open())
    {
        std::string str;
        for (int i = 0; i < theNumberOfFeature; i++)
        {
            ifs >> str;
            featureSequence.push_back(str);
        }
        ifs.close();
    }

    ifs.open("../file/paramFile/EBF.txt", std::ios::in);
    if (ifs.is_open())
    {
        std::string str;
        for (int i = 0; i < theNumberOfFeature; i++)
        {
            ifs >> str;
            EBF.push_back(str);
        }
        ifs.close();
    }

    for (int i = 0; i < featureSequence.size(); i++)
    {
        std::string fileName = "../file/nodeFile/" + featureSequence[i] + "_rootNode";
        std::string line;
        std::vector<unsigned int> buffer;
        BMFRootNode.push_back(std::vector<unsigned int>());
        readMask(fileName, buffer);
        for (int j = 0; j < buffer.size(); j++)
        {
            BMFRootNode[i].push_back(buffer[j]);
        }
        buffer.clear();

        fileName = "../file/nodeFile/" + featureSequence[i] + "_middleNode";
        BMFMiddleNode.push_back(std::vector<unsigned int>());
        readMask(fileName, buffer);
        for (int j = 0; j < buffer.size(); j++)
        {
            BMFMiddleNode[i].push_back(buffer[j]);
        }
        buffer.clear();

        fileName = "../file/nodeFile/" + featureSequence[i] + "_leafNode";
        BMFLeafNode.push_back(std::vector<unsigned int>());
        readMask(fileName, buffer);
        // cout<<buffer.size()<<endl;
        for (int j = 0; j < buffer.size(); j++)
        {
            BMFLeafNode[i].push_back(buffer[j]);
            if(i == 0)cout<<buffer[j]<<endl;
        }
        buffer.clear();

        fileName = "../file/firstIndexFile/Index_" + std::to_string(i);
        ifs.open(fileName);
        if (!ifs.is_open())
        {
            cout << "intit firstIndex error" << endl;
        }
        firstLeafIndex.push_back(std::vector<int>());
        firstMiddleIndex.push_back(std::vector<int>());
        for (int j = 0; j < theNumberOfRoot; j++)
        {
            ifs >> line;
            // cout << line << endl;
            firstMiddleIndex[i].push_back(std::stoll(line));
            // cout << line << endl;
            ifs >> line;
            firstLeafIndex[i].push_back(std::stoll(line));
        }
        ifs.close();
    }
    cout << "initFinished" << endl;
}

void BMFTree::updataParam()
{
    theNumberOfLeaf = std::ceil(static_cast<float>(theNumberOfData) / static_cast<float>(32));
    theNumberOfMiddleNode = std::ceil(static_cast<float>(theNumberOfLeaf) / static_cast<float>(32));
    theNumberOfRoot = std::ceil(static_cast<float>(theNumberOfMiddleNode) / static_cast<float>(32));
    theNumberOfNode = theNumberOfLeaf + theNumberOfMiddleNode + theNumberOfRoot;
}

void BMFTree::showParam()
{
    std::cout << "theNumberOfData:" << theNumberOfData << std::endl;
    std::cout << "theNumberOfRoot:" << theNumberOfRoot << std::endl;
    std::cout << "theNumberOfMiddleNode:" << theNumberOfMiddleNode << std::endl;
    std::cout << "theNumberOfLeaf:" << theNumberOfLeaf << std::endl;
    std::cout << "theNumberOfNode:" << theNumberOfNode << std::endl;
    std::cout << "theNumberOfFeature:" << theNumberOfFeature << std::endl;
    std::cout << "theLengthOfEachData:" << theLengthOfEachData << std::endl;

    std::cout << "eventList:" << std::endl;
    for (int i = 0; i < theNumberOfFeature; i++)
    {
        std::cout << " **********" << featureSequence[i] << std::endl;
    }
}

void BMFTree::generateIndex(int featureID, std::vector<unsigned int> leafMaskBuffer, std::vector<unsigned int> middleMaskBuffer)
{

    int cnt = 0;
    for (int i = 0; i < middleMaskBuffer.size(); i++)
    {
        if (middleMaskBuffer[i] != 0)
        {
            if (firstMiddleIndex[featureID][i / 32] == -1)
            {
                firstMiddleIndex[featureID][i / 32] = cnt;
            }
            cnt++;
        }
    }

    cnt = 0;
    for (int i = 0; i < leafMaskBuffer.size(); i++)
    {
        if (leafMaskBuffer[i] != 0)
        {
            if (firstLeafIndex[featureID][i / (32 * 32)] == -1)
            {
                firstLeafIndex[featureID][i / (32 * 32)] = cnt;
            }
            cnt++;
        }
    }

    std::ofstream ofs;
    ofs.open("../file/firstIndexFile/Index_" + std::to_string(featureID), std::ios::out);
    if (ofs.is_open())
    {
        for (int i = 0; i < theNumberOfRoot; i++)
        {
            ofs << firstMiddleIndex[featureID][i] << "\n";
            ofs << firstLeafIndex[featureID][i] << "\n";
        }
        ofs.close();
    }
}

void BMFTree::updataIndex(int featureID, std::vector<unsigned int> leafMaskBuffer, std::vector<unsigned int> middleMaskBuffer, int theNumberOfAllRoot, int middleID, int leafID)
{
    for (int i = theNumberOfRoot; i < theNumberOfAllRoot; i++)
    {
        firstLeafIndex[featureID].push_back(-1);
        firstMiddleIndex[featureID].push_back(-1);
    }

    int cnt = BMFMiddleNode[featureID].size();
    for (int i = 0; i < middleMaskBuffer.size(); i++)
    {
        if (middleMaskBuffer[i] != 0)
        {
            if (firstMiddleIndex[featureID][(i + middleID) / 32] == -1)
            {
                firstMiddleIndex[featureID][(i + middleID) / 32] = cnt;
            }
            cnt++;
        }
    }

    cnt = BMFLeafNode[featureID].size();
    for (int i = 0; i < leafMaskBuffer.size(); i++)
    {
        if (leafMaskBuffer[i] != 0)
        {
            if (firstLeafIndex[featureID][(i + leafID) / (32 * 32)] == -1)
            {
                firstLeafIndex[featureID][((i + leafID)) / (32 * 32)] = cnt;
            }
            cnt++;
        }
    }

    std::fstream ofs;
    ofs.open("../file/firstIndexFile/Index_" + std::to_string(featureID), std::ios::in | std::ios::out);
    if (ofs.is_open())
    {

        for (int i = 0; i < theNumberOfAllRoot; i++)
        {
            ofs << firstMiddleIndex[featureID][i] << "\n";
            ofs << firstLeafIndex[featureID][i] << "\n";
        }
        ofs.close();
    }
}

void BMFTree::creatFeature(std::vector<std::string> featureName, std::vector<int> attribute, std::vector<std::string> keywords)
{
    // int cnt = 0;
    std::vector<std::string> datastr;
    std::vector<std::string> tEBF;
    std::vector<std::vector<std::string>> data;
    std::vector<unsigned int> leafMaskBuffer;
    std::vector<unsigned int> middleMaskBuffer;
    std::vector<unsigned int> rootMaskBuffer;
    std::vector<int> middleIndex;
    std::vector<int> leafIndex;
    unsigned int maskBuffer = 0;
    int k = 31;
    std::ofstream ofs;
    ofs.open("../file/paramFile/featureName.txt", std::ios::app);
    if (ofs.is_open())
    {
        for (int i = 0; i < featureName.size(); i++)
        {
            ofs << featureName[i] << "\n";
        }
        ofs.close();
    }

    readData(dataFileName, 0, theNumberOfData, datastr);
    transformStrToData(datastr, data);
    for (int i = 0; i < featureName.size(); i++)
    {
        firstLeafIndex.push_back(std::vector<int>(theNumberOfRoot, -1));
        firstMiddleIndex.push_back(std::vector<int>(theNumberOfRoot, -1));

        rootMaskBuffer.clear();
        leafMaskBuffer.clear();
        middleMaskBuffer.clear();
        middleIndex.clear();
        leafIndex.clear();
        k = 31;
        maskBuffer = 0;
        for (int j = 0; j < data.size(); j++)
        {
            if (data[j][attribute[i]] == keywords[i])
            {
                maskBuffer |= (1 << k);
            }
            k--;
            if (k == -1)
            {
                leafMaskBuffer.push_back(maskBuffer);
                k = 31;
                maskBuffer = 0;
            }
        }
        if (theNumberOfData % 32 != 0)
        {
            leafMaskBuffer.push_back(maskBuffer);
        }
        k = 31;
        maskBuffer = 0;
        for (int j = 0; j < leafMaskBuffer.size(); j++)
        {
            if (leafMaskBuffer[j] != 0)
            {
                maskBuffer = maskBuffer | (1 << k);
            }
            k--;
            if (k == -1)
            {
                middleMaskBuffer.push_back(maskBuffer);
                k = 31;
                maskBuffer = 0;
            }
        }
        if (leafMaskBuffer.size() % 32 != 0)
        {
            middleMaskBuffer.push_back(maskBuffer);
        }

        k = 31;
        maskBuffer = 0;
        for (int j = 0; j < middleMaskBuffer.size(); j++)
        {
            if (middleMaskBuffer[j] != 0)
            {
                maskBuffer = maskBuffer | (1 << k);
            }
            k--;
            if (k == -1)
            {
                rootMaskBuffer.push_back(maskBuffer);
                k = 31;
                maskBuffer = 0;
            }
        }
        if (middleMaskBuffer.size() % 32 != 0)
        {
            rootMaskBuffer.push_back(maskBuffer);
        }
        std::string rootFile = "../file/nodeFile/" + featureName[i] + "_rootNode";
        std::string middleFile = "../file/nodeFile/" + featureName[i] + "_middleNode";
        std::string leafFile = "../file/nodeFile/" + featureName[i] + "_leafNode";
        std::ofstream ofs;
        std::string tmpFilter = "";

        ofs.open(rootFile, std::ios::out | std::ios::binary);
        if (ofs.is_open())
        {

            for (size_t k = 0; k < rootMaskBuffer.size(); k++)
            {
                if (k == rootMaskBuffer.size() - 1 || rootMaskBuffer[k] != 0)
                {
                    ofs.write(reinterpret_cast<char *>(&rootMaskBuffer[k]), sizeof(unsigned int));
                }
                if (rootMaskBuffer[k] != 0)
                {
                    tmpFilter.append(1, '1');
                }
                else
                {
                    tmpFilter.append(1, '0');
                }
            }
            ofs.close();
        }

        tEBF.push_back(tmpFilter);

        ofs.open(middleFile, std::ios::out | std::ios::binary);
        if (ofs.is_open())
        {
            for (size_t k = 0; k < middleMaskBuffer.size(); k++)
            {
                if (k == middleMaskBuffer.size() - 1 || middleMaskBuffer[k] != 0)
                {
                    ofs.write(reinterpret_cast<char *>(&middleMaskBuffer[k]), sizeof(unsigned int));
                }
            }
            ofs.close();
        }
        ofs.open(leafFile, std::ios::out | std::ios::binary);
        if (ofs.is_open())
        {
            for (size_t k = 0; k < leafMaskBuffer.size(); k++)
            {
                if (k == leafMaskBuffer.size() - 1 || leafMaskBuffer[k] != 0)
                {
                    ofs.write(reinterpret_cast<char *>(&leafMaskBuffer[k]), sizeof(unsigned int));
                }
            }

            ofs.close();
        }
        generateIndex(theNumberOfFeature + i, leafMaskBuffer, middleMaskBuffer);
    }
    theNumberOfFeature += featureName.size();
    ofs.open("../file/paramFile/keywords.txt", std::ios::app);
    if (ofs.is_open())
    {
        for (int i = 0; i < featureName.size(); i++)
        {
            ofs << attribute[i] << "\n";
            ofs << keywords[i] << "\n";
        }
        ofs.close();
    }

    ofs.open("../file/paramFile/EBF.txt", std::ios::app);
    if (ofs.is_open())
    {
        for (int i = 0; i < featureName.size(); i++)
        {
            ofs << tEBF[i] << "\n";
        }
        ofs.close();
    }
    // cout<<cnt<<endl;
}

void BMFTree::saveParam()
{
    std::ofstream ofs;
    ofs.open("../file/paramFile/param.txt", std::ios::out);
    if (ofs.is_open())
    {
        ofs << theNumberOfData << "\n";
        ofs << theNumberOfFeature << "\n";
        ofs << theLengthOfEachData << "\n";
        ofs << dataFileName << "\n";
        ofs.close();
    }
}

void BMFTree::init(int theNumberOfAllData)
{
    inintParam();
    if (theNumberOfAllData > theNumberOfData)
    {
        auto beforeTime = std::chrono::steady_clock::now();
        updataTree(theNumberOfAllData);
        auto afterTime = std::chrono::steady_clock::now();

        double duration_millsecond = std::chrono::duration<double, std::milli>(afterTime - beforeTime).count();
        std::cout << duration_millsecond << "OOms" << std::endl;
    }
    showParam();
}

unsigned int BMFTree::HybridQuery(std::vector<unsigned int> maskList)
{
    unsigned int result = maskList[0];
    for (int i = 1; i < maskList.size(); i++)
    {
        result = result & maskList[i];
    }
    return result;
}

// customized
bool BMFTree::evenIsOn(std::vector<std::string> data, int featureID)
{
    switch (featureID)
    {
    case 0:
        return data[5] >= "5000000000";
        break;
    case 1:
        return data[0] == "0x32be343b94f860124dc4fee278fdcbd38c102d88";
        break;
    default:
        return 0;
        break;
    }
}

int BMFTree::getMaskIndex(int root, int middle, int leaf, int feature, int middleIndex)
{
    if (middle == -1)
    {
        int cnt = 0;
        for (int i = 0; i < root; i++)
        {
            if (EBF[feature][i] == '1')
            {
                cnt++;
            }
        }
        return cnt;
    }
    if (leaf == -1)
    {
        std::vector<int> indexBuffer = findAllOn(BMFRootNode[feature][root]);
        for (int i = 0; i < indexBuffer.size(); i++)
        {
            if (indexBuffer[i] == middle)
            {
                return firstMiddleIndex[feature][root] + i;
            }
        }
    }
    int cnt = 0;
    for (int i = firstMiddleIndex[feature][root]; i < middleIndex; i++)
    {
        cnt += findAllOn(BMFMiddleNode[feature][i]).size();
    }
    std::vector<int> indexBuffer = findAllOn(BMFMiddleNode[feature][middleIndex]);
    for (int i = 0; i < indexBuffer.size(); i++)
    {
        if (indexBuffer[i] == leaf)
        {
            return firstLeafIndex[feature][root] + cnt + i;
        }
    }
    return -1;
}

void BMFTree::showTree()
{
    std::cout << "root:"
              << "\n";
    for (int e = 1; e < theNumberOfFeature; e++)
    {
        for (int i = 0; i < theNumberOfRoot; i++)
        {
            std::bitset<sizeof(int) * 8> bits(BMFRootNode[e][i]);
            std::cout << bits.to_string() << " ";
        }
        std::cout << "\n";
    }

    std::cout << "middleNode:"
              << "\n";
    for (int e = 0; e < theNumberOfFeature; e++)
    {
        for (int i = 0; i < BMFMiddleNode[e].size(); i++)
        {
            std::bitset<sizeof(int) * 8> bits(BMFMiddleNode[e][i]);
            std::cout << bits.to_string() << " ";
        }
        std::cout << "\n";
    }

    std::cout << "leafNode:"
              << "\n";
    for (int e = 1; e < theNumberOfFeature; e++)
    {

        std::bitset<sizeof(int) * 8> bits(BMFLeafNode[e][0]);
        std::cout << bits.to_string() << " ";

        std::cout << "\n";
    }
}

void BMFTree::putBitOn(int featureID, int dataID) {}

void BMFTree::putBitOff(int featureID, int dataID) {}

void BMFTree::readDataForQuery(std::string fileName, std::vector<int> indexList, std::vector<std::string> &resultData)
{
    std::ifstream file(fileName, std::ios::binary);
    vector<vector<std::string>> data;
    if (file)
    {
        file.seekg(indexList[0] * theLengthOfEachData, std::ios::beg);
        for (int i = 0; i < indexList.size(); i++)
        {
            file.seekg(indexList[i] * theLengthOfEachData, std::ios::beg);
            std::vector<char> buffer(theLengthOfEachData);
            file.read(buffer.data(), theLengthOfEachData);
            std::string line(buffer.begin(), buffer.end());
            resultData.push_back(line);
        }
        file.close();
    }
}

void BMFTree::query(std::vector<int> featureList, std::vector<std::string> &resultData)
{
    std::vector<int> tmp, tmp0, tmp1, tmp2;
    std::vector<int> result;
    std::vector<unsigned int> maskBuffer, middleIndex;
    int indexLeaf;

    for (int i = 0; i < theNumberOfRoot; i++)
    {
        bool flag = true;
        for (int f = 0; f < featureList.size(); f++)
        {
            if (EBF[f][i] == '0')
            {
                flag = false;
            }
        }
        if (flag)
        {
            tmp.push_back(i);
        }
    }
    for (int i = 0; i < tmp.size(); i++)
    {
        indexLeaf = 0;
        maskBuffer.clear();
        for (int f = 0; f < featureList.size(); f++)
        {
            maskBuffer.push_back(BMFRootNode[featureList[f]][getMaskIndex(tmp[i], -1, -1, featureList[f], -1)]);
        }
        tmp0 = findAllOn(HybridQuery(maskBuffer));
        for (int j = 0; j < tmp0.size(); j++)
        {
            maskBuffer.clear();
            middleIndex.clear();
            for (int f = 0; f < featureList.size(); f++)
            {
                int tmp = getMaskIndex(i, tmp0[j], -1, featureList[f], -1);
                middleIndex.push_back(tmp);
                maskBuffer.push_back(BMFMiddleNode[featureList[f]][tmp]);
            }
            tmp1 = findAllOn(HybridQuery(maskBuffer));
            for (int k = 0; k < tmp1.size(); k++)
            {
                maskBuffer.clear();
                for (int f = 0; f < featureList.size(); f++)
                {
                    maskBuffer.push_back(BMFLeafNode[featureList[f]][getMaskIndex(i, tmp0[j], tmp1[k], featureList[f], middleIndex[f])]);
                }
                tmp2 = findAllOn(HybridQuery(maskBuffer));
                for (int l = 0; l < tmp2.size(); l++)
                {
                    result.push_back(tmp[i] << 15 | tmp0[j] << 10 | tmp1[k] << 5 | tmp2[l]);
                }
                tmp2.clear();
            }
            indexLeaf += tmp1.size();
            tmp1.clear();
        }
        tmp0.clear();
    }
    readDataForQuery(dataFileName, result, resultData);
}

void BMFTree::readData(std::string fileName, int begainIndex, int endIndex, std::vector<std::string> &resultData)
{
    std::ifstream file(fileName, std::ios::binary);
    vector<vector<std::string>> data;
    if (file)
    {
        file.seekg(begainIndex * theLengthOfEachData, std::ios::beg);
        for (int i = begainIndex; i < endIndex; i++)
        {
            std::vector<char> buffer(theLengthOfEachData);
            file.read(buffer.data(), theLengthOfEachData);
            std::string line(buffer.begin(), buffer.end());
            resultData.push_back(line);
        }
        file.close();
    }
}
void BMFTree::updataTree(int theNumberOfAllData)
{
    cout << "begainUnpdata" << endl;
    std::vector<std::string> datastr;
    std::vector<std::vector<std::string>> data;
    std::vector<unsigned int> leafMaskBuffer;
    std::vector<unsigned int> middleMaskBuffer;
    std::vector<unsigned int> rootMaskBuffer;
    std::vector<int> middleIndex;
    std::vector<int> leafIndex;
    unsigned int maskBuffer = 0;
    int k;
    std::fstream ofs;
    unsigned allRoot = std::ceil(static_cast<float>(std::ceil(static_cast<float>(std::ceil(static_cast<float>(theNumberOfAllData) / static_cast<float>(32))) / static_cast<float>(32))) / static_cast<float>(32));
    int middleID = theNumberOfMiddleNode;
    int leafID = theNumberOfLeaf;
    if (theNumberOfData % 32 != 0)
    {
        middleID--;
        leafID--;
    }
    readData(dataFileName, theNumberOfData, theNumberOfAllData, datastr);
    transformStrToData(datastr, data);
    for (int i = 0; i < theNumberOfFeature; i++)
    {
        rootMaskBuffer.clear();
        leafMaskBuffer.clear();
        middleMaskBuffer.clear();
        middleIndex.clear();
        leafIndex.clear();
        k = 31 - (theNumberOfData % 32);
        maskBuffer = 0;
        for (int j = 0; j < (theNumberOfAllData - theNumberOfData); j++)
        {
            if (data[j][BMFAttribute[i]] == BMFKeywords[i])
            {
                maskBuffer |= (1 << k);
            }
            k--;
            if (k == -1)
            {
                leafMaskBuffer.push_back(maskBuffer);
                k = 31;
                maskBuffer = 0;
            }
        }
        if (k != 31)
        {
            leafMaskBuffer.push_back(maskBuffer);
        }
        k = 31 - ((theNumberOfData % (32 * 32))/32);
        maskBuffer = 0;
        for (int j = 0; j < leafMaskBuffer.size(); j++)
        {
            if (leafMaskBuffer[j] != 0)
            {
                maskBuffer = maskBuffer | (1 << k);
            }
            k--;
            if (k == -1)
            {
                middleMaskBuffer.push_back(maskBuffer);
                k = 31;
                maskBuffer = 0;
            }
        }
        if (k != 31)
        {
            middleMaskBuffer.push_back(maskBuffer);
        }

        k = 31 - ((theNumberOfData % (32 * 32 * 32))/ (32 * 32));
        maskBuffer = 0;
        for (int j = 0; j < middleMaskBuffer.size(); j++)
        {
            if (middleMaskBuffer[j] != 0)
            {
                maskBuffer = maskBuffer | (1 << k);
            }
            k--;
            if (k == -1)
            {
                rootMaskBuffer.push_back(maskBuffer);
                k = 31;
                maskBuffer = 0;
            }
        }
        if (k != 31)
        {
            rootMaskBuffer.push_back(maskBuffer);
        }

        if (theNumberOfData % 32 != 0)
        {
            leafMaskBuffer[0] = BMFLeafNode[i][BMFLeafNode[i].size() - 1] | leafMaskBuffer[0];
            middleMaskBuffer[0] = BMFMiddleNode[i][BMFMiddleNode[i].size() - 1] | middleMaskBuffer[0];
            rootMaskBuffer[0] = BMFRootNode[i][BMFRootNode[i].size() - 1] | rootMaskBuffer[0];
        }
        std::string rootFile = "../file/nodeFile/" + featureSequence[i] + "_rootNode";
        std::string middleFile = "../file/nodeFile/" + featureSequence[i] + "_middleNode";
        std::string leafFile = "../file/nodeFile/" + featureSequence[i] + "_leafNode";

        ofs.open(rootFile, std::ios::in | std::ios::out | std::ios::binary);
        if (ofs.is_open())
        {
            if (theNumberOfData % 32 != 0)
            {
                ofs.seekp(static_cast<std::streamoff>(sizeof(unsigned int)) * (BMFRootNode[i].size() - 1), std::ios::beg);
            }
            else
            {
                ofs.seekp(static_cast<std::streamoff>(sizeof(unsigned int)) * (BMFRootNode[i].size()), std::ios::beg);
            }
            for (size_t k = 0; k < rootMaskBuffer.size(); k++)
            {
                ofs.write(reinterpret_cast<char *>(&rootMaskBuffer[k]), sizeof(unsigned int));
            }
            ofs.close();
        }

        ofs.open(middleFile, std::ios::in | std::ios::out | std::ios::binary);
        if (ofs.is_open())
        {
            if (theNumberOfData % 32 != 0)
            {
                ofs.seekp(static_cast<std::streamoff>(sizeof(unsigned int)) * (BMFMiddleNode[i].size() - 1), std::ios::beg);
            }
            else
            {
                ofs.seekp(static_cast<std::streamoff>(sizeof(unsigned int)) * (BMFMiddleNode[i].size()), std::ios::beg);
            }
            for (size_t k = 0; k < middleMaskBuffer.size(); k++)
            {
                if (k == middleMaskBuffer.size() - 1 || middleMaskBuffer[k] != 0)
                {
                    ofs.write(reinterpret_cast<char *>(&middleMaskBuffer[k]), sizeof(unsigned int));
                }
            }
            ofs.close();
        }

        ofs.open(leafFile, std::ios::in | std::ios::out | std::ios::binary);
        if (ofs.is_open())
        {
            if (theNumberOfData % 32 != 0)
            {
                ofs.seekp(static_cast<std::streamoff>(sizeof(unsigned int)) * (BMFLeafNode[i].size() - 1), std::ios::beg);
            }
            else
            {
                ofs.seekp(static_cast<std::streamoff>(sizeof(unsigned int)) * (BMFLeafNode[i].size()), std::ios::beg);
            }
            for (size_t k = 0; k < leafMaskBuffer.size(); k++)
            {
                if (k == leafMaskBuffer.size() - 1 || leafMaskBuffer[k] != 0)
                {
                    cout << "leaf" << leafMaskBuffer[k] << endl;
                    ofs.write(reinterpret_cast<char *>(&leafMaskBuffer[k]), sizeof(unsigned int));
                }
            }

            ofs.close();
        }
        updataIndex(i, leafMaskBuffer, middleMaskBuffer, allRoot, middleID, leafID);
    }
    theNumberOfData = theNumberOfAllData;
    updataParam();
    saveParam();
    cout << "updataFinished" << endl;
}

void BMFTree::updateDataIntoFile(std::vector<std::vector<std::string>> data)
{
    addAGroupOfData(data, theLengthOfEachData);
}

void BMFTree::parseToken(long long token, unsigned int &featureID, unsigned int &offset_root, unsigned int &offset_middle, unsigned int &offset_leaf, unsigned int &offset_data)
{
    featureID = (token >> 35) & mask28;
    offset_root = (token >> 15) & mask20;
    offset_middle = (token >> 10) & mask5;
    offset_leaf = (token >> 5) & mask5;
    offset_data = token & mask5;
}

void BMFTree::genToken(int featureID, long long &token)
{
    if (theNumberOfData == 0)
        return;
    token = token | (long long)featureID << 35 | (theNumberOfRoot - 1) << 15 | ((theNumberOfData - 1) % (2 << 15)) << 10 | ((theNumberOfData - 1) % (2 << 10)) << 5 | ((theNumberOfData - 1) % (2 << 5));
}

void BMFTree::ArticulatedSearch(std::vector<int> featureList, long long token_i, std::vector<std::string> &resultData, long long &token_j)
{
    std::vector<int> tmp, tmp0, tmp1, tmp2;
    std::vector<int> result;
    std::vector<unsigned int> maskBuffer, middleIndex;
    int indexLeaf;
    unsigned int featureID, offset_root, offset_middle, offset_leaf, offset_data;
    parseToken(token_i, featureID, offset_root, offset_middle, offset_leaf, offset_data);
    long long start = offset_root << 15 | offset_middle << 10 | offset_leaf << 5 | offset_data;

    for (int i = offset_root; i < theNumberOfRoot; i++)
    {
        bool flag = true;
        for (int f = 0; f < featureList.size(); f++)
        {
            if (EBF[f][i] == '0')
            {
                flag = false;
            }
        }
        if (flag)
        {
            tmp.push_back(i);
        }
    }
    for (int i = 0; i < tmp.size(); i++)
    {
        indexLeaf = 0;
        maskBuffer.clear();
        for (int f = 0; f < featureList.size(); f++)
        {
            maskBuffer.push_back(BMFRootNode[featureList[f]][getMaskIndex(tmp[i], -1, -1, featureList[f], -1)]);
        }
        tmp0 = findAllOn(HybridQuery(maskBuffer));

        for (int j = 0; j < tmp0.size(); j++)
        {
            maskBuffer.clear();
            middleIndex.clear();
            for (int f = 0; f < featureList.size(); f++)
            {
                int tmp = getMaskIndex(i, tmp0[j], -1, featureList[f], -1);
                middleIndex.push_back(tmp);
                maskBuffer.push_back(BMFMiddleNode[featureList[f]][tmp]);
            }
            tmp1 = findAllOn(HybridQuery(maskBuffer));
            for (int k = 0; k < tmp1.size(); k++)
            {
                maskBuffer.clear();
                for (int f = 0; f < featureList.size(); f++)
                {
                    maskBuffer.push_back(BMFLeafNode[featureList[f]][getMaskIndex(i, tmp0[j], tmp1[k], featureList[f], middleIndex[f])]);
                }
                tmp2 = findAllOn(HybridQuery(maskBuffer));
                for (int l = 0; l < tmp2.size(); l++)
                {
                    long long tIndex = tmp[i] << 15 | tmp0[j] << 10 | tmp1[k] << 5 | tmp2[l];
                    if (tIndex > start)
                    {
                        result.push_back(tIndex);
                    }
                }
                tmp2.clear();
            }
            indexLeaf += tmp1.size();
            tmp1.clear();
        }
        tmp0.clear();
    }
    readDataForQuery(dataFileName, result, resultData);
    if (featureList.size() > 1)
    {
        token_j |= (long long)1 << 63;
        genToken(0, token_j);
    }
    else
    {
        genToken(featureList[0], token_j);
    }
}
// /***************************************************************************/
void BMFTree::Test_addNewData()
{

    inintParam();
    cout << "initFinished" << endl;
    // showParam();
    updataTree(360);
    // cout<<BMFRootNode[2][1]<<endl;
}

void BMFTree::Test_creatFeature()
{
    static int cnt = 0;
    // cout << "Test_creatFeature_begin" << endl;
    inintParam();
    cout << "init Finied" << endl;
    // std::vector<std::string> keyWords;
    // std::vector<std::string> featureName;
    // std::vector<std::string> keyWordBuffer;
    // std::vector<std::string> featureNameBuffer;
    // std::vector<int> attribute;
    // std::vector<int> attributeBuffer;
    // std::ifstream ifs("keywords.txt", std::ios::in);
    // std::string line;

    // while (std::getline(ifs, line))
    // {
    //     string tmp = line;
    //     keyWords.push_back(line);
    //     std::getline(ifs, line);
    //     attribute.push_back(std::stoi(line));
    //     featureName.push_back(line + "_" + tmp);
    // }
    // ifs.close();
    // creatFeature(featureName, attribute, keyWords);

    std::vector<std::string> featureName;
    std::vector<int> attribute;
    std::vector<std::string> keywords;
    featureName.push_back("fromd88");
    attribute.push_back(0);
    keywords.push_back("0x32be343b94f860124dc4fee278fdcbd38c102d88");
    featureName.push_back("fromc4");
    attribute.push_back(0);
    keywords.push_back("0x387ddb52b1c2dc3821fb79b14518b6ef79e5d3c4");
    featureName.push_back("gasLimit300000");
    attribute.push_back(6);
    keywords.push_back("300000");
    featureName.push_back("gasUse31561");
    attribute.push_back(8);
    keywords.push_back("31561");
    creatFeature(featureName, attribute, keywords);
    saveParam();
    // inintParam();
    // showParam();
}

void BMFTree::Test_Query()
{
    inintParam();
    showParam();
    std::vector<int> featureList;
    std::vector<std::string> resultData;
    featureList.push_back(0);
    // featureList.push_back(3);
    // featureList.push_back(1);
    // cout << "root:" << endl;
    // for (int i = 0; i < BMFRootNode[2].size(); i++)
    // {
    //     cout << BMFRootNode[2][i] << " ";
    // }
    // cout << endl
    //      << "middle:" << endl;
    // for (int i = 0; i < BMFMiddleNode[2].size(); i++)
    // {
    //     cout << BMFMiddleNode[2][i] << " ";
    // }
    // cout << endl
    //      << "leaf:" << endl;
    // for (int i = 0; i < BMFLeafNode[2].size(); i++)
    // {
    //     cout << BMFLeafNode[2][i] << " ";
    // }
    query(featureList, resultData);
    // cout << endl
    //      << "Data:" << endl;
    for (int i = 0; i < resultData.size(); i++)
    {
        cout << resultData[i] << endl;
    }
    // cout<<BMFLeafNode[2][0]<<endl;
}