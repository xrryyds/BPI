# BPI

#### Instructions

This version is for SIGMOD 2026 review only.

Get the dataset：Execute BlockTransaction.py

#### Instructions for use
C++ 11 and Above

1. In main.cpp, replace ifstream file("E:\\work\\data\\BlockTransaction.csv"); in the preData function with the path to your own dataset.
2. In param.txt, set the first line to your desired data scale, and ensure the second line is 0.
3. Run preData(200); to preprocess the data.
4. Execute tree.Test_creatFeature(); to create four features. You can also modify the Test_creatFeature() function to customize which features are generated.
5. Run tree.Test_Query(); to perform a query.
6. To use Articulated Search instead, replace the query in tree.Test_Query() with a call to ArticulatedSearch.
7. Run tree.Test_addNewData(); to add new data. You can specify the number of new entries to add by modifying the parameter in updataTree(360);.
