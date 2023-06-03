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
    vector<QueryResultItem> getItems() const;
    void getBest(int K);
    int size() const;
    QueryResult intersectionFastSearch(const QueryResult& queryResult) const;
    QueryResult intersectionBinarySearch(const QueryResult& queryResult) const;
    QueryResult intersectionLinearSearch(const QueryResult& queryResult) const;
};


#endif //INFORMATIONRETRIEVAL_QUERYRESULT_H
