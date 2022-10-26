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
