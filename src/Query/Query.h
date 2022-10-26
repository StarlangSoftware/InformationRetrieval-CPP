//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#ifndef INFORMATIONRETRIEVAL_QUERY_H
#define INFORMATIONRETRIEVAL_QUERY_H


#include <vector>
#include "Dictionary/Word.h"

class Query {
private:
    vector<Word> terms;
public:
    explicit Query(const string& query);
    Word getTerm(int index) const;
    int size() const;
};


#endif //INFORMATIONRETRIEVAL_QUERY_H
