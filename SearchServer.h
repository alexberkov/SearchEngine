#pragma once
#include <map>
#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include "InvertedIndex.h"
#include "ConverterJson.h"

using namespace std;

float round(float value, unsigned char precision) {
  float pow10 = pow(10.0f, (float) precision);
  return round(value * pow10) / pow10;
}

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

void print(const vector<vector<RelativeIndex>>& v) {
  for (const auto& i: v) {
    for (auto j: i) j.print();
    cout << endl;
  }
}

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

vector<vector<string>> SearchServer::processQueries(const vector<string> &inputQueries) {
  vector<vector<string>> queries;
  for (const string &query: inputQueries) {
    vector<string> words;
    string nextWord;
    for (size_t j = 0; j < query.length(); j++) {
      if (isalpha(query[j])) {
        nextWord += query[j];
        if (j + 1 == query.length()) words.push_back(nextWord);
      } else {
        words.push_back(nextWord);
        nextWord = "";
      }
    }
    queries.push_back(words);
  }
  return queries;
}

vector<vector<RelativeIndex>> SearchServer::search(const vector<string> &inputQueries) {
  vector<vector<RelativeIndex>> results;
  vector<vector<string>> queries = processQueries(inputQueries);
  for (const auto &query: queries) {
    auto words = processWords(query);
    vector<AbsoluteIndex> result;
    for (size_t i = 0; i < index.docs.size(); i++) result.push_back(AbsoluteIndex{i, 0});
    for (const auto &word: words) {
      if (word.second == 0) {
        result = {};
        break;
      }
      auto p = index.freqDictionary.find(word.first);
      vector<AbsoluteIndex> tempResult;
      for (size_t i = 0; i < index.docs.size(); i++) tempResult.push_back(AbsoluteIndex{i, 0});
      for (auto e: p->second) tempResult[e.docID].rank += e.count;
      for (size_t i = 0; i < index.docs.size(); i++) result[i].rank += tempResult[i].rank;
    }
    vector<RelativeIndex> finalResult = processResults(result);
    results.push_back(finalResult);
  }
  return results;
}

vector<pair<string, size_t>> SearchServer::processWords(const vector<string> &words) {
  vector<pair<string, size_t>> wordFreq;
  for (const string& word: words) wordFreq.emplace_back(word, index.GetTotalWordCount(word));
  sort(wordFreq.begin(), wordFreq.end(), [](auto &l, auto &r) {
      return l.second < r.second;
  });
  return wordFreq;
}

vector<RelativeIndex> SearchServer::processResults(const vector<AbsoluteIndex>& results) {
  vector<RelativeIndex> tempRes;
  size_t max = 0;
  for (auto r: results) {
    if (r.rank != 0) tempRes.push_back(RelativeIndex{r.docID, (float) r.rank});
    if (r.rank > max) max = r.rank;
  }
  sort(tempRes.begin(), tempRes.end(), [](auto &left, auto &right) {
      if (left.rank == right.rank) return left.docID < right.docID;
      else return left.rank > right.rank;
  });
  vector<RelativeIndex> res;
  int k = ConverterJSON::getResponseLimit(), i = 0;
  for (auto r: tempRes) {
    float newRank = round(r.rank / ((float) max), 2);
    if (i < k) res.push_back(RelativeIndex{r.docID, newRank});
    i++;
  }
  return res;
}

vector<vector<pair<int, float>>> SearchServer::formAnswers(const vector<vector<RelativeIndex>>& results) {
  vector<vector<pair<int, float>>> res;
  int maxResponses = ConverterJSON::getResponseLimit(), k = 0;
  for (const auto& i: results) {
    vector<pair<int, float>> r;
    for (auto j: i) {
      r.emplace_back(j.docID, j.rank);
      k++;
      if (k == maxResponses) break;
    }
    res.push_back(r);
  }
  return res;
}
