//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#include <regex>
#include "Query.h"

const vector<string> Query::shortcuts = {"cc", "cm2", "cm", "gb", "ghz", "gr", "gram", "hz", "inc", "inch",
                                                      "inç", "kg", "kw", "kva", "litre", "lt", "m2", "m3", "mah", "mb",
                                                      "metre", "mg", "mhz", "ml", "mm", "mp", "ms", "mt", "mv", "tb",
                                                      "tl", "va", "volt", "watt", "ah", "hp", "oz", "rpm", "dpi", "ppm",
                                                      "ohm", "kwh", "kcal", "kbit", "mbit", "gbit", "bit", "byte", "mbps",
                                                      "gbps", "cm3", "mm2", "mm3", "khz", "ft", "db", "sn", "g", "v", "m", "l", "w", "s"};

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

Query Query::filterAttributes(const unordered_set<string>& attributeList, Query &termAttributes,
                             Query &phraseAttributes) {
    int i = 0;
    Query filteredQuery = Query();
    while (i < this->terms.size()){
        if (i < this->terms.size() - 1){
            string pair = this->terms[i].getName() + " " + this->terms[i + 1].getName();
            if (attributeList.contains(pair)){
                phraseAttributes.terms.emplace_back(pair);
                i += 2;
                continue;
            }
            if (std::find(shortcuts.begin(), shortcuts.end(), this->terms[i + 1].getName()) != shortcuts.end() &&
                    (regex_match(this->terms[i].getName(), regex("[-+]?\\d+")) || regex_match(this->terms[i].getName(), regex(R"((\+|-)?(\d+)?\.\d*)")))){
                phraseAttributes.terms.emplace_back(pair);
                i += 2;
                continue;
            }
        }
        if (attributeList.contains(this->terms[i].getName())){
            termAttributes.terms.emplace_back(this->terms[i]);
        } else {
            filteredQuery.terms.emplace_back(this->terms[i]);
        }
        i++;
    }
    return filteredQuery;
}

Query::Query() = default;
