//
// Created by Olcay Taner YILDIZ on 25.07.2022.
//

#include <fstream>
#include <iostream>
#include "InvertedIndex.h"
#include "PostingListComparator.h"

InvertedIndex::InvertedIndex() = default;

InvertedIndex::InvertedIndex(TermDictionary& dictionary, const vector<TermOccurrence>& terms) {
    if (!terms.empty()){
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
                if (term.isDifferent(previousTerm)){
                    add(termId, term.getDocId());
                    prevDocId = term.getDocId();
                } else {
                    if (prevDocId != term.getDocId()){
                        add(termId, term.getDocId());
                        prevDocId = term.getDocId();
                    }
                }
            } else {
                cout << "Word " << term.getTerm().getName() << " not found in dictionary" << "\n";
            }
            i++;
            previousTerm = term;
        }
    }
}

void InvertedIndex::readPostingList(const string& fileName) {
    ifstream inputFile;
    string line;
    inputFile.open(fileName + "-postings.txt", ifstream :: in);
    while (inputFile.good()) {
        getline(inputFile, line);
        if (line.empty()){
            continue;
        }
        vector<string> items = Word::split(line);
        int wordId = stoi(items[0]);
        getline(inputFile, line);
        index[wordId] = PostingList(line);
    }
    inputFile.close();
}

InvertedIndex::InvertedIndex(const string& fileName) {
    readPostingList(fileName);
}

void InvertedIndex::save(const string& fileName) {
    ofstream outfile;
    outfile.open(fileName + "-postings.txt", ofstream :: out);
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

QueryResult InvertedIndex::search(const Query& query, TermDictionary& dictionary) {
    vector<PostingList> queryTerms;
    for (int i = 0; i < query.size(); i++){
        int termIndex = dictionary.getWordIndex(query.getTerm(i).getName());
        if (termIndex != -1){
            queryTerms.emplace_back(index[termIndex]);
        } else {
            return QueryResult();
        }
    }
    std::sort(queryTerms.begin(), queryTerms.end(), postingListComparator());
    PostingList result = queryTerms[0];
    for (int i = 1; i < queryTerms.size(); i++){
        result = result.intersection(queryTerms[i]);
    }
    return result.toQueryResult();
}
