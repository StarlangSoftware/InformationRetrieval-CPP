//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#ifndef INFORMATIONRETRIEVAL_QUERY_H
#define INFORMATIONRETRIEVAL_QUERY_H

#include "unordered_set"
#include <vector>
#include "Dictionary/Word.h"

class Query {
private:
    vector<Word> terms;
public:
    Query();
    explicit Query(const string& query);
    Word getTerm(int index) const;
    int size() const;
    Query filterAttributes(const unordered_set<string>& attributeList, Query& termAttributes, Query& phraseAttributes);
};


#endif //INFORMATIONRETRIEVAL_QUERY_H
