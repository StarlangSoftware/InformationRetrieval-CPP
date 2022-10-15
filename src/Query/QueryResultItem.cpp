//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#include "QueryResultItem.h"

QueryResultItem::QueryResultItem(int docId, double score) {
    this->docId = docId;
    this->score = score;
}

int QueryResultItem::getDocId() {
    return docId;
}

double QueryResultItem::getScore() {
    return score;
}

QueryResultItem::QueryResultItem() {

}
