//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#include <MaxHeap.h>
#include "QueryResult.h"

void QueryResult::add(int docId, double score) {
    items.emplace_back(QueryResultItem(docId, score));
}

void QueryResult::add(int docId) {
    items.emplace_back(QueryResultItem(docId, 0.0));
}

vector<QueryResultItem> QueryResult::getItems() {
    return items;
}

int isSmallerQueryResultItem(QueryResultItem first, QueryResultItem second){
    if (first.getScore() - second.getScore() < 0){
        return -1;
    } else {
        if (first.getScore() - second.getScore() > 0){
            return 1;
        } else {
            return 0;
        }
    }
}

void QueryResult::getBest(int K) {
    MaxHeap<QueryResultItem> maxHeap = MaxHeap<QueryResultItem>(2 * K, isSmallerQueryResultItem);
    for (QueryResultItem queryResultItem : items){
        maxHeap.insert(queryResultItem);
    }
    items.clear();
    for (int i = 0; i < K && !maxHeap.isEmpty(); i++){
        items.emplace_back(maxHeap.deleteTop());
    }
}
