//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#include "PostingSkip.h"

PostingSkip::PostingSkip(int id) : Posting(id) {
}

bool PostingSkip::hasSkip() const{
    return skipAvailable;
}

void PostingSkip::addSkip(PostingSkip *_skip) {
    skipAvailable = true;
    this->skip = _skip;
}

void PostingSkip::setNext(PostingSkip *next) {
    this->_next = next;
}

PostingSkip *PostingSkip::next() const{
    return _next;
}

PostingSkip *PostingSkip::getSkip() const{
    return skip;
}
