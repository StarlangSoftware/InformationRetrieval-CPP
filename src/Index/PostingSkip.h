//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#ifndef INFORMATIONRETRIEVAL_POSTINGSKIP_H
#define INFORMATIONRETRIEVAL_POSTINGSKIP_H


#include "Posting.h"

class PostingSkip : public Posting{
private:
    bool skipAvailable = false;
    PostingSkip* skip = nullptr;
    PostingSkip* _next = nullptr;
public:
    explicit PostingSkip(int id);
    bool hasSkip();
    void addSkip(PostingSkip* skip);
    void setNext(PostingSkip* next);
    PostingSkip* next();
    PostingSkip* getSkip();
};


#endif //INFORMATIONRETRIEVAL_POSTINGSKIP_H
