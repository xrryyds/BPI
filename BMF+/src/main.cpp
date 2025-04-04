#include <iostream>
#include "../include/Node.h"
#include "../include/tree.h"
#include "../include/tools.h"
#include "../include/IO.h"
#include <bitset>
#include <algorithm>
#include <memory>
#include <sstream>
#include <vector>
#include <fstream>
#include <string>
#include <math.h>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <map>
using namespace std;

int theNumberOfAllData = 7305462;
int theLengthOfEachData = 200;

std::vector<std::string> featureName = {"keyPeron", "bigTran"};
int theNumberOfFeature = 2;

void addFeature(string featureName)
{
    string tmp = featureName;
    std::ofstream ofs;
    ofs.open("featureFileName.txt", std::ios::out);
    if (ofs.is_open())
    {
        ofs << tmp + "_root_fileName" << "\n";
        ofs << tmp + "_MiddleNode_fileName" << "\n";
        ofs << tmp + "__leafNode_fileName" << "\n";
        ofs.close();
    }
    std::cout << "addFeatureFinished" << std::endl;
    ofs.close();
}

void Test_GenerateParam()
{
    std::vector<std::string> featureSequence;
    std::vector<std::string> fileSequence;

    int theNumberOfData = 10000l;

    fileSequence.push_back("event1_root_fileName");
    fileSequence.push_back("event1_MiddleNode_fileName");
    fileSequence.push_back("event1_leafNode_fileName");
    fileSequence.push_back("event2_root_fileName");
    fileSequence.push_back("event2_MiddleNode_fileName");
    fileSequence.push_back("event2_leafNode_fileName");

    featureSequence.push_back("keyPerson");
    featureSequence.push_back("bigValue");
    std::cout << "paramSetSuccess" << std::endl;
    std::ofstream ofs;
    ofs.open("param.txt", std::ios::out);
    if (ofs.is_open())
    {
        ofs << theNumberOfData << "\n";
        ofs << theNumberOfFeature << "\n";
        ofs << theNumberOfFeature << "\n";
        ofs << 200 << "\n";
        ofs << "data.txt" << "\n";
        ofs.close();
    }
    ofs.open("featureName.txt", std::ios::out);
    if (ofs.is_open())
    {
        for (int i = 0; i < theNumberOfFeature; i++)
        {
            ofs << featureSequence[i] << "\n";
        }
        ofs.close();
    }
    ofs.open("nodeNameFile.txt", std::ios::out);
    if (ofs.is_open())
    {
        for (int i = 0; i < theNumberOfFeature * 3; i++)
        {
            ofs << fileSequence[i] << "\n";
        }
        ofs.close();
    }
    std::cout << "inintParamFinished" << std::endl;
}

void Test_ReadParam()
{
    std::vector<std::string> featureSequence;
    std::vector<std::string> fileSequence;
    int theNumberOfData;
    int theNumberOfRoot;
    int theNumberOfLeaf;
    int theNumberOfMiddleNode;
    int theNumberOfNode;
    int theNumberOfFeature;
    std::ifstream ifs;
    ifs.open("param.txt", std::ios::in);
    if (ifs.is_open())
    {
        ifs >> theNumberOfData;
        ifs >> theNumberOfFeature;
        theNumberOfLeaf = std::ceil(static_cast<float>(theNumberOfData) / static_cast<float>(32));
        theNumberOfMiddleNode = std::ceil(static_cast<float>(theNumberOfLeaf) / static_cast<float>(32));
        theNumberOfRoot = std::ceil(static_cast<float>(theNumberOfMiddleNode) / static_cast<float>(32));
        theNumberOfNode = theNumberOfLeaf + theNumberOfMiddleNode + theNumberOfRoot;
        ifs.close();
    }

    ifs.open("featureName.txt", std::ios::in);
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

    ifs.open("nodeNameFile.txt", std::ios::in);
    if (ifs.is_open())
    {
        std::string str;
        for (int i = 0; i < theNumberOfFeature * 3; i++)
        {
            ifs >> str;
            cout << str << endl;
            fileSequence.push_back(str);
        }
        ifs.close();
    }

    std::cout << "theNumberOfData:" << theNumberOfData << std::endl;
    std::cout << "theNumberOfRoot:" << theNumberOfRoot << std::endl;
    std::cout << "theNumberOfMiddleNode:" << theNumberOfMiddleNode << std::endl;
    std::cout << "theNumberOfLeaf:" << theNumberOfLeaf << std::endl;
    std::cout << "theNumberOfNode:" << theNumberOfNode << std::endl;
    std::cout << "theNumberOfFeature:" << theNumberOfFeature << std::endl;

    std::cout << "eventList:" << std::endl;
    for (int i = 0; i < theNumberOfFeature; i++)
    {
        std::cout << featureSequence[i] << std::endl;
    }

    std::cout << "fileList:" << std::endl;
    for (int i = 0; i < theNumberOfFeature; i++)
    {
        std::cout << fileSequence[i * 3] << std::endl;
        std::cout << fileSequence[i * 3 + 1] << std::endl;
        std::cout << fileSequence[i * 3 + 2] << std::endl;
    }
}

void Test_SingleQuery(int queryID, int theNumberOfAllData)
{
    BMFTree tree;
    tree.init(theNumberOfAllData);
    // tree.showParam();
    tree.showTree();
    std::vector<std::string> result;

    auto beforeTime = std::chrono::steady_clock::now();

    std::vector<int> feature;

    feature.push_back(queryID);
    tree.query(feature, result);

    int cnt = 0;
    for (int i = 0; i < result.size(); i++)
        cnt++;
    auto afterTime = std::chrono::steady_clock::now();

    // 毫秒级
    double duration_millsecond = std::chrono::duration<double, std::milli>(afterTime - beforeTime).count();
    std::cout << duration_millsecond << "ms" << std::endl;

    // std::cout << result.size() << "\n";
    // for (int i = result.size() - 5; i < result.size(); i++)
    // {
    //     std::cout << result[i] << "\n";
    // }
    std::cout << result.size() << std::endl;
}

void readCSV()
{
    ifstream file("7.csv");
    vector<vector<string>> data;

    string line;
    int num = 0;
    while (getline(file, line))
    {
        cout << line << endl;
        break;
        if (num == 1010000 || num == 2010000 || num == 3010000 || num == 4010000 || num == 5010000 || num == 6010000 || num == 7010000)
        {
            cout << line << endl;
        }
        vector<string> row;
        size_t pos = 0;
        string token;
        while ((pos = line.find(",")) != string::npos)
        {
            token = line.substr(0, pos);
            row.push_back(token);
            line.erase(0, pos + 1);
        }
        row.push_back(line);
        data.push_back(row);
    }

    cout << data.size();
}

// Initialization data
void preData(int length)
{
    std::vector<std::string> keyWords;
    std::vector<int> attribute;
    // data file address
    ifstream file("E:\\work\\data\\BlockTransaction.csv");
    std::vector<std::map<std::string, int>> mapList;
    vector<vector<string>> data;
    size_t max = 0;
    string line;
    getline(file, line);
    max = std::max(max, line.length());
    while (getline(file, line))
    {
        vector<string> row;
        vector<int> tmp;
        size_t pos = 0;
        string token;
        int cnt = 0;
        while ((pos = line.find(",")) != string::npos)
        {
            token = line.substr(0, pos);
            if (cnt != 0 && cnt != 1 && cnt != 2)
            {
                row.push_back(token);
            }
            line.erase(0, pos + 1);
            cnt++;
        }
        data.push_back(row);
    }
    file.close();
    cout << data.size() << endl;
    vector<int> tmp(data[0].size(), 0);
    for (int k = 0; k < data[0].size(); k++)
    {
        mapList.push_back(std::map<std::string, int>());
    }
    std::ofstream ofs;
    ofs.open("data.txt", std::ios::out);
    if (ofs.is_open())
    {
        for (int i = 0; i < data.size(); i++)
        {
            std::string buffer;
            for (int j = 0; j < data[i].size(); j++)
            {
                buffer.append(data[i][j]);
                buffer.append(",");
            }
            int cnt = length - buffer.length();
            while (cnt--)
            {
                buffer.append("*");
            }
            ofs << buffer;
        }
        ofs.close();
    }
    for (int i = 0; i < data.size(); i++)
    {
        for (int j = 0; j < data[i].size(); j++)
        {
            if(j == 5){
                continue;
            }
            auto it = mapList[j].find(data[i][j]);

            if (it == mapList[j].end())
            {
                mapList[j].insert({data[i][j], 0});
                keyWords.push_back(data[i][j]);
                attribute.push_back(j);
                tmp[j]++;
            }
        }
    }
    ofs.open("keywords.txt", std::ios::out);
    if (ofs.is_open())
    {
        for (int i = 0; i < keyWords.size(); i++)
        {
            ofs << keyWords[i] << "\n";
            ofs << attribute[i] << "\n";
        }
        ofs.close();
    }

    for (int i = 0; i < mapList.size(); i++)
    {
        cout << tmp[i] << endl;
    }
    cout << keyWords.size() << endl;
}

void readKeywords(std::vector<std::string> &keyWords, std::vector<std::string> &attributes)
{
    std::ifstream ifs("keywords.txt", std::ios::in); // 打开文件用于读取
    if (ifs.is_open())
    {
        std::string line;
        while (std::getline(ifs, line))
        {                             // 逐行读取文件
            keyWords.push_back(line); // 存储关键词
            std::getline(ifs, line);
            attributes.push_back(line); // 存储属性
        }

        ifs.close(); // 关闭文件
    }
    else
    {
        std::cerr << "Failed to open file for reading!" << std::endl;
    }
}

// Test the predata for success
void testReadData(int k)
{
    std::ifstream file("data.txt", std::ios::binary);
    if (file)
    {
        std::vector<char> buffer(k);
        file.read(buffer.data(), k);
        std::string data(buffer.begin(), buffer.end());
        std::cout << data << std::endl;

        std::vector<char> buffer2(k);
        file.read(buffer2.data(), k);
        std::string data2(buffer2.begin(), buffer2.end());
        std::cout << data2 << std::endl;

        std::vector<char> buffer3(k);
        file.read(buffer3.data(), k);
        std::string data3(buffer3.begin(), buffer3.end());
        std::cout << data3 << std::endl;
    }
}

void init()
{
    std::ifstream ifs;
    ifs.open("globalParameter.txt", std::ios::in);
    if (ifs.is_open())
    {
        ifs >> theNumberOfAllData;
        ifs >> theLengthOfEachData;
        ifs.close();
    }
}

void save()
{
    std::ofstream ofs;
    ofs.open("globalParameter.txt", std::ios::out);
    if (ofs.is_open())
    {
        ofs << theNumberOfAllData << "\n";
        ofs << theLengthOfEachData << "\n";
        ofs.close();
    }
}

int main()
{
    // Test_GenerateParam();
    // Test_ReadParam();
    // preData(200);
    // cout << "all"<<keyWords.size() << endl;
    BMFTree tree;
    tree.Test_creatFeature();
    // tree.Test_addNewData();
    // tree.Test_Query();
}
