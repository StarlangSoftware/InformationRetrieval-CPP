//
// Created by Olcay Taner YILDIZ on 24.07.2022.
//

#ifndef INFORMATIONRETRIEVAL_TERMDICTIONARY_H
#define INFORMATIONRETRIEVAL_TERMDICTIONARY_H


#include <set>
#include "Dictionary.h"
#include "TermOccurrence.h"

class TermDictionary : public Dictionary{
public:
    TermDictionary();
    explicit TermDictionary(Comparator comparator);
    TermDictionary(Comparator comparator, string fileName);
    TermDictionary(Comparator comparator, vector<TermOccurrence> terms);
    TermDictionary(Comparator comparator, set<string> words);
    void addTerm(string name, int termID);
    void save(string fileName);
    static vector<TermOccurrence> constructNGrams(string word, int termId, int k);
    vector<TermOccurrence> constructTermsFromDictionary(int k);
};


#endif //INFORMATIONRETRIEVAL_TERMDICTIONARY_H
