//
// Created by Olcay Taner YILDIZ on 25.07.2022.
//

#ifndef INFORMATIONRETRIEVAL_NGRAMINDEX_H
#define INFORMATIONRETRIEVAL_NGRAMINDEX_H


#include "InvertedIndex.h"

class NGramIndex : public InvertedIndex{
public:
    NGramIndex();
    NGramIndex(TermDictionary& dictionary, const vector<TermOccurrence>& terms);
    explicit NGramIndex(const string& fileName);
};


#endif //INFORMATIONRETRIEVAL_NGRAMINDEX_H
