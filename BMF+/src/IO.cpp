#include "IO.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include "../include/Node.h"
#include "../include/tree.h"
#include <fstream>
#include <string>
#include <cmath>

void initMask(std::string fileName, std::vector<Node> BMFNode, int featureID)
{
        int buffer;
        std::ofstream file(fileName, std::ios::binary);
        if (!file.is_open())
        {
            std::cerr << "openFileErro:" << fileName << std::endl;
            return;
        }

        for (int i = 0; i < BMFNode.size(); i++)
        {
            buffer = BMFNode[i].getQueryMask(featureID);
            file.write(reinterpret_cast<char *>(&buffer), sizeof(int));
        }

        if (!file.good())
        {
            std::cerr << "writeErro" << std::endl;
        }

        file.close();
}



void readMask(std::string fileName, std::vector<unsigned int> &result)
{
    unsigned int buffer;
    std::ifstream file(fileName, std::ios::in | std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "openFileErro:" << fileName << std::endl;
        return;
    }
    while (file.read(reinterpret_cast<char *>(&buffer), sizeof(unsigned int)))
    {
        // std::cout<<buffer<<std::endl;
        result.push_back(buffer);
    }

    file.close();
}

void addAGroupOfData(std::vector<std::vector<std::string>> data, int length)
{
    std::ofstream ofs;
    ofs.open("data.txt", std::ios::out | std::ios::app);
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

void creatFeatureFlush(std::string fileName, std::vector<unsigned int> maskBuffer, int root)
{
    std::ofstream ofs;
    ofs.open(fileName, std::ios::out | std::ios::binary);
    if (ofs.is_open())
    {
        for (size_t k = 0; k < maskBuffer.size(); k++)
        {
            if(!root && maskBuffer[k]!=0){
                ofs.write(reinterpret_cast<char *>(&maskBuffer[k]), sizeof(unsigned int));
            }
        }
        ofs.close();
    }
}