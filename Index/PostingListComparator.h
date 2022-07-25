//
// Created by Olcay Taner YILDIZ on 25.07.2022.
//

#ifndef INFORMATIONRETRIEVAL_POSTINGLISTCOMPARATOR_H
#define INFORMATIONRETRIEVAL_POSTINGLISTCOMPARATOR_H

#include "PostingList.h"

struct postingListComparator{

    bool operator() (PostingList listA, PostingList listB){
        if (listA.size() <= listB.size()){
            return false;
        } else {
            return true;
        }
    }
};

#endif //INFORMATIONRETRIEVAL_POSTINGLISTCOMPARATOR_H
