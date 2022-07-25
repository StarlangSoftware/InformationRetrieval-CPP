//
// Created by Olcay Taner YILDIZ on 25.07.2022.
//

#ifndef INFORMATIONRETRIEVAL_POSTINGLIST_H
#define INFORMATIONRETRIEVAL_POSTINGLIST_H

#include <vector>
#include <string>
#include "Posting.h"
#include "../Query/QueryResult.h"

using namespace  std;

class PostingList {
protected:
    vector<Posting*> postings;
public:
    PostingList();
    explicit PostingList(string line);
    void add(int docId);
    int size();
    PostingList intersection(PostingList secondList);
    PostingList unionWith(PostingList secondList);
    QueryResult toQueryResult();
    void writeToFile(ofstream& outfile, int index);
    string to_string();
};


#endif //INFORMATIONRETRIEVAL_POSTINGLIST_H
