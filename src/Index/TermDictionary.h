//
// Created by Olcay Taner YILDIZ on 24.07.2022.
//

#ifndef INFORMATIONRETRIEVAL_TERMDICTIONARY_H
#define INFORMATIONRETRIEVAL_TERMDICTIONARY_H


#include <set>
#include "Dictionary/Dictionary.h"
#include "TermOccurrence.h"

class TermDictionary : public Dictionary{
public:
    TermDictionary();
    explicit TermDictionary(Comparator comparator);
    TermDictionary(Comparator comparator, const string& fileName);
    TermDictionary(Comparator comparator, const vector<TermOccurrence>& terms);
    TermDictionary(Comparator comparator, const set<string>& words);
    void addTerm(const string& name, int termID);
    void save(const string& fileName);
    static vector<TermOccurrence> constructNGrams(const string& word, int termId, int k);
    vector<TermOccurrence> constructTermsFromDictionary(int k);
};


#endif //INFORMATIONRETRIEVAL_TERMDICTIONARY_H
