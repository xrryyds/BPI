#include "IO.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "../include/Node.h"
#include "../include/tree.h"
#include <fstream>
#include <string>
#include <cmath>

#include <cstdint>


void transformStrToData(std::vector<std::string> str, std::vector<std::vector<std::string>> &data)
{
    for (int i = 0; i < str.size(); i++)
    {
        std::string line = str[i];
        std::vector<std::string> row;
        size_t pos = 0;
        std::string token;
        while ((pos = line.find(",")) != std::string::npos)
        {
            token = line.substr(0, pos);
            row.push_back(token);
            line.erase(0, pos + 1);
        }
        data.push_back(row);
    }
}

const int deBruijn32 = 0x077CB531;
const int deBruijnTable[32] = {
    0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
    31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
};


std::vector<int> findAllOn(uint32_t number) {
    std::vector<int> positions;
    while (number) {
        uint32_t rightmostOne = number & -number; 
        int pos = deBruijnTable[((rightmostOne * deBruijn32) >> 27) & 0x1F];
        positions.push_back(31 - pos); 
        number ^= rightmostOne;
    }
    std::reverse(positions.begin(), positions.end());
    return positions;
}
