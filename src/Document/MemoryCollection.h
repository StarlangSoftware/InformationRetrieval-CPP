//
// Created by Olcay Taner YILDIZ on 26.07.2022.
//

#ifndef INFORMATIONRETRIEVAL_MEMORYCOLLECTION_H
#define INFORMATIONRETRIEVAL_MEMORYCOLLECTION_H


#include "IndexType.h"
#include "AbstractCollection.h"
#include "../Index/TermOccurrence.h"
#include "../Query/SearchParameter.h"

class MemoryCollection : public AbstractCollection{
private:
    IndexType indexType;
    void constructIndexesInMemory();
    vector<TermOccurrence> constructTerms(TermType termType) const;
    void saveCategories() const;
    QueryResult attributeSearch(Query& query, const SearchParameter& parameter);
    QueryResult searchWithInvertedIndex(const Query& query, const SearchParameter& parameter);
    QueryResult filterAccordingToCategories(const QueryResult& currentResult, const vector<CategoryNode*>& categories) const;
protected:
    void loadIndexesFromFile(const string& directory);
public:
    MemoryCollection(const string& directory, const Parameter& parameter);
    void save();
    vector<string> autoCompleteWord(const string& prefix);
    QueryResult searchCollection(Query& query, const SearchParameter& searchParameter);
};


#endif //INFORMATIONRETRIEVAL_MEMORYCOLLECTION_H
