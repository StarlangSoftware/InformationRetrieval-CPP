//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#include <string>
#include "PositionalPosting.h"

PositionalPosting::PositionalPosting(int docId) {
    this->docId = docId;
}

void PositionalPosting::add(int position) {
    positions.emplace_back(position);
}

int PositionalPosting::getDocId() const{
    return docId;
}

vector<Posting> PositionalPosting::getPositions() const{
    return positions;
}

int PositionalPosting::size() const{
    return positions.size();
}

string PositionalPosting::to_string() const{
    string result = ::to_string(docId) + " " + ::to_string(positions.size());
    for (Posting posting : positions){
        result += " ";
        result += ::to_string(posting.getId());
    }
    return result;
}
