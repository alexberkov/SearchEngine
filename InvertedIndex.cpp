#include "InvertedIndex.h"

mutex freqDictionaryAccess;

void print(const vector<vector<Entry>> &v) {
    for (const auto &i: v) {
        for (auto j: i) j.print();
        cout << endl;
    }
}

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
    vector<thread> t;
    for (int i = 0; i < docs.size(); i++) {
        string doc = docs[i];
        t.emplace_back(([this, doc, i] {
            processDocument(doc, i);
        }));
    }
    for (auto & i : t) if (i.joinable()) i.join();
}

vector<Entry> InvertedIndex::GetWordCount(const string &word, size_t startingDoc) {
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
