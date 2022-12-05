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
    TermOccurrence(const Word& term, int docId, int position);
    Word getTerm() const;
    int getDocId() const;
    int getPosition() const;
    bool isDifferent(const TermOccurrence& currentTerm);
};


#endif //INFORMATIONRETRIEVAL_TERMOCCURRENCE_H
