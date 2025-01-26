//
// Created by Olcay Taner YILDIZ on 25.07.2022.
//

#include <fstream>
#include <iostream>
#include "InvertedIndex.h"
#include <StringUtils.h>
#include "PostingListComparator.h"

/**
 * Constructs an empty inverted index.
 */
InvertedIndex::InvertedIndex() = default;

/**
 * Constructs an inverted index from a list of sorted tokens. The terms array should be sorted before calling this
 * method. Multiple occurrences of the same term from the same document are merged in the index. Instances of the
 * same term are then grouped, and the result is split into a postings list.
 * @param dictionary Term dictionary
 * @param terms Sorted list of tokens in the memory collection.
 */
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

/**
 * Reads the postings list of the inverted index from an input file. The postings are stored in two lines. The first
 * line contains the term id and the number of postings for that term. The second line contains the postings
 * list for that term.
 * @param fileName Inverted index file.
 */
void InvertedIndex::readPostingList(const string& fileName) {
    ifstream inputFile;
    string line;
    inputFile.open(fileName + "-postings.txt", ifstream :: in);
    while (inputFile.good()) {
        getline(inputFile, line);
        if (line.empty()){
            continue;
        }
        vector<string> items = StringUtils::split(line);
        int wordId = stoi(items[0]);
        getline(inputFile, line);
        index[wordId] = PostingList(line);
    }
    inputFile.close();
}

/**
 * Reads the inverted index from an input file.
 * @param fileName Input file name for the inverted index.
 */
InvertedIndex::InvertedIndex(const string& fileName) {
    readPostingList(fileName);
}

/**
 * Saves the inverted index into the index file. The postings are stored in two lines. The first
 * line contains the term id and the number of postings for that term. The second line contains the postings
 * list for that term.
 * @param fileName Index file name. Real index file name is created by attaching -postings.txt to this
 *                 file name
 */
void InvertedIndex::save(const string& fileName) {
    ofstream outfile;
    outfile.open(fileName + "-postings.txt", ofstream :: out);
    for (auto & item : index) {
        item.second.writeToFile(outfile, item.first);
    }
    outfile.close();
}

/**
 * Adds a possible new term with a document id to the inverted index. First the term is searched in the hash map,
 * then the document id is put into the correct postings list.
 * @param termId Id of the term
 * @param docId Document id in which the term exists
 */
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

/**
 * Searches a given query in the document collection using inverted index boolean search.
 * @param query Query string
 * @param dictionary Term dictionary
 * @return The result of the query obtained by doing inverted index boolean search in the collection.
 */
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

/**
 * Constructs a sorted array list of frequency counts for a word list and also sorts the word list according to
 * those frequencies.
 * @param wordList Word list for which frequency array is constructed.
 * @param dictionary Term dictionary
 */
void InvertedIndex::autoCompleteWord(vector<string> &wordList, TermDictionary &dictionary) {
    vector<int> counts;
    for (const string& word : wordList){
        counts.emplace_back(index[dictionary.getWordIndex(word)].size());
    }
    for (int i = 0; i < wordList.size() - 1; i++){
        for (int j = i + 1; j < wordList.size(); j++){
            if (counts[i] < counts[j]){
                std::swap(counts[i], counts[j]);
                std::swap(wordList[i], wordList[j]);
            }
        }
    }
}
