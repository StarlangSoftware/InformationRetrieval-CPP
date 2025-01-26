//
// Created by Olcay Taner YILDIZ on 3.12.2022.
//

#ifndef INFORMATIONRETRIEVAL_LARGECOLLECTION_H
#define INFORMATIONRETRIEVAL_LARGECOLLECTION_H


#include "DiskCollection.h"

class LargeCollection : public DiskCollection{
private:
    void constructDictionaryAndIndexesInDisk();
    bool notCombinedAllDictionaries(const string* currentWords, int size) const;
    vector<int> selectDictionariesWithMinimumWords(const string* currentWords, int size) const;
    void combineMultipleDictionariesInDisk(const string& _name, const string& tmpName, int blockCount) const;
    void constructDictionaryAndInvertedIndexInDisk(TermType termType) const;
    void constructDictionaryAndPositionalIndexInDisk(TermType termType);
    void addNGramsToDictionaryAndIndex(const string& line, int k, TermDictionary& nGramDictionary, NGramIndex& nGramIndex);
    void constructNGramDictionaryAndIndexInDisk();
public:
    LargeCollection(const string& directory, const Parameter& parameter);
};


#endif //INFORMATIONRETRIEVAL_LARGECOLLECTION_H
