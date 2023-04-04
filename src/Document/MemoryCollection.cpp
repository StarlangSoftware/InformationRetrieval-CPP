//
// Created by Olcay Taner YILDIZ on 26.07.2022.
//

#include <iostream>
#include "MemoryCollection.h"
#include "../Index/TermOccurrenceComparator.h"
#include "../Index/Term.h"

using std::filesystem::directory_iterator;

MemoryCollection::MemoryCollection(const string& directory, const Parameter& parameter) : AbstractCollection(directory, parameter){
    indexType = parameter.getIndexType();
    if (parameter.loadIndexesFromFile()){
        loadIndexesFromFile(directory);
    } else {
        constructIndexesInMemory();
    }
    if (parameter.getDocumentType() == DocumentType::CATEGORICAL){
        positionalIndex.setCategoryCounts(documents);
        categoryTree->setRepresentativeCount(parameter.getRepresentationCount());
    }
}

void MemoryCollection::loadIndexesFromFile(const string &directory) {
    dictionary = TermDictionary(directory);
    invertedIndex = InvertedIndex(directory);
    if (parameter.constructPositionalIndex()){
        positionalIndex = PositionalIndex(directory);
        int* sizes = positionalIndex.getDocumentSizes(documents.size());
        for (Document& doc : documents){
            doc.setSize(sizes[doc.getDocId()]);
        }
        delete sizes;
    }
    if (parameter.constructPhraseIndex()){
        phraseDictionary = TermDictionary(directory + "-phrase");
        phraseIndex = InvertedIndex(directory + "-phrase");
        if (parameter.constructPositionalIndex()){
            phrasePositionalIndex = PositionalIndex(directory + "-phrase");
        }
    }
    if (parameter.constructNGramIndex()){
        biGramDictionary = TermDictionary(directory + "-biGram");
        triGramDictionary = TermDictionary(directory + "-triGram");
        biGramIndex = NGramIndex(directory + "-biGram");
        triGramIndex = NGramIndex(directory + "-triGram");
    }
}

void MemoryCollection::save() {
    if (indexType == IndexType::INVERTED_INDEX){
        dictionary.save(name);
        invertedIndex.save(name);
        if (parameter.constructPositionalIndex()){
            positionalIndex.save(name);
        }
        if (parameter.constructPhraseIndex()){
            phraseDictionary.save(name + "-phrase");
            phraseIndex.save(name + "-phrase");
            if (parameter.constructPositionalIndex()){
                phrasePositionalIndex.save(name + "-phrase");
            }
        }
        if (parameter.constructNGramIndex()){
            biGramDictionary.save(name + "-biGram");
            triGramDictionary.save(name + "-triGram");
            biGramIndex.save(name + "-biGram");
            triGramIndex.save(name + "-triGram");
        }
    }
    if (parameter.getDocumentType() == DocumentType::CATEGORICAL){
        saveCategories();
    }
}

void MemoryCollection::saveCategories() {
    ofstream outfile;
    outfile.open(name + "-categories.txt", ofstream::out);
    for (const Document& document : documents){
        outfile << document.getDocId() << "\t" << document.getCategory()->to_string() << "\n";
    }
    outfile.close();
}

void MemoryCollection::constructIndexesInMemory() {
    vector<TermOccurrence> terms = constructTerms(TermType::TOKEN);
    dictionary = TermDictionary(terms);
    switch (indexType){
        case IndexType::INCIDENCE_MATRIX:
            incidenceMatrix = IncidenceMatrix(terms, dictionary, documents.size());
            break;
        case IndexType::INVERTED_INDEX:
            invertedIndex = InvertedIndex(dictionary, terms);
            if (parameter.constructPositionalIndex()){
                positionalIndex = PositionalIndex(dictionary, terms);
            }
            if (parameter.constructPhraseIndex()){
                terms = constructTerms(TermType::PHRASE);
                phraseDictionary = TermDictionary(terms);
                phraseIndex = InvertedIndex(phraseDictionary, terms);
                if (parameter.constructPositionalIndex()){
                    phrasePositionalIndex = PositionalIndex(phraseDictionary, terms);
                }
            }
            if (parameter.constructNGramIndex()){
                constructNGramIndex();
            }
            if (parameter.getDocumentType() == DocumentType::CATEGORICAL){
                categoryTree = new CategoryTree(name);
                for (Document& document : documents){
                    document.loadCategory(categoryTree);
                }
            }
            break;
    }
}

vector<TermOccurrence> MemoryCollection::constructTerms(TermType termType) const{
    vector<TermOccurrence> terms;
    vector<TermOccurrence> docTerms;
    for (Document doc : documents){
        DocumentText documentText = doc.loadDocument();
        docTerms = documentText.constructTermList(doc.getDocId(), termType);
        terms.insert(terms.end(), docTerms.begin(), docTerms.end());
    }
    std::sort(terms.begin(), terms.end(),  termOccurrenceComparator());
    return terms;
}

QueryResult MemoryCollection::attributeSearch(Query& query) {
    Query termAttributes = Query();
    Query phraseAttributes = Query();
    QueryResult termResult = QueryResult();
    QueryResult phraseResult = QueryResult();
    query.filterAttributes(attributeList, termAttributes, phraseAttributes);
    if (termAttributes.size() > 0){
        termResult = invertedIndex.search(termAttributes, dictionary);
    }
    if (phraseAttributes.size() > 0){
        phraseResult = phraseIndex.search(phraseAttributes, phraseDictionary);
    }
    if (termAttributes.size() == 0){
        return phraseResult;
    }
    if (phraseAttributes.size() == 0){
        return termResult;
    }
    return termResult.intersection(phraseResult);
}

QueryResult MemoryCollection::searchWithInvertedIndex(Query &query, const SearchParameter &parameter) {
    switch (parameter.getRetrievalType()){
        case    RetrievalType::BOOLEAN:
            return invertedIndex.search(query, dictionary);
        case RetrievalType::POSITIONAL:
            return positionalIndex.positionalSearch(query, dictionary);
        case RetrievalType::RANKED:
            return positionalIndex.rankedSearch(query,
                                                dictionary,
                                                documents,
                                                parameter.getTermWeighting(),
                                                parameter.getDocumentWeighting(),
                                                parameter.getDocumentsRetrieved());
        case RetrievalType::ATTRIBUTE:
            return attributeSearch(query);
    }
    return QueryResult();
}

QueryResult MemoryCollection::filterAccordingToCategories(const QueryResult &currentResult, const vector<CategoryNode*>& categories) {
    QueryResult filteredResult = QueryResult();
    vector<QueryResultItem> items = currentResult.getItems();
    for (QueryResultItem queryResultItem : items) {
        CategoryNode* categoryNode = documents[queryResultItem.getDocId()].getCategory();
        for (CategoryNode* possibleAncestor : categories){
            if (categoryNode->isDescendant(possibleAncestor)) {
                filteredResult.add(queryResultItem.getDocId(), queryResultItem.getScore());
                break;
            }
        }
    }
    return filteredResult;
}

QueryResult MemoryCollection::searchCollection(Query& query, const SearchParameter& searchParameter) {
    if (searchParameter.getFocusType() == FocusType::CATEGORY){
        QueryResult currentResult = searchWithInvertedIndex(query, searchParameter);
        vector<CategoryNode*> categories = categoryTree->getCategories(query, &dictionary, searchParameter.getCategoryDeterminationType());
        return filterAccordingToCategories(currentResult, categories);
    } else {
        switch (indexType){
            case IndexType::INCIDENCE_MATRIX:
                return incidenceMatrix.search(query, dictionary);
            case   IndexType::INVERTED_INDEX:
                return searchWithInvertedIndex(query, searchParameter);
        }
    }
    return QueryResult();
}

vector<string> MemoryCollection::autoCompleteWord(const string &prefix) {
    vector<string> result;
    int i = dictionary.getWordStartingWith(prefix);
    if (i < 0){
        i = -(i + 1);
    }
    while (i < dictionary.size()){
        if (dictionary.getWord(i)->getName().starts_with(prefix)){
            result.emplace_back(dictionary.getWord(i)->getName());
        } else {
            break;
        }
        i++;
    }
    invertedIndex.autoCompleteWord(result, dictionary);
    return result;
}
