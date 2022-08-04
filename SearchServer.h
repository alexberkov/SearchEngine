#pragma once
#include <map>
#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include "InvertedIndex.h"
#include "ConverterJson.h"

using namespace std;

float round(float value, unsigned char precision);

struct RelativeIndex {
    size_t docID;
    float rank;
    bool operator ==(const RelativeIndex& other) const {
      return (docID == other.docID && rank == other.rank);
    }
    void print() const {
      cout << "{" << docID << ", " << rank << "} ";
    }
};

void print(const vector<vector<RelativeIndex>>& v);

struct AbsoluteIndex {
    size_t docID;
    size_t rank;
    bool operator ==(const AbsoluteIndex& other) const {
      return (docID == other.docID && rank == other.rank);
    }
};

class SearchServer {
private:
    InvertedIndex index;
public:
    explicit SearchServer (InvertedIndex &i) : index (i) { };
    static vector<vector<string>> processQueries (const vector<string> &inputQueries);
    vector<pair<string, size_t>> processWords (const vector<string> &words);
    vector<vector<RelativeIndex>> search (const vector<string> &inputQueries);
    static vector<vector<pair<int, float>>> formAnswers(const vector<vector<RelativeIndex>>& results);
    static vector<RelativeIndex> processResults (const vector<AbsoluteIndex>& results);
};
