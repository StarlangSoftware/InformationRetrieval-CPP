//
// Created by Olcay Taner YILDIZ on 25.07.2022.
//

#ifndef INFORMATIONRETRIEVAL_POSITIONALINDEX_H
#define INFORMATIONRETRIEVAL_POSITIONALINDEX_H

#include <map>
#include "PositionalPostingList.h"
#include "TermDictionary.h"
#include "../Query/Query.h"
#include "../Document/Document.h"
#include "TermWeighting.h"
#include "../Document/DocumentWeighting.h"

class PositionalIndex {
private:
    map<int, PositionalPostingList> positionalIndex;
    void readPositionalPostingList(const string& fileName);
public:
    PositionalIndex();
    explicit PositionalIndex(const string& fileName);
    PositionalIndex(TermDictionary& dictionary, const vector<TermOccurrence>& terms, Comparator comparator);
    void addPosition(int termId, int docId, int position);
    void save(const string& fileName);
    int* getDocumentSizes(int documentSize) const;
    void setCategoryCounts(vector<Document>& documents);
    QueryResult positionalSearch(const Query& query, TermDictionary& dictionary);
    int* getTermFrequencies(int docId) const;
    int* getDocumentFrequencies() const;
    QueryResult rankedSearch(const Query& query, TermDictionary& dictionary, const vector<Document>& documents, TermWeighting termWeighting, DocumentWeighting documentWeighting, int documentsReturned);
};


#endif //INFORMATIONRETRIEVAL_POSITIONALINDEX_H
