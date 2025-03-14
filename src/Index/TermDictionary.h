//
// Created by Olcay Taner YILDIZ on 24.07.2022.
//

#ifndef INFORMATIONRETRIEVAL_TERMDICTIONARY_H
#define INFORMATIONRETRIEVAL_TERMDICTIONARY_H


#include <set>
#include "Dictionary/Dictionary.h"
#include "TermOccurrence.h"

class TermDictionary : public Dictionary {
public:
    TermDictionary();
    TermDictionary(const string& fileName);
    TermDictionary(const vector<TermOccurrence>& terms);
    TermDictionary(const set<string>& words);
    void addTerm(const string& name, int termID);
    void save(const string& fileName) const;
    static vector<TermOccurrence> constructNGrams(const string& word, int termId, int k);
    vector<TermOccurrence> constructTermsFromDictionary(int k) const;
};


#endif //INFORMATIONRETRIEVAL_TERMDICTIONARY_H
