//
// Created by Olcay Taner YILDIZ on 26.07.2022.
//

#include <iostream>
#include "MemoryCollection.h"
#include "../Index/TermOccurrenceComparator.h"
#include "../Index/Term.h"

using std::filesystem::directory_iterator;

/**
 * Constructor for the MemoryCollection class. In small collections, dictionary and indexes are kept in memory.
 * Memory collection also supports categorical documents.
 * @param directory Directory where the document collection resides.
 * @param parameter Search parameter
 */
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

/**
 * The method loads the term dictionary, inverted index, positional index, phrase and N-Gram indexes from dictionary
 * and index files to the memory.
 * @param directory Directory where the document collection resides.
 */
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

/**
 * The method saves the term dictionary, inverted index, positional index, phrase and N-Gram indexes to the dictionary
 * and index files. If the collection is a categorical collection, categories are also saved to the category
 * files.
 */
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

/**
 * The method saves the category tree for the categorical collections.
 */
void MemoryCollection::saveCategories() const {
    ofstream outfile;
    outfile.open(name + "-categories.txt", ofstream::out);
    for (const Document& document : documents){
        outfile << document.getDocId() << "\t" << document.getCategory()->to_string() << "\n";
    }
    outfile.close();
}

/**
 * The method constructs the term dictionary, inverted index, positional index, phrase and N-Gram indexes in memory.
 */
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

/**
 * Given the document collection, creates an array list of terms. If term type is TOKEN, the terms are single
 * word, if the term type is PHRASE, the terms are bi-words. Each document is loaded into memory and
 * word list is created. Since the dictionary can be kept in memory, all operations can be done in memory.
 * @param termType If term type is TOKEN, the terms are single word, if the term type is PHRASE, the terms are
 *                 bi-words.
 * @return Array list of terms occurring in the document collection.
 */
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

/**
 * The method searches given query string in the document collection using the attribute list according to the
 * given search parameter. First, the original query is filtered by removing phrase attributes, shortcuts and single
 * word attributes. At this stage, we get the word and phrase attributes in the original query and the remaining
 * words in the original query as two separate queries. Second, both single word and phrase attributes in the
 * original query are searched in the document collection. Third, these intermediate query results are then
 * intersected. Fourth, we put this results into either (i) an inverted index (ii) or a ranked based positional
 * filtering with the filtered query to get the end result.
 * @param query Query string
 * @param parameter Search parameter for the query
 * @return The intermediate result of the query obtained by doing attribute list based search in the collection.
 */
QueryResult MemoryCollection::attributeSearch(Query& query, const SearchParameter& parameter) {
    Query termAttributes = Query();
    Query phraseAttributes = Query();
    QueryResult termResult = QueryResult();
    QueryResult phraseResult = QueryResult();
    QueryResult attributeResult, filteredResult;
    Query filteredQuery = query.filterAttributes(attributeList, termAttributes, phraseAttributes);
    if (termAttributes.size() > 0){
        termResult = invertedIndex.search(termAttributes, dictionary);
    }
    if (phraseAttributes.size() > 0){
        phraseResult = phraseIndex.search(phraseAttributes, phraseDictionary);
    }
    if (termAttributes.size() == 0){
        attributeResult = phraseResult;
    } else {
        if (phraseAttributes.size() == 0){
            attributeResult = termResult;
        } else {
            attributeResult = termResult.intersectionFastSearch(phraseResult);
        }
    }
    if (filteredQuery.size() == 0){
        return attributeResult;
    } else {
        if (parameter.getRetrievalType() != RetrievalType::RANKED){
            filteredResult = searchWithInvertedIndex(filteredQuery, parameter);
            return filteredResult.intersectionFastSearch(attributeResult);
        } else {
            filteredResult = positionalIndex.rankedSearch(filteredQuery,
                                                          dictionary,
                                                          documents,
                                                          parameter);
            if (attributeResult.size() < 10){
                filteredResult = filteredResult.intersectionLinearSearch(attributeResult);
            } else {
                filteredResult = filteredResult.intersectionBinarySearch(attributeResult);
            }
            filteredResult.getBest(parameter.getDocumentsRetrieved());
            return filteredResult;
        }
    }
}

/**
 * The method searches given query string in the document collection using the inverted index according to the
 * given search parameter. If the search is (i) boolean, inverted index is used (ii) positional, positional
 * inverted index is used, (iii) ranked, positional inverted index is used with a ranking algorithm at the end.
 * @param query Query string
 * @param parameter Search parameter for the query
 * @return The intermediate result of the query obtained by doing inverted index based search in the collection.
 */
QueryResult MemoryCollection::searchWithInvertedIndex(const Query &query, const SearchParameter &parameter) {
    switch (parameter.getRetrievalType()){
        case    RetrievalType::BOOLEAN:
            return invertedIndex.search(query, dictionary);
        case RetrievalType::POSITIONAL:
            return positionalIndex.positionalSearch(query, dictionary);
        case RetrievalType::RANKED:
            QueryResult result = positionalIndex.rankedSearch(query,
                                                              dictionary,
                                                              documents,
                                                              parameter);
            result.getBest(parameter.getDocumentsRetrieved());
            return result;
    }
    return QueryResult();
}

/**
 * Filters current search result according to the predicted categories from the query string. For every search
 * result, if it is in one of the predicated categories, is added to the filtered end result. Otherwise, it is
 * omitted in the end result.
 * @param currentResult Current search result before filtering.
 * @param categories Predicted categories that match the query string.
 * @return Filtered query result
 */
QueryResult MemoryCollection::filterAccordingToCategories(const QueryResult &currentResult, const vector<CategoryNode*>& categories) const {
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

/**
 * Searches a document collection for a given query according to the given search parameters. The documents are
 * searched using (i) incidence matrix if the index type is incidence matrix, (ii) attribute list if search
 * attributes option is selected, (iii) inverted index if the index type is inverted index and no attribute
 * search is done. After the initial search, if there is a categorical focus, it filters the results
 * according to the predicted categories from the query string.
 * @param query Query string
 * @param searchParameter Search parameter for the query
 * @return The result of the query obtained by doing search in the collection.
 */
QueryResult MemoryCollection::searchCollection(Query& query, const SearchParameter& searchParameter) {
    QueryResult currentResult;
    if (searchParameter.getFocusType() == FocusType::CATEGORY){
        if (searchParameter.getSearchAttributes()){
            currentResult = attributeSearch(query, searchParameter);
        } else {
            currentResult = searchWithInvertedIndex(query, searchParameter);
        }
        vector<CategoryNode*> categories = categoryTree->getCategories(query, &dictionary, searchParameter.getCategoryDeterminationType());
        return filterAccordingToCategories(currentResult, categories);
    } else {
        switch (indexType){
            case IndexType::INCIDENCE_MATRIX:
                return incidenceMatrix.search(query, dictionary);
            case   IndexType::INVERTED_INDEX:
                if (searchParameter.getSearchAttributes()){
                    return attributeSearch(query, searchParameter);
                } else {
                    return searchWithInvertedIndex(query, searchParameter);
                }
        }
    }
    return QueryResult();
}

/**
 * Constructs an auto complete list of product names for a given prefix. THe results are sorted according to
 * frequencies.
 * @param prefix Prefix of the name of the product.
 * @return An auto complete list of product names for a given prefix.
 */
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
