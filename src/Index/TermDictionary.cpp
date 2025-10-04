//
// Created by Olcay Taner YILDIZ on 24.07.2022.
//

#include <fstream>
#include "TermDictionary.h"

#include <algorithm>

#include "Term.h"
#include "TermOccurrenceComparator.h"

/**
 * Constructor of the TermDictionary. Initializes the comparator for terms and the hasp map.
 */
TermDictionary::TermDictionary() : Dictionary() {
}

/**
 * Constructor of the TermDictionary. Reads the terms and their ids from the given dictionary file. Each line stores
 * the term id and the term name separated via space.
 * @param fileName Dictionary file name
 */
TermDictionary::TermDictionary(const string& fileName) : Dictionary(){
    ifstream inputFile;
    string line;
    inputFile.open(fileName + "-dictionary.txt", ifstream :: in);
    while (inputFile.good()) {
        getline(inputFile, line);
        if (line.empty()){
            continue;
        }
        int termId = stoi(line.substr(0, line.find(' ')));
        string word = line.substr(line.find(' ') + 1);
        Term* term = new Term(word, termId);
        words.emplace_back(term);
    }
    inputFile.close();
    updateWordMap();
}

/**
 * Constructs the TermDictionary from a list of tokens (term occurrences). The terms array should be sorted
 * before calling this method. Constructs the distinct terms and their corresponding term ids.
 * @param terms Sorted list of tokens in the memory collection.
 */
TermDictionary::TermDictionary(const vector<TermOccurrence>& terms) : Dictionary(){
    int i, termId = 0;
    if (!terms.empty()){
        TermOccurrence term = terms[0];
        addTerm(term.getTerm().getName(), termId);
        termId++;
        TermOccurrence previousTerm = term;
        i = 1;
        while (i < terms.size()){
            term = terms[i];
            if (term.isDifferent(previousTerm)){
                addTerm(term.getTerm().getName(), termId);
                termId++;
            }
            i++;
            previousTerm = term;
        }
    }
    sort();
}

bool compareWord1(const Word* wordA, const Word* wordB)
{
    return wordA->getName() < wordB->getName();
}

/**
 * Constructs the TermDictionary from a hash set of tokens (strings). Constructs sorted dictinct terms array and
 * their corresponding term ids.
 * @param words Hash set of tokens in the memory collection.
 */
TermDictionary::TermDictionary(const set<string>& words) : Dictionary(){
    vector<Word*> wordList;
    for (const auto& word : words){
        wordList.emplace_back(new Word(word));
    }
    std::sort(wordList.begin(), wordList.end(), compareWord1);
    int termID = 0;
    for (Word* term : wordList){
        addTerm(term->getName(), termID);
        termID++;
    }
    sort();
}

/**
 * Adds a new term to the sorted words array. First the term is searched in the words array using binary search,
 * then the word is added into the correct place.
 * @param name Lemma of the term
 * @param termID Id of the term
 */
void TermDictionary::addTerm(const string& name, int termID) {
    auto middle = lower_bound(words.begin(), words.end(), new Word(name), compareWord1);
    words.insert(middle, new Term(name, termID));
}

/**
 * Saves the term dictionary into the dictionary file. Each line stores the term id and the term name separated via
 * space.
 * @param fileName Dictionary file name. Real dictionary file name is created by attaching -dictionary.txt to this
 *                 file name
 */
void TermDictionary::save(const string& fileName) const {
    ofstream outfile;
    outfile.open(fileName + "-dictionary.txt", ofstream :: out);
    for (auto & word : words) {
        Term* term = (Term*) word;
        outfile << term->getTermId() << " " << term->getName() << "\n";
    }
    outfile.close();
}

/**
 * Constructs all NGrams from a given word. For example, 3 grams for word "term" are "$te", "ter", "erm", "rm$".
 * @param word Word for which NGrams will b created.
 * @param termId Term id to add into the posting list.
 * @param k N in NGram.
 * @return An array of NGrams for a given word.
 */
vector<TermOccurrence> TermDictionary::constructNGrams(const string& word, int termId, int k){
    vector<TermOccurrence> nGrams;
    if (word.length() >= k - 1){
        for (int l = -1; l < Word::size(word) - k + 2; l++){
            string term;
            if (l == -1){
                term = "$" + Word::substring(word, 0, k - 1);
            } else {
                if (l == Word::size(word) - k + 1){
                    term = Word::substring(word, l, k - 1) + "$";
                } else {
                    term = Word::substring(word, l, k);
                }
            }
            nGrams.emplace_back(TermOccurrence(Word(term), termId, l));
        }
    }
    return nGrams;
}

/**
 * Constructs all NGrams for all words in the dictionary. For example, 3 grams for word "term" are "$te", "ter",
 * "erm", "rm$".
 * @param k N in NGram.
 * @return A sorted array of NGrams for all words in the dictionary.
 */
vector<TermOccurrence> TermDictionary::constructTermsFromDictionary(int k) const {
    termOccurrenceComparator termComparator = termOccurrenceComparator();
    vector<TermOccurrence> terms;
    for (int i = 0; i < size(); i++){
        string word = getWord(i)->getName();
        vector<TermOccurrence> termList = TermDictionary::constructNGrams(word, i, k);
        terms.insert(terms.end(), termList.begin(), termList.end());
    }
    std::sort(terms.begin(), terms.end(), termComparator);
    return terms;
}
