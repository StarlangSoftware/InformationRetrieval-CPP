//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#ifndef INFORMATIONRETRIEVAL_TERM_H
#define INFORMATIONRETRIEVAL_TERM_H


#include "Word.h"

class Term : Word{
private:
    int termId;
public:
    Term(string name, int termId);
    int getTermId();
};


#endif //INFORMATIONRETRIEVAL_TERM_H
