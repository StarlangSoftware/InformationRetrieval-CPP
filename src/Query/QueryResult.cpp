//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#include <MinHeap.h>
#include "QueryResult.h"

/**
 * Adds a new result item to the list of query result.
 * @param docId Document id of the result
 * @param score Score of the result
 */
void QueryResult::add(int docId, double score) {
    items.emplace_back(QueryResultItem(docId, score));
}

/**
 * Adds a new result item with score 0 to the list of query result.
 * @param docId Document id of the result
 */
void QueryResult::add(int docId) {
    items.emplace_back(QueryResultItem(docId, 0.0));
}

/**
 * Returns result list for query
 * @return Result list for query
 */
vector<QueryResultItem> QueryResult::getItems() const{
    return items;
}

int isSmallerQueryResultItem(const QueryResultItem& first, const QueryResultItem& second){
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

/**
 * The method returns K best results from the query result using min heap in O(K log N + N log K) time.
 * @param K Size of the best subset.
 */
void QueryResult::getBest(int K){
    MinHeap<QueryResultItem> minHeap = MinHeap<QueryResultItem>(K, isSmallerQueryResultItem);
    for (int i = 0; i < K && i < items.size(); i++){
        minHeap.insert(items[i]);
    }
    for (int i = K + 1; i < items.size(); i++){
        QueryResultItem top = minHeap.deleteTop();
        if (isSmallerQueryResultItem(top, items[i]) > 0){
            minHeap.insert(top);
        } else {
            minHeap.insert(items[i]);
        }
    }
    items.clear();
    for (int i = 0; i < K && !minHeap.isEmpty(); i++){
        items.insert(items.begin(), minHeap.deleteTop());
    }
}

/**
 * Returns number of results for query
 * @return Number of results for query
 */
int QueryResult::size() const{
    return items.size();
}

/**
 * Given two query results, this method identifies the intersection of those two results by doing parallel iteration
 * in O(N).
 * @param queryResult Second query result to be intersected.
 * @return Intersection of this query result with the second query result
 */
QueryResult QueryResult::intersectionFastSearch(const QueryResult &queryResult) const {
    QueryResult result = QueryResult();
    int i = 0, j = 0;
    while (i < size() && j < queryResult.size()){
        QueryResultItem item1 = items[i];
        QueryResultItem item2 = queryResult.items[j];
        if (item1.getDocId() == item2.getDocId()){
            result.add(item1.getDocId());
            i++;
            j++;
        } else {
            if (item1.getDocId() < item2.getDocId()){
                i++;
            } else {
                j++;
            }
        }
    }
    return result;
}

/**
 * Given two query results, this method identifies the intersection of those two results by doing binary search on
 * the second list in O(N log N).
 * @param queryResult Second query result to be intersected.
 * @return Intersection of this query result with the second query result
 */
QueryResult QueryResult::intersectionBinarySearch(const QueryResult &queryResult) const {
    QueryResult result = QueryResult();
    for (QueryResultItem searchedItem : items){
        int low = 0;
        int high = queryResult.size() - 1;
        int middle = (low + high) / 2;
        bool found = false;
        while (low <= high){
            if (searchedItem.getDocId() > queryResult.items[middle].getDocId()){
                low = middle + 1;
            } else {
                if (searchedItem.getDocId() < queryResult.items[middle].getDocId()){
                    high = middle - 1;
                } else {
                    found = true;
                    break;
                }
            }
            middle = (low + high) / 2;
        }
        if (found){
            result.add(searchedItem.getDocId(), searchedItem.getScore());
        }
    }
    return result;
}

/**
 * Given two query results, this method identifies the intersection of those two results by doing exhaustive search
 * on the second list in O(N^2).
 * @param queryResult Second query result to be intersected.
 * @return Intersection of this query result with the second query result
 */
QueryResult QueryResult::intersectionLinearSearch(const QueryResult &queryResult) const {
    QueryResult result = QueryResult();
    for (QueryResultItem searchedItem : items){
        for (QueryResultItem item : queryResult.items){
            if (searchedItem.getDocId() == item.getDocId()){
                result.add(searchedItem.getDocId(), searchedItem.getScore());
            }
        }
    }
    return result;
}
