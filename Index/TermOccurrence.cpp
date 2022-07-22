//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#include "TermOccurrence.h"

TermOccurrence::TermOccurrence(Word term, int docId, int position) {
    this->term = term;
    this->docId = docId;
    this->position = position;
}

Word TermOccurrence::getTerm() {
    return term;
}

int TermOccurrence::getDocId() {
    return docId;
}

int TermOccurrence::getPosition() {
    return position;
}

bool TermOccurrence::isDifferent(TermOccurrence currentTerm, Comparator comparator) {
    return !(currentTerm.getTerm() == term);
}
