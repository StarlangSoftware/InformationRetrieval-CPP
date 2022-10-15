//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#ifndef INFORMATIONRETRIEVAL_QUERYRESULTITEM_H
#define INFORMATIONRETRIEVAL_QUERYRESULTITEM_H


class QueryResultItem {
private:
    int docId;
    double score;
public:
    QueryResultItem();
    QueryResultItem(int docId, double score);
    int getDocId();
    double getScore();
};


#endif //INFORMATIONRETRIEVAL_QUERYRESULTITEM_H
