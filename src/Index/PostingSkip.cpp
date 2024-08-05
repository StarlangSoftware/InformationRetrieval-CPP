//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#include "PostingSkip.h"

/**
 * Constructor for the PostingSkip class. Sets the document id.
 * @param Id Document id.
 */
PostingSkip::PostingSkip(int id) : Posting(id) {
}

/**
 * Checks if this posting has a skip pointer or not.
 * @return True, if this posting has a skip pointer, false otherwise.
 */
bool PostingSkip::hasSkip() const{
    return skipAvailable;
}

/**
 * Adds a skip pointer to the next skip posting.
 * @param skip Next posting to jump.
 */
void PostingSkip::addSkip(PostingSkip *_skip) {
    skipAvailable = true;
    this->skip = _skip;
}

/**
 * Updated the skip pointer.
 * @param next New skip pointer
 */
void PostingSkip::setNext(PostingSkip *next) {
    this->_next = next;
}

/**
 * Accessor for the skip pointer.
 * @return Next posting to skip.
 */
PostingSkip *PostingSkip::next() const{
    return _next;
}

/**
 * Accessor for the skip.
 * @return Skip
 */
PostingSkip *PostingSkip::getSkip() const{
    return skip;
}
