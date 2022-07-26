//
// Created by Olcay Taner YILDIZ on 25.07.2022.
//

#include <string>
#include <fstream>
#include "PositionalIndex.h"
#include "../Query/VectorSpaceModel.h"

PositionalIndex::PositionalIndex() = default;

PositionalIndex::PositionalIndex(string fileName) {
    readPositionalPostingList(fileName);
}

PositionalIndex::PositionalIndex(TermDictionary dictionary, vector<TermOccurrence> terms, Comparator comparator) {
    if (!terms.empty()){
        TermOccurrence term = terms[0];
        int i = 1;
        TermOccurrence previousTerm = term;
        int termId = dictionary.getWordIndex(term.getTerm().getName());
        addPosition(termId, term.getDocId(), term.getPosition());
        int prevDocId = term.getDocId();
        while (i < terms.size()){
            term = terms[i];
            termId = dictionary.getWordIndex(term.getTerm().getName());
            if (termId != -1){
                if (term.isDifferent(previousTerm, comparator)){
                    addPosition(termId, term.getDocId(), term.getPosition());
                    prevDocId = term.getDocId();
                } else {
                    if (prevDocId != term.getDocId()){
                        addPosition(termId, term.getDocId(), term.getPosition());
                        prevDocId = term.getDocId();
                    } else {
                        addPosition(termId, term.getDocId(), term.getPosition());
                    }
                }
            }
            i++;
            previousTerm = term;
        }
    }
}

void PositionalIndex::readPositionalPostingList(string fileName) {
    ifstream inputFile;
    string line;
    inputFile.open(fileName, ifstream :: in);
    while (inputFile.good()) {
        getline(inputFile, line);
        vector<string> items = Word::split(line);
        int wordId = stoi(items[0]);
        positionalIndex[wordId] = PositionalPostingList(inputFile, stoi(items[1]));
        getline(inputFile, line);
    }
    inputFile.close();
}

void PositionalIndex::addPosition(int termId, int docId, int position) {
    PositionalPostingList positionalPostingList;
    if (!positionalIndex.contains(termId)){
        positionalPostingList = PositionalPostingList();
    } else {
        positionalPostingList = positionalIndex[termId];
    }
    positionalPostingList.add(docId, position);
    positionalIndex[termId] = positionalPostingList;
}

void PositionalIndex::save(string fileName) {
    ofstream outfile;
    outfile.open(fileName, ofstream :: out);
    for (auto & item : positionalIndex) {
        item.second.writeToFile(outfile, item.first);
    }
    outfile.close();
}

QueryResult PositionalIndex::positionalSearch(Query query, TermDictionary dictionary) {
    PositionalPostingList postingResult = PositionalPostingList();
    for (int i = 0; i < query.size(); i++){
        int term = dictionary.getWordIndex(query.getTerm(i).getName());
        if (term != -1){
            if (i == 0){
                postingResult = positionalIndex[term];
            } else {
                if (postingResult.size() != 0){
                    postingResult = postingResult.intersection(positionalIndex[term]);
                } else {
                    return {};
                }
            }
        } else {
            return {};
        }
    }
    if (postingResult.size() != 0)
        return postingResult.toQueryResult();
    else
        return {};
}

int *PositionalIndex::getTermFrequencies(int docId) {
    int* tf = new int[positionalIndex.size()];
    int i = 0;
    for (auto & item : positionalIndex){
        PositionalPostingList positionalPostingList = positionalIndex[item.first];
        int index = positionalPostingList.getIndex(docId);
        if (index != -1){
            tf[i] = positionalPostingList.get(index).size();
        } else {
            tf[i] = 0;
        }
        i++;
    }
    return tf;
}

int *PositionalIndex::getDocumentFrequencies() {
    int* df = new int[positionalIndex.size()];
    int i = 0;
    for (auto & item : positionalIndex){
        df[i] = positionalIndex[item.first].size();
        i++;
    }
    return df;
}

QueryResult PositionalIndex::rankedSearch(Query query, TermDictionary dictionary, vector<Document> documents,
                                          TermWeighting termWeighting, DocumentWeighting documentWeighting) {
    int N = documents.size();
    auto* scores = new double[N];
    QueryResult result = QueryResult();
    PositionalPostingList positionalPostingList;
    for (int i = 0; i < query.size(); i++){
        int term = dictionary.getWordIndex(query.getTerm(i).getName());
        if (term != -1){
            positionalPostingList = positionalIndex[term];
            for (int j = 0; j < positionalPostingList.size(); j++){
                PositionalPosting positionalPosting = positionalPostingList.get(j);
                int docID = positionalPosting.getDocId();
                int tf = positionalPosting.size();
                int df = positionalIndex[term].size();
                if (tf > 0 && df > 0){
                    scores[docID] += VectorSpaceModel::weighting(tf, df, N, termWeighting, documentWeighting);
                }
            }
        }
    }
    for (int i = 0; i < N; i++){
        scores[i] /= documents[i].getSize();
        if (scores[i] > 0.0){
            result.add(i, scores[i]);
        }
    }
    result.sort();
    return result;
}

