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

void print(const vector<vector<Entry>> &v) {
  for (const auto &i: v) {
    for (auto j: i) j.print();
    cout << endl;
  }
}

mutex freqDictionaryAccess;


class InvertedIndex {
private:
    vector<string> docs;
    map<string, vector<Entry>> freqDictionary;
public:
    InvertedIndex() = default;
    void UpdateDocumentBase(const vector<string> &inputDocs);
    void processDocument(const string &doc, size_t docID);
    vector<Entry> GetWordCount(const string &word, size_t startingDoc);
    size_t GetTotalWordCount(const string &word);
    friend class SearchServer;
};

void InvertedIndex::processDocument(const string &doc, size_t docID) {
  string word;
  for (size_t j = 0; j < doc.length(); j++) {
    if (isalpha(doc[j])) {
      word += doc[j];
      freqDictionaryAccess.lock();
      if (j + 1 == doc.length() && freqDictionary.find(word) == freqDictionary.end()) {
          freqDictionary.insert(make_pair(word, GetWordCount(word, docID)));
        }
      freqDictionaryAccess.unlock();
    } else {
      freqDictionaryAccess.lock();
      if (freqDictionary.find(word) == freqDictionary.end()) {
        freqDictionary.insert(make_pair(word, GetWordCount(word, docID)));
      }
      freqDictionaryAccess.unlock();
      word = "";
    }
  }
}

void InvertedIndex::UpdateDocumentBase(const vector<string> &inputDocs) {
  docs = inputDocs;
  for (int i = 0; i < docs.size(); i++) {
    string doc = docs[i];
    thread p([this, doc, i] {
        processDocument(doc, i);
    });
    if (p.joinable()) p.join();
  }
}

vector<Entry> InvertedIndex::GetWordCount(const string &word, size_t startingDoc = 0) {
  vector<Entry> entries;
  for (size_t i = startingDoc; i < docs.size(); i++) {
    string nextWord, doc = docs[i];
    if (doc.find(word) == string::npos) continue;
    Entry entry = {i, 0};
    for (size_t j = 0; j < doc.length(); j++) {
      if (isalpha(doc[j])) {
        nextWord += doc[j];
        if (j + 1 == doc.length() && nextWord == word) entry.count++;
      } else {
        if (nextWord == word) entry.count++;
        nextWord = "";
      }
    }
    if (entry.count > 0) entries.push_back(entry);
  }
  return entries;
}

size_t InvertedIndex::GetTotalWordCount(const string &word) {
  auto it = freqDictionary.find(word);
  if (it == freqDictionary.end()) return 0;
  size_t total = 0;
  for (auto e: it->second) total += e.count;
  return total;
}
