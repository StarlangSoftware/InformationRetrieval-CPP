//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#include "Term.h"

#include <utility>

Term::Term(const string& name, int termId) : Word(name){
    this->termId = termId;
}

int Term::getTermId() const{
    return termId;
}
