//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#ifndef INFORMATIONRETRIEVAL_QUERTRESULTITEMCOMPARATOR_H
#define INFORMATIONRETRIEVAL_QUERTRESULTITEMCOMPARATOR_H

#include "QueryResultItem.h"

struct queryResultItemComparator{

    bool operator() (QueryResultItem resultA, QueryResultItem resultB){
        return resultA.getScore() > resultB.getScore();
    }
};

#endif //INFORMATIONRETRIEVAL_QUERTRESULTITEMCOMPARATOR_H
