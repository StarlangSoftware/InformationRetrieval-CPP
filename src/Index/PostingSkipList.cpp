//
// Created by Olcay Taner YILDIZ on 25.07.2022.
//

#include <cmath>
#include "PostingSkipList.h"
#include "PostingSkip.h"

PostingSkipList::PostingSkipList() = default;

void PostingSkipList::add(int docId) {
    auto* p = new PostingSkip(docId);
    ((PostingSkip*) postings[postings.size() - 1])->setNext(p);
    postings.emplace_back(p);
}

void PostingSkipList::addSkipPointers() {
    int i, j, N = (int) sqrt(size());
    int posting;
    int skip;
    if (!skipped){
        skipped = true;
        for (i = 0, posting = 0; posting != postings.size(); posting++, i++){
            if (i % N == 0 && i + N < size()){
                for (j = 0, skip = posting; j < N; skip++){
                    j++;
                }
                ((PostingSkip*)postings[posting])->addSkip((PostingSkip*) postings[skip]);
            }
        }
    }
}

PostingSkipList PostingSkipList::intersection(PostingSkipList secondList) {
    auto* p1 = (PostingSkip*) postings[0];
    auto* p2 = (PostingSkip*) secondList.postings[0];
    PostingSkipList result = PostingSkipList();
    while (p1 != nullptr && p2 != nullptr){
        if (p1->getId() == p2->getId()){
            result.add(p1->getId());
            p1 = p1->next();
            p2 = p2->next();
        } else {
            if (p1->getId() < p2->getId()){
                if (skipped && p1->hasSkip() && p1->getSkip()->getId() < p2->getId()){
                    while (p1->hasSkip() && p1->getSkip()->getId() < p2->getId()){
                        p1 = p1->getSkip();
                    }
                } else {
                    p1 = p1->next();
                }
            } else {
                if (skipped && p2->hasSkip() && p2->getSkip()->getId() < p1->getId()){
                    while (p2->hasSkip() && p2->getSkip()->getId() < p1->getId()){
                        p2 = p2->getSkip();
                    }
                } else {
                    p2 = p2->next();
                }
            }
        }
    }
    return result;
}
