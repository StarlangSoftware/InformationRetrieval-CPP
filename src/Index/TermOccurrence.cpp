//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#include "TermOccurrence.h"

/**
 * Constructor for the TermOccurrence class. Sets the attributes.
 * @param term Term for this occurrence.
 * @param docId Document id of the term occurrence.
 * @param position Position of the term in the document for this occurrence.
 */
TermOccurrence::TermOccurrence(const Word& term, int docId, int position) {
    this->term = term;
    this->docId = docId;
    this->position = position;
}

/**
 * Accessor for the term.
 * @return Term
 */
Word TermOccurrence::getTerm() const{
    return term;
}

/**
 * Accessor for the document id.
 * @return Document id.
 */
int TermOccurrence::getDocId() const{
    return docId;
}

/**
 * Accessor for the position of the term.
 * @return Position of the term.
 */
int TermOccurrence::getPosition() const{
    return position;
}

/**
 * Checks if the current occurrence is different from the other occurrence.
 * @param currentTerm Term occurrence to be compared.
 * @return True, if two terms are different; false if they are the same.
 */
bool TermOccurrence::isDifferent(const TermOccurrence& currentTerm) const {
    return !(currentTerm.getTerm() == term);
}
