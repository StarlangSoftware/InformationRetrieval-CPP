//
// Created by Olcay Taner YILDIZ on 25.07.2022.
//

#ifndef INFORMATIONRETRIEVAL_POSITIONALPOSTINGLIST_H
#define INFORMATIONRETRIEVAL_POSITIONALPOSTINGLIST_H


#include "PositionalPosting.h"
#include "../Query/QueryResult.h"

class PositionalPostingList {
private:
    vector<PositionalPosting> postings;
public:
    PositionalPostingList();
    PositionalPostingList(ifstream &infile, int count);
    int size() const;
    int getIndex(int docId) const;
    QueryResult toQueryResult() const;
    void add(int docId, int position);
    PositionalPosting get(int index) const;
    PositionalPostingList unionWith(const PositionalPostingList& second) const;
    PositionalPostingList intersection(const PositionalPostingList& secondList) const;
    string to_string() const;
    void writeToFile(ofstream& outfile, int index);
};


#endif //INFORMATIONRETRIEVAL_POSITIONALPOSTINGLIST_H
