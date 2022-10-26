//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#ifndef INFORMATIONRETRIEVAL_POSITIONALPOSTING_H
#define INFORMATIONRETRIEVAL_POSITIONALPOSTING_H


#include "Posting.h"
#include <vector>

using namespace std;

class PositionalPosting {
private:
    vector<Posting> positions;
    int docId;
public:
    explicit PositionalPosting(int docId);
    void add(int position);
    int getDocId() const;
    vector<Posting> getPositions() const;
    int size() const;
    string to_string() const;
};


#endif //INFORMATIONRETRIEVAL_POSITIONALPOSTING_H
