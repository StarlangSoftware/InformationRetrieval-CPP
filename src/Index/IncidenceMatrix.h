//
// Created by Olcay Taner YILDIZ on 25.07.2022.
//

#ifndef INFORMATIONRETRIEVAL_INCIDENCEMATRIX_H
#define INFORMATIONRETRIEVAL_INCIDENCEMATRIX_H


#include "TermOccurrence.h"
#include "TermDictionary.h"
#include "../Query/QueryResult.h"
#include "../Query/Query.h"

class IncidenceMatrix {
private:
    bool** incidenceMatrix = nullptr;
    int dictionarySize{};
    int documentSize{};
public:
    IncidenceMatrix();
    IncidenceMatrix(int dictionarySize, int documentSize);
    IncidenceMatrix(vector<TermOccurrence> terms, TermDictionary dictionary, int documentSize);
    void set(int row, int col);
    QueryResult search(Query query, TermDictionary dictionary);
};


#endif //INFORMATIONRETRIEVAL_INCIDENCEMATRIX_H
