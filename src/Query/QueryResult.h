//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#ifndef INFORMATIONRETRIEVAL_QUERYRESULT_H
#define INFORMATIONRETRIEVAL_QUERYRESULT_H

#include <vector>
#include "QueryResultItem.h"

using namespace std;

class QueryResult {
private:
    vector<QueryResultItem> items;
public:
    void add(int docId, double score);
    void add(int docId);
    vector<QueryResultItem> getItems();
    void getBest(int K);
};


#endif //INFORMATIONRETRIEVAL_QUERYRESULT_H
