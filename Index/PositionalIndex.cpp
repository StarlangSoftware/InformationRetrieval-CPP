//
// Created by Olcay Taner YILDIZ on 25.07.2022.
//

#include <string>
#include <fstream>
#include "PositionalIndex.h"

PositionalIndex::PositionalIndex() = default;

PositionalIndex::PositionalIndex(string fileName) {
    readPositionalPostingList(fileName);
}

PositionalIndex::PositionalIndex(TermDictionary dictionary, vector<TermOccurrence> terms, Comparator comparator) {
    if (terms.size() > 0){
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

