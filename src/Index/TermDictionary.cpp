//
// Created by Olcay Taner YILDIZ on 24.07.2022.
//

#include <fstream>
#include "TermDictionary.h"
#include "Term.h"
#include "TermOccurrenceComparator.h"

TermDictionary::TermDictionary(Comparator comparator) : Dictionary(comparator) {
}

TermDictionary::TermDictionary(Comparator comparator, const string& fileName) : Dictionary(comparator){
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
}

TermDictionary::TermDictionary(Comparator comparator, const vector<TermOccurrence>& terms) : Dictionary(comparator){
    int i, termId = 0;
    if (!terms.empty()){
        TermOccurrence term = terms[0];
        addTerm(term.getTerm().getName(), termId);
        termId++;
        TermOccurrence previousTerm = term;
        i = 1;
        while (i < terms.size()){
            term = terms[i];
            if (term.isDifferent(previousTerm, comparator)){
                addTerm(term.getTerm().getName(), termId);
                termId++;
            }
            i++;
            previousTerm = term;
        }
    }
}

TermDictionary::TermDictionary(Comparator comparator, const set<string>& words) : Dictionary(comparator){
    vector<Word*> wordList;
    for (const auto& word : words){
        wordList.emplace_back(new Word(word));
    }
    std::sort(wordList.begin(), wordList.end(), turkishWordComparator(turkishComparatorMap));
    int termID = 0;
    for (Word* term : wordList){
        addTerm(term->getName(), termID);
        termID++;
    }
}

void TermDictionary::addTerm(const string& name, int termID) {
    auto middle = lower_bound(words.begin(), words.end(), new Word(name), turkishWordComparator(turkishComparatorMap));
    words.insert(middle, new Term(name, termID));
}

void TermDictionary::save(const string& fileName) {
    ofstream outfile;
    outfile.open(fileName + "-dictionary.txt", ofstream :: out);
    for (auto & word : words) {
        Term* term = (Term*) word;
        outfile << term->getTermId() << " " << term->getName() << "\n";
    }
    outfile.close();
}

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

vector<TermOccurrence> TermDictionary::constructTermsFromDictionary(int k){
    termOccurrenceComparator termComparator = termOccurrenceComparator(turkishComparatorMap);
    vector<TermOccurrence> terms;
    for (int i = 0; i < size(); i++){
        string word = getWord(i)->getName();
        vector<TermOccurrence> termList = TermDictionary::constructNGrams(word, i, k);
        terms.insert(terms.end(), termList.begin(), termList.end());
    }
    std::sort(terms.begin(), terms.end(), termComparator);
    return terms;
}

TermDictionary::TermDictionary() : Dictionary(){

}
