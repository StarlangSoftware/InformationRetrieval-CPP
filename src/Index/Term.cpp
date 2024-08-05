//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#include "Term.h"

/**
 * Constructor for the Term class. Sets the fields.
 * @param name Text of the term
 * @param termId Id of the term
 */
Term::Term(const string& name, int termId) : Word(name){
    this->termId = termId;
}

/**
 * Accessor for the term id attribute.
 * @return Term id attribute
 */
int Term::getTermId() const{
    return termId;
}
