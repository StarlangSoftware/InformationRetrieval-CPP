//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#include "TermOccurrence.h"

TermOccurrence::TermOccurrence(const Word& term, int docId, int position) {
    this->term = term;
    this->docId = docId;
    this->position = position;
}

Word TermOccurrence::getTerm() const{
    return term;
}

int TermOccurrence::getDocId() const{
    return docId;
}

int TermOccurrence::getPosition() const{
    return position;
}

bool TermOccurrence::isDifferent(const TermOccurrence& currentTerm, Comparator comparator) {
    return !(currentTerm.getTerm() == term);
}
