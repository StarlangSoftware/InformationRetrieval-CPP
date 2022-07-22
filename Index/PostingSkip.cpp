//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#include "PostingSkip.h"

PostingSkip::PostingSkip(int id) : Posting(id) {
}

bool PostingSkip::hasSkip() {
    return skipAvailable;
}

void PostingSkip::addSkip(PostingSkip *skip) {
    skipAvailable = true;
    this->skip = skip;
}

void PostingSkip::setNext(PostingSkip *next) {
    this->_next = next;
}

PostingSkip *PostingSkip::next() {
    return _next;
}

PostingSkip *PostingSkip::getSkip() {
    return skip;
}
