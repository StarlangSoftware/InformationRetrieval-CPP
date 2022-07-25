//
// Created by Olcay Taner YILDIZ on 25.07.2022.
//

#include <fstream>
#include "InvertedIndex.h"
#include "PostingListComparator.h"

InvertedIndex::InvertedIndex() = default;

InvertedIndex::InvertedIndex(TermDictionary dictionary, vector<TermOccurrence> terms, Comparator comparator) {
    if (terms.size() > 0){
        TermOccurrence term = terms[0];
        int i = 1;
        TermOccurrence previousTerm = term;
        int termId = dictionary.getWordIndex(term.getTerm().getName());
        add(termId, term.getDocId());
        int prevDocId = term.getDocId();
        while (i < terms.size()){
            term = terms[i];
            termId = dictionary.getWordIndex(term.getTerm().getName());
            if (termId != -1){
                if (term.isDifferent(previousTerm, comparator)){
                    add(termId, term.getDocId());
                    prevDocId = term.getDocId();
                } else {
                    if (prevDocId != term.getDocId()){
                        add(termId, term.getDocId());
                        prevDocId = term.getDocId();
                    }
                }
            }
            i++;
            previousTerm = term;
        }
    }
}

void InvertedIndex::readPostingList(string fileName) {
    ifstream inputFile;
    string line;
    inputFile.open(fileName, ifstream :: in);
    while (inputFile.good()) {
        getline(inputFile, line);
        vector<string> items = Word::split(line);
        int wordId = stoi(items[0]);
        getline(inputFile, line);
        index[wordId] = PostingList(line);
        getline(inputFile, line);
    }
    inputFile.close();
}

InvertedIndex::InvertedIndex(string fileName) {
    readPostingList(fileName);
}

void InvertedIndex::save(string fileName) {
    ofstream outfile;
    outfile.open(fileName, ofstream :: out);
    for (auto & item : index) {
        item.second.writeToFile(outfile, item.first);
    }
    outfile.close();
}

void InvertedIndex::add(int termId, int docId) {
    PostingList postingList;
    if (!index.contains(termId)){
        postingList = PostingList();
    } else {
        postingList = index[termId];
    }
    postingList.add(docId);
    index[termId] = postingList;
}

QueryResult InvertedIndex::search(Query query, TermDictionary dictionary) {
    vector<PostingList> queryTerms;
    for (int i = 0; i < query.size(); i++){
        int termIndex = dictionary.getWordIndex(query.getTerm(i).getName());
        if (termIndex != -1){
            queryTerms.emplace_back(index[termIndex]);
        }
    }
    std::sort(queryTerms.begin(), queryTerms.end(), postingListComparator());
    PostingList result = queryTerms[0];
    for (int i = 1; i < queryTerms.size(); i++){
        result = result.intersection(queryTerms[i]);
    }
    return result.toQueryResult();
}
