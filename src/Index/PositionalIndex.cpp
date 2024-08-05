//
// Created by Olcay Taner YILDIZ on 25.07.2022.
//

#include <string>
#include <fstream>
#include <iostream>
#include "PositionalIndex.h"
#include "../Query/VectorSpaceModel.h"

/**
 * Constructs an empty inverted index.
 */
PositionalIndex::PositionalIndex() = default;

/**
 * Reads the positional inverted index from an input file.
 * @param fileName Input file name for the positional inverted index.
 */
PositionalIndex::PositionalIndex(const string &fileName) {
    readPositionalPostingList(fileName);
}

/**
 * Constructs a positional inverted index from a list of sorted tokens. The terms array should be sorted before
 * calling this method. Multiple occurrences of the same term from the same document are enlisted separately in the
 * index.
 * @param dictionary Term dictionary
 * @param terms Sorted list of tokens in the memory collection.
 * @param comparator Comparator method to compare two terms.
 */
PositionalIndex::PositionalIndex(TermDictionary &dictionary, const vector<TermOccurrence> &terms) {
    if (!terms.empty()) {
        TermOccurrence term = terms[0];
        int i = 1;
        TermOccurrence previousTerm = term;
        int termId = dictionary.getWordIndex(term.getTerm().getName());
        addPosition(termId, term.getDocId(), term.getPosition());
        int prevDocId = term.getDocId();
        while (i < terms.size()) {
            term = terms[i];
            termId = dictionary.getWordIndex(term.getTerm().getName());
            if (termId != -1) {
                if (term.isDifferent(previousTerm)) {
                    addPosition(termId, term.getDocId(), term.getPosition());
                    prevDocId = term.getDocId();
                } else {
                    if (prevDocId != term.getDocId()) {
                        addPosition(termId, term.getDocId(), term.getPosition());
                        prevDocId = term.getDocId();
                    } else {
                        addPosition(termId, term.getDocId(), term.getPosition());
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

/**
 * Reads the positional postings list of the positional index from an input file. The postings are stored in n
 * lines. The first line contains the term id and the number of documents that term occurs. Other n - 1 lines
 * contain the postings list for that term for a separate document.
 * @param fileName Positional index file.
 */
void PositionalIndex::readPositionalPostingList(const string &fileName) {
    ifstream inputFile;
    string line;
    inputFile.open(fileName + "-positionalPostings.txt", ifstream::in);
    while (inputFile.good()) {
        getline(inputFile, line);
        if (line.empty()) {
            continue;
        }
        vector<string> items = Word::split(line);
        int wordId = stoi(items[0]);
        positionalIndex[wordId] = PositionalPostingList(inputFile, stoi(items[1]));
    }
    inputFile.close();
}

/**
 * Adds a possible new term with a position and document id to the positional index. First the term is searched in
 * the hash map, then the position and the document id is put into the correct postings list.
 * @param termId Id of the term
 * @param docId Document id in which the term exists
 * @param position Position of the term in the document with id docId
 */
void PositionalIndex::addPosition(int termId, int docId, int position) {
    PositionalPostingList positionalPostingList;
    if (!positionalIndex.contains(termId)) {
        positionalPostingList = PositionalPostingList();
    } else {
        positionalPostingList = positionalIndex[termId];
    }
    positionalPostingList.add(docId, position);
    positionalIndex[termId] = positionalPostingList;
}

/**
 * Saves the positional index into the index file. The postings are stored in n lines. The first line contains the
 * term id and the number of documents that term occurs. Other n - 1 lines contain the postings list for that term
 * for a separate document.
 * @param fileName Index file name. Real index file name is created by attaching -positionalPostings.txt to this
 *                 file name
 */
void PositionalIndex::save(const string &fileName) {
    ofstream outfile;
    outfile.open(fileName + "-positionalPostings.txt", ofstream::out);
    for (auto &item: positionalIndex) {
        item.second.writeToFile(outfile, item.first);
    }
    outfile.close();
}

/**
 * Searches a given query in the document collection using positional index boolean search.
 * @param query Query string
 * @param dictionary Term dictionary
 * @return The result of the query obtained by doing positional index boolean search in the collection.
 */
QueryResult PositionalIndex::positionalSearch(const Query &query, TermDictionary &dictionary) {
    PositionalPostingList postingResult = PositionalPostingList();
    for (int i = 0; i < query.size(); i++) {
        int term = dictionary.getWordIndex(query.getTerm(i).getName());
        if (term != -1) {
            if (i == 0) {
                postingResult = positionalIndex[term];
            } else {
                if (postingResult.size() != 0) {
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

/**
 * Returns the term frequencies  in a given document.
 * @param docId Id of the document
 * @return Term frequencies of the given document.
 */
int *PositionalIndex::getTermFrequencies(int docId) const {
    int *tf = new int[positionalIndex.size()];
    int i = 0;
    for (auto &item: positionalIndex) {
        PositionalPostingList positionalPostingList = positionalIndex.at(item.first);
        int index = positionalPostingList.getIndex(docId);
        if (index != -1) {
            tf[i] = positionalPostingList.get(index).size();
        } else {
            tf[i] = 0;
        }
        i++;
    }
    return tf;
}

/**
 * Returns the document frequencies of the terms in the collection.
 * @return The document frequencies of the terms in the collection.
 */
int *PositionalIndex::getDocumentFrequencies() const {
    int *df = new int[positionalIndex.size()];
    int i = 0;
    for (auto &item: positionalIndex) {
        df[i] = positionalIndex.at(item.first).size();
        i++;
    }
    return df;
}

/**
 * Searches a given query in the document collection using inverted index ranked search.
 * @param query Query string
 * @param dictionary Term dictionary
 * @param documents Document collection
 * @param parameter Search parameter
 * @return The result of the query obtained by doing inverted index ranked search in the collection.
 */
QueryResult
PositionalIndex::rankedSearch(const Query &query,
                              TermDictionary &dictionary,
                              const vector<Document> &documents,
                              const SearchParameter searchParameter) {
    int N = documents.size();
    map<int, double> scores;
    QueryResult result = QueryResult();
    PositionalPostingList positionalPostingList;
    for (int i = 0; i < query.size(); i++) {
        int term = dictionary.getWordIndex(query.getTerm(i).getName());
        if (term != -1) {
            positionalPostingList = positionalIndex[term];
            for (int j = 0; j < positionalPostingList.size(); j++) {
                PositionalPosting positionalPosting = positionalPostingList.get(j);
                int docID = positionalPosting.getDocId();
                int tf = positionalPosting.size();
                int df = positionalIndex[term].size();
                if (tf > 0 && df > 0) {
                    double score = VectorSpaceModel::weighting(tf,
                                                               df,
                                                               N,
                                                               searchParameter.getTermWeighting(),
                                                               searchParameter.getDocumentWeighting());
                    if (scores.contains(docID)) {
                        scores.insert_or_assign(docID, scores[docID] + score);
                    } else {
                        scores.insert_or_assign(docID, score);
                    }
                }
            }
        }
    }
    for (auto &iterator: scores) {
        result.add(iterator.first, iterator.second / documents[iterator.first].getSize());
    }
    return result;
}

/**
 * Returns the document frequencies of the terms in the collection.
 * @return The document frequencies of the terms in the collection.
 */
int *PositionalIndex::getDocumentSizes(int documentSize) const {
    int *sizes = new int[documentSize];
    for (auto &iterator: positionalIndex) {
        for (int j = 0; j < iterator.second.size(); j++) {
            PositionalPosting positionalPosting = iterator.second.get(j);
            int docId = positionalPosting.getDocId();
            sizes[docId] += positionalPosting.size();
        }
    }
    return sizes;
}

/**
 * Calculates and updates the frequency counts of the terms in each category node.
 * @param documents Document collection.
 */
void PositionalIndex::setCategoryCounts(vector<Document> &documents) {
    for (auto &iterator: positionalIndex) {
        for (int j = 0; j < iterator.second.size(); j++) {
            PositionalPosting positionalPosting = iterator.second.get(j);
            int docId = positionalPosting.getDocId();
            documents[docId].getCategory()->addCounts(iterator.first, positionalPosting.size());
        }
    }
}

