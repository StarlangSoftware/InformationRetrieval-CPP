//
// Created by Olcay Taner YILDIZ on 26.07.2022.
//

#ifndef INFORMATIONRETRIEVAL_COLLECTION_H
#define INFORMATIONRETRIEVAL_COLLECTION_H


#include "IndexType.h"
#include "../Index/TermDictionary.h"
#include "Document.h"
#include "../Index/IncidenceMatrix.h"
#include "../Index/InvertedIndex.h"
#include "../Index/NGramIndex.h"
#include "../Index/PositionalIndex.h"
#include "Parameter.h"

class Collection {
private:
    IndexType indexType;
    TermDictionary dictionary;
    TermDictionary phraseDictionary;
    TermDictionary biGramDictionary;
    TermDictionary triGramDictionary;
    vector<Document> documents;
    IncidenceMatrix incidenceMatrix;
    InvertedIndex invertedIndex;
    NGramIndex biGramIndex;
    NGramIndex triGramIndex;
    PositionalIndex positionalIndex;
    InvertedIndex phraseIndex;
    PositionalIndex phrasePositionalIndex;
    Comparator comparator;
    string name;
    Parameter parameter;
    void constructDictionaryInDisk();
    void constructIndexesInDisk();
    void constructIndexesInMemory();
    vector<TermOccurrence> constructTerms(TermType termType);
    set<string> constructDistinctWordList(TermType termType);
    bool notCombinedAllIndexes(int* currentIdList, int size);
    bool notCombinedAllDictionaries(string* currentWords, int size);
    vector<int> selectIndexesWithMinimumTermIds(int* currentIdList, int size);
public:
    Collection(string directory, Parameter parameter);
    int size();
    int vocabularySize();
    void save();
};


#endif //INFORMATIONRETRIEVAL_COLLECTION_H
