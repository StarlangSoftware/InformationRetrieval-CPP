//
// Created by Olcay Taner YILDIZ on 25.07.2022.
//

#include "IncidenceMatrix.h"

/**
 * Empty constructor for the incidence matrix representation. Initializes the incidence matrix according to the
 * given dictionary and document size.
 * @param dictionarySize Number of words in the dictionary (number of distinct words in the collection)
 * @param documentSize Number of documents in the collection
 */
IncidenceMatrix::IncidenceMatrix(int dictionarySize, int documentSize) {
    this->documentSize = documentSize;
    this->dictionarySize = dictionarySize;
    incidenceMatrix = new bool*[dictionarySize];
    for (int i = 0; i < dictionarySize; i++){
        incidenceMatrix[i] = new bool[documentSize];
        for (int j = 0; j < documentSize; j++)
        {
            incidenceMatrix[i][j] = false;
        }
    }
}

/**
 * Constructs an incidence matrix from a list of sorted tokens in the given terms array.
 * @param dictionary Term dictionary
 * @param terms List of tokens in the memory collection.
 * @param documentSize Number of documents in the collection
 */
IncidenceMatrix::IncidenceMatrix(const vector<TermOccurrence>& terms, TermDictionary& dictionary, int documentSize){
    this->documentSize = documentSize;
    this->dictionarySize = dictionary.size();
    incidenceMatrix = new bool*[this->dictionarySize];
    for (int i = 0; i < dictionary.size(); i++){
        incidenceMatrix[i] = new bool[documentSize];
        for (int j = 0; j < documentSize; j++)
        {
            incidenceMatrix[i][j] = false;
        }
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

/**
 * Sets the given cell in the incidence matrix to true.
 * @param row Row no of the cell
 * @param col Column no of the cell
 */
void IncidenceMatrix::set(int row, int col) const {
    incidenceMatrix[row][col] = true;
}

/**
 * Searches a given query in the document collection using incidence matrix boolean search.
 * @param query Query string
 * @param dictionary Term dictionary
 * @return The result of the query obtained by doing incidence matrix boolean search in the collection.
 */
QueryResult IncidenceMatrix::search(const Query& query, TermDictionary& dictionary) const {
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
        } else {
            return result;
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
