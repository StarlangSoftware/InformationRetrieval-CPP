//
// Created by Olcay Taner YILDIZ on 25.07.2022.
//

#ifndef INFORMATIONRETRIEVAL_POSTINGLISTCOMPARATOR_H
#define INFORMATIONRETRIEVAL_POSTINGLISTCOMPARATOR_H

#include "PostingList.h"

struct postingListComparator{

    /**
     * Comparator method to compare two posting lists.
     * @param listA the first posting list to be compared.
     * @param listB the second posting list to be compared.
     * @return 1 if the size of the first posting list is larger than the second one, -1 if the size
     * of the first posting list is smaller than the second one, 0 if they are the same.
     */
    bool operator() (const PostingList& listA, const PostingList& listB){
        if (listA.size() <= listB.size()){
            return false;
        } else {
            return true;
        }
    }
};

#endif //INFORMATIONRETRIEVAL_POSTINGLISTCOMPARATOR_H
