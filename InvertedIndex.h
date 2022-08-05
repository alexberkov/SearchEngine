#pragma once
#include <map>
#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

struct Entry {
    size_t docID, count;
    bool operator ==(const Entry &other) const {
      return (docID == other.docID && count == other.count);
    }
    void print() const {
      cout << "{" << docID << ", " << count << "} ";
    }
};

void print(const vector<vector<Entry>> &v);

class InvertedIndex {
private:
    vector<string> docs;
    map<string, vector<Entry>> freqDictionary;
public:
    InvertedIndex() = default;
    void UpdateDocumentBase(const vector<string> &inputDocs);
    void processDocument(const string &doc);
    vector<Entry> GetWordCount(const string &word);
    size_t GetTotalWordCount(const string &word);
    friend class SearchServer;
};
