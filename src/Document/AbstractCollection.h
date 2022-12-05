//
// Created by Olcay Taner YILDIZ on 3.12.2022.
//

#ifndef INFORMATIONRETRIEVAL_ABSTRACTCOLLECTION_H
#define INFORMATIONRETRIEVAL_ABSTRACTCOLLECTION_H


#include "../Index/TermDictionary.h"
#include "../Index/IncidenceMatrix.h"
#include "../Index/InvertedIndex.h"
#include "../Index/NGramIndex.h"
#include "../Index/PositionalIndex.h"
#include "Parameter.h"

class AbstractCollection {
protected:
    TermDictionary dictionary;
    TermDictionary phraseDictionary;
    TermDictionary biGramDictionary;
    TermDictionary triGramDictionary;
    vector<Document> documents;
    IncidenceMatrix incidenceMatrix;
    InvertedIndex invertedIndex;
    NGramIndex biGramIndex;
    NGramIndex triGramIndex;
    PositionalIndex positionalIndex;
    InvertedIndex phraseIndex;
    PositionalIndex phrasePositionalIndex;
    string name;
    Parameter parameter;
    CategoryTree* categoryTree = nullptr;
    unordered_set<string> attributeList;
    void constructNGramIndex();
public:
    AbstractCollection(const string& directory, const Parameter& parameter);
    int size() const;
    int vocabularySize() const;
private:
    void loadCategories();
    void loadAttributeList();
};


#endif //INFORMATIONRETRIEVAL_ABSTRACTCOLLECTION_H
