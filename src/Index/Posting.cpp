//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#include "Posting.h"

/**
 * Constructor for the Posting class. Sets the document id attribute.
 * @param id Document id.
 */
Posting::Posting(int id) {
    this->id = id;
}

/**
 * Accessor for the document id attribute.
 * @return Document id.
 */
int Posting::getId() const{
    return id;
}
