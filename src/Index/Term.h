//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#ifndef INFORMATIONRETRIEVAL_TERM_H
#define INFORMATIONRETRIEVAL_TERM_H


#include "Dictionary/Word.h"

class Term : public Word{
private:
    int termId;
public:
    Term(const string& name, int termId);
    int getTermId() const;
};


#endif //INFORMATIONRETRIEVAL_TERM_H
