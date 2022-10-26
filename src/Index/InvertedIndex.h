//
// Created by Olcay Taner YILDIZ on 25.07.2022.
//

#ifndef INFORMATIONRETRIEVAL_INVERTEDINDEX_H
#define INFORMATIONRETRIEVAL_INVERTEDINDEX_H

#include <map>
#include "PostingList.h"
#include "TermDictionary.h"
#include "../Query/Query.h"

using namespace std;

class InvertedIndex {
private:
    map<int, PostingList> index;
    void readPostingList(const string& fileName);
public:
    InvertedIndex();
    explicit InvertedIndex(const string& fileName);
    InvertedIndex(TermDictionary dictionary, const vector<TermOccurrence>& terms, Comparator comparator);
    void save(const string& fileName);
    void add(int termId, int docId);
    QueryResult search(const Query& query, TermDictionary dictionary);
};


#endif //INFORMATIONRETRIEVAL_INVERTEDINDEX_H
