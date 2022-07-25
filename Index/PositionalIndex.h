//
// Created by Olcay Taner YILDIZ on 25.07.2022.
//

#ifndef INFORMATIONRETRIEVAL_POSITIONALINDEX_H
#define INFORMATIONRETRIEVAL_POSITIONALINDEX_H

#include <map>
#include "PositionalPostingList.h"
#include "TermDictionary.h"

class PositionalIndex {
private:
    map<int, PositionalPostingList> positionalIndex;
    void readPositionalPostingList(string fileName);
public:
    PositionalIndex();
    explicit PositionalIndex(string fileName);
    PositionalIndex(TermDictionary dictionary, vector<TermOccurrence> terms, Comparator comparator);
    void addPosition(int termId, int docId, int position);
};


#endif //INFORMATIONRETRIEVAL_POSITIONALINDEX_H
