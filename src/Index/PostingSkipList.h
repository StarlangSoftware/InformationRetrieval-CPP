//
// Created by Olcay Taner YILDIZ on 25.07.2022.
//

#ifndef INFORMATIONRETRIEVAL_POSTINGSKIPLIST_H
#define INFORMATIONRETRIEVAL_POSTINGSKIPLIST_H


#include "PostingList.h"

class PostingSkipList : public PostingList{
private:
    bool skipped = false;
public:
    PostingSkipList();
    void add(int docId);
    void addSkipPointers();
    PostingSkipList intersection(PostingSkipList secondList);
};


#endif //INFORMATIONRETRIEVAL_POSTINGSKIPLIST_H
