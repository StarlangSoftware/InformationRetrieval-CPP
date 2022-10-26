//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#include "QueryResultItem.h"

QueryResultItem::QueryResultItem(int docId, double score) {
    this->docId = docId;
    this->score = score;
}

int QueryResultItem::getDocId() const{
    return docId;
}

double QueryResultItem::getScore() const{
    return score;
}

QueryResultItem::QueryResultItem() {

}
