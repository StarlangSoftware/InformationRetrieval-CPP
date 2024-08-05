//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#include <string>
#include "PositionalPosting.h"

/**
 * Constructor for the PositionalPosting class. Sets the document id and initializes the position array.
 * @param docId document id of the posting.
 */
PositionalPosting::PositionalPosting(int docId) {
    this->docId = docId;
}

/**
 * Adds a position to the position list.
 * @param position Position added to the position list.
 */
void PositionalPosting::add(int position) {
    positions.emplace_back(position);
}

/**
 * Accessor for the document id attribute.
 * @return Document id.
 */
int PositionalPosting::getDocId() const{
    return docId;
}

/**
 * Accessor for the positions attribute.
 * @return Position list.
 */
vector<Posting> PositionalPosting::getPositions() const{
    return positions;
}

/**
 * Returns size of the position list.
 * @return Size of the position list.
 */
int PositionalPosting::size() const{
    return positions.size();
}

/**
 * Converts the positional posting to a string. String is of the form, document id, number of positions, and all
 * positions separated via space.
 * @return String form of the positional posting.
 */
string PositionalPosting::to_string() const{
    string result = ::to_string(docId) + " " + ::to_string(positions.size());
    for (Posting posting : positions){
        result += " ";
        result += ::to_string(posting.getId());
    }
    return result;
}
