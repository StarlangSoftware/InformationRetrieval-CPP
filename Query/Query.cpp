//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#include "Query.h"

Query::Query(string query) {
    vector<string> terms = Word::split(query);
    for (string term : terms){
        this->terms.emplace_back(Word(term));
    }
}

Word Query::getTerm(int index) {
    return terms[index];
}

int Query::size() {
    return terms.size();
}
