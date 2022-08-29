#include "ConverterJson.h"

vector<string> ConverterJSON::getTextDocuments(const string& extra) {
    ifstream configFile(extra + "../JSON/config.json");
    if (configFile.is_open()) {
        json config;
        configFile >> config;
        vector<string> docs, filePaths = config["files"];
        if (!extra.empty()) {
          for (auto & filePath : filePaths) filePath = extra + filePath;
        }
        for (const string& path: filePaths) {
            ifstream file(path);
            if (file.is_open()) {
                string fileContent, word;
                file >> word;
                fileContent += word;
                while(file >> word) {
                    fileContent += (" " + word);
                }
                docs.push_back(fileContent);
                file.close();
            }
        }
        return docs;
    } else return {};
}

vector<string> ConverterJSON::getFileNames(const string& extra) {
  ifstream configFile(extra + "../JSON/config.json");
  if (configFile.is_open()) {
    json config;
    configFile >> config;
    vector<string> fileNames = config["files"];
    configFile.close();
    return fileNames;
  } else return {};
}

int ConverterJSON::getResponseLimit(const string& extra) {
    ifstream configFile(extra + "../JSON/config.json");
    int maxResponses = 5;
    if (configFile.is_open()) {
        json config;
        configFile >> config;
        maxResponses = config["config"]["max_responses"];
        configFile.close();
    }
    return maxResponses;
}

vector<string> ConverterJSON::getRequests() {
    ifstream requestsFile("../JSON/requests.json");
    if (requestsFile.is_open()) {
        json requestsContent;
        requestsFile >> requestsContent;
        vector<string> requests = requestsContent["requests"];
        return requests;
    } else return {};
}

void ConverterJSON::putAnswers(vector<vector<pair<int, float>>> answers) {
    json results;
    results["answers"] = {};
    for (int i = 0; i < answers.size(); i++) {
        string requestName = "request" + to_string((i + 1));
        results["answers"][requestName] = {};
        if (!answers[i].empty()) {
            results["answers"][requestName]["result"] = "true";
            if (answers[i].size() > 1) {
                results["answers"][requestName]["relevance"] = {};
                for (pair<int, float> result: answers[i]) {
                    json resultLine;
                    resultLine["docid"] = result.first;
                    resultLine["rank"] = result.second;
                    results["answers"][requestName]["relevance"].push_back(resultLine);
                }
            } else {
                pair<int, float> result = answers[i][0];
                results["answers"][requestName]["docid"] = result.first;
                results["answers"][requestName]["rank"] = result.second;
            }
        } else results["answers"][requestName]["result"] = "false";
    }
    ofstream file("../JSON/answers.json");
    file << results.dump(2, ' ');
    file.close();
}