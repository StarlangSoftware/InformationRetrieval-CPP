//
// Created by Olcay Taner YILDIZ on 25.07.2022.
//

#include "IncidenceMatrix.h"

IncidenceMatrix::IncidenceMatrix(int dictionarySize, int documentSize) {
    this->documentSize = documentSize;
    this->dictionarySize = dictionarySize;
    incidenceMatrix = new bool*[dictionarySize];
    for (int i = 0; i < dictionarySize; i++){
        incidenceMatrix[i] = new bool[documentSize];
    }
}

IncidenceMatrix::IncidenceMatrix(vector<TermOccurrence> terms, TermDictionary dictionary, int documentSize){
    this->documentSize = documentSize;
    this->dictionarySize = dictionary.size();
    incidenceMatrix = new bool*[dictionary.size()];
    for (int i = 0; i < dictionary.size(); i++){
        incidenceMatrix[i] = new bool[documentSize];
    }
    if (terms.size() > 0){
        TermOccurrence term = terms[0];
        int i = 1;
        int termIndex = dictionary.getWordIndex(term.getTerm().getName());
        set(termIndex, term.getDocId());
        while (i < terms.size()){
            term = terms[i];
            termIndex = dictionary.getWordIndex(term.getTerm().getName());
            set(termIndex, term.getDocId());
            i++;
        }
    }
}

void IncidenceMatrix::set(int row, int col) {
    incidenceMatrix[row][col] = true;
}

QueryResult IncidenceMatrix::search(Query query, TermDictionary dictionary) {
    QueryResult result = QueryResult();
    bool* resultRow = new bool[documentSize];
    for (int i = 0; i < documentSize; i++){
        resultRow[i] = true;
    }
    for (int i = 0; i < query.size(); i++){
        int termIndex = dictionary.getWordIndex(query.getTerm(i).getName());
        if (termIndex != -1){
            for (int j = 0; j < documentSize; j++){
                resultRow[j] = resultRow[j] && incidenceMatrix[termIndex][j];
            }
        }
    }
    for (int i = 0; i < documentSize; i++){
        if (resultRow[i]){
            result.add(i);
        }
    }
    return result;
}

IncidenceMatrix::IncidenceMatrix() = default;
