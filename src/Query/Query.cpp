//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#include <regex>
#include "Query.h"
#include <StringUtils.h>

const vector<string> Query::shortcuts = {"cc", "cm2", "cm", "gb", "ghz", "gr", "gram", "hz", "inc", "inch",
                                                      "inç", "kg", "kw", "kva", "litre", "lt", "m2", "m3", "mah", "mb",
                                                      "metre", "mg", "mhz", "ml", "mm", "mp", "ms", "mt", "mv", "tb",
                                                      "tl", "va", "volt", "watt", "ah", "hp", "oz", "rpm", "dpi", "ppm",
                                                      "ohm", "kwh", "kcal", "kbit", "mbit", "gbit", "bit", "byte", "mbps",
                                                      "gbps", "cm3", "mm2", "mm3", "khz", "ft", "db", "sn", "g", "v", "m", "l", "w", "s"};

/**
 * Constructor of the Query class. Initializes the terms array.
 */
Query::Query() = default;

/**
 * Another constructor of the Query class. Splits the query into multiple words and put them into the terms array.
 * @param query Query string
 */
Query::Query(const string& query) {
    vector<string> _terms = StringUtils::split(query);
    for (const string& term : _terms){
        this->terms.emplace_back(Word(term));
    }
}

/**
 * Accessor for the terms array. Returns the term at position index.
 * @param index Position of the term in the terms array.
 * @return The term at position index.
 */
Word Query::getTerm(int index) const{
    return terms[index];
}

/**
 * Returns the size of the query, i.e. number of words in the query.
 * @return Size of the query, i.e. number of words in the query.
 */
int Query::size() const{
    return terms.size();
}

/**
 * Filters the origilan query by removing phrase attributes, shortcuts and single word attributes.
 * @param attributeList Hash set containing all attributes (phrase and single word)
 * @param termAttributes New query that will accumulate single word attributes from the original query.
 * @param phraseAttributes New query that will accumulate phrase attributes from the original query.
 * @return Filtered query after removing single word and phrase attributes from the original query.
 */
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
