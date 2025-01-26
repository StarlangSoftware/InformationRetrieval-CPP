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
    explicit PostingList(const string& line);
    void add(int docId);
    int size() const;
    PostingList intersection(const PostingList& secondList) const;
    PostingList unionWith(const PostingList& secondList) const;
    QueryResult toQueryResult() const;
    void writeToFile(ofstream& outfile, int index) const;
    string to_string() const;
};


#endif //INFORMATIONRETRIEVAL_POSTINGLIST_H
