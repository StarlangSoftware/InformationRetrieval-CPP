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
    bool hasSkip() const;
    void addSkip(PostingSkip* _skip);
    void setNext(PostingSkip* next);
    PostingSkip* next() const;
    PostingSkip* getSkip() const;
};


#endif //INFORMATIONRETRIEVAL_POSTINGSKIP_H
