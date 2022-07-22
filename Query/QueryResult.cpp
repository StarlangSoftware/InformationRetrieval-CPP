//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#include "QueryResult.h"
#include "QuertResultItemComparator.h"

void QueryResult::add(int docId, double score) {
    items.emplace_back(QueryResultItem(docId, score));
}

void QueryResult::add(int docId) {
    items.emplace_back(QueryResultItem(docId, 0.0));
}

vector<QueryResultItem> QueryResult::getItems() {
    return items;
}

void QueryResult::sort() {
    std::sort(items.begin(), items.end(), queryResultItemComparator());
}
