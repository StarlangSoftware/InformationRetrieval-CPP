//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#include "Term.h"

#include <utility>

Term::Term(string name, int termId) : Word(std::move(name)){
    this->termId = termId;
}

int Term::getTermId() {
    return termId;
}
