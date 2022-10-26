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
#include "../Query/VectorSpaceModel.h"
#include "Matrix.h"
#include "../Query/RetrievalType.h"
#include "../Query/SearchParameter.h"

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
    vector<TermOccurrence> constructTerms(TermType termType) const;
    set<string> constructDistinctWordList(TermType termType) const;
    bool notCombinedAllIndexes(const int* currentIdList, int size) const;
    bool notCombinedAllDictionaries(string* currentWords, int size) const;
    vector<int> selectIndexesWithMinimumTermIds(const int* currentIdList, int size) const;
    vector<int> selectDictionariesWithMinimumWords(string* currentWords, int size) const;
    void combineMultipleDictionariesInDisk(const string& _name, const string& tmpName, int blockCount);
    void addNGramsToDictionaryAndIndex(const string& line, int k, TermDictionary& nGramDictionary, NGramIndex& nGramIndex);
    void constructNGramDictionaryAndIndexInDisk();
    void combineMultipleInvertedIndexesInDisk(const string& _name, const string& tmpName, int blockCount);
    void constructInvertedIndexInDisk(TermDictionary _dictionary, TermType termType);
    void constructDictionaryAndInvertedIndexInDisk(TermType termType);
    void combineMultiplePositionalIndexesInDisk(const string& _name, int blockCount);
    void constructDictionaryAndPositionalIndexInDisk(TermType termType);
    void constructPositionalIndexInDisk(TermDictionary _dictionary, TermType termType);
    void constructNGramIndex();
    void saveCategories();
    void loadCategories();
public:
    Collection(const string& directory, const Parameter& parameter);
    int size() const;
    int vocabularySize() const;
    void save();
    QueryResult searchCollection(const Query& query, const SearchParameter& searchParameter);
};


#endif //INFORMATIONRETRIEVAL_COLLECTION_H
