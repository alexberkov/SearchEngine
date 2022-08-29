#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

class ConverterJSON {
public:
    ConverterJSON() = default;
    static vector<string> getTextDocuments(const string& extra = "");
    static vector<string> getFileNames(const string& extra = "");
    static int getResponseLimit(const string& extra = "");
    static vector<string> getRequests();
    static void putAnswers(vector<vector<pair<int, float>>> answers);
};
