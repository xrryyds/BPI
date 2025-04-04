#ifndef IO_H
#define IO_H

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

void initMask(std::string fileName, std::vector<Node> BMFNode, int featureID);

void readMask(std::string fileName, std::vector<unsigned int> &buffer);

void addAGroupOfData(std::vector<std::vector<std::string>> newData, int length);

void creatFeatureFlush(std::string fileName,std::vector<unsigned int> maskBuffer, int root);

#endif // FILE_H