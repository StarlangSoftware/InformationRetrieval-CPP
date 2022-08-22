//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#ifndef INFORMATIONRETRIEVAL_TERMOCCURRENCE_H
#define INFORMATIONRETRIEVAL_TERMOCCURRENCE_H


#include "Dictionary/Word.h"
#include "Dictionary/Dictionary.h"

class TermOccurrence {
private:
    Word term;
    int docId;
    int position;
public:
    TermOccurrence(Word term, int docId, int position);
    Word getTerm();
    int getDocId();
    int getPosition();
    bool isDifferent(TermOccurrence currentTerm, Comparator comparator);
};


#endif //INFORMATIONRETRIEVAL_TERMOCCURRENCE_H
