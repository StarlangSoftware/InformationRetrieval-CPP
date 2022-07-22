//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#ifndef INFORMATIONRETRIEVAL_QUERY_H
#define INFORMATIONRETRIEVAL_QUERY_H


#include <vector>
#include "Word.h"

class Query {
private:
    vector<Word> terms;
public:
    explicit Query(string query);
    Word getTerm(int index);
    int size();
};


#endif //INFORMATIONRETRIEVAL_QUERY_H
