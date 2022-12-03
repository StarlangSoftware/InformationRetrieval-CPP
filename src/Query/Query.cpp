//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#include "Query.h"

Query::Query(const string& query) {
    vector<string> _terms = Word::split(query);
    for (const string& term : _terms){
        this->terms.emplace_back(Word(term));
    }
}

Word Query::getTerm(int index) const{
    return terms[index];
}

int Query::size() const{
    return terms.size();
}

void Query::filterAttributes(const unordered_set<string>& attributeList, Query &termAttributes,
                             Query &phraseAttributes) {
    int i = 0;
    while (i < this->terms.size()){
        if (i < this->terms.size() - 1){
            string pair = this->terms[i].getName() + " " + this->terms[i + 1].getName();
            if (attributeList.contains(pair)){
                phraseAttributes.terms.emplace_back(pair);
                i += 2;
                continue;
            }
        }
        if (attributeList.contains(this->terms[i].getName())){
            termAttributes.terms.emplace_back(this->terms[i]);
        }
        i++;
    }
}

Query::Query() = default;
