//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#include "QueryResultItem.h"

/**
 * Constructor for the QueryResultItem class. Sets the document id and score of a single query result.
 * @param docId Id of the document that satisfies the query.
 * @param score Score of the document for the query.
 */
QueryResultItem::QueryResultItem(int docId, double score) {
    this->docId = docId;
    this->score = score;
}

/**
 * Accessor for the docID attribute.
 * @return docID attribute
 */
int QueryResultItem::getDocId() const{
    return docId;
}

/**
 * Accessor for the score attribute.
 * @return score attribute.
 */
double QueryResultItem::getScore() const{
    return score;
}

QueryResultItem::QueryResultItem() {

}
