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
    int size();
    int getIndex(int docId);
    QueryResult toQueryResult();
    void add(int docId, int position);
    PositionalPosting get(int index);
    PositionalPostingList unionWith(PositionalPostingList second);
    PositionalPostingList intersection(PositionalPostingList secondList);
    string to_string();
    void writeToFile(ofstream& outfile, int index);
};


#endif //INFORMATIONRETRIEVAL_POSITIONALPOSTINGLIST_H
