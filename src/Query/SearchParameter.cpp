//
// Created by Olcay Taner YILDIZ on 15.10.2022.
//

#include "SearchParameter.h"

/**
 * Accessor for the retrieval type
 * @return Retrieval type.
 */
RetrievalType SearchParameter::getRetrievalType() const{
    return retrievalType;
}

/**
 * Setter for the retrievalType.
 * @param retrievalType New retrieval type
 */
void SearchParameter::setRetrievalType(RetrievalType _retrievalType) {
    this->retrievalType = _retrievalType;
}

/**
 * Accessor for the document weighting scheme in tf-idf search
 * @return Document weighting scheme in tf-idf search
 */
DocumentWeighting SearchParameter::getDocumentWeighting() const{
    return documentWeighting;
}

/**
 * Mutator for the documentWeighting scheme used in tf-idf search.
 * @param documentWeighting New document weighting scheme for tf-idf search.
 */
void SearchParameter::setDocumentWeighting(DocumentWeighting _documentWeighting) {
    this->documentWeighting = _documentWeighting;
}

/**
 * Accessor for the term weighting scheme in tf-idf search
 * @return Term weighting scheme in tf-idf search
 */
TermWeighting SearchParameter::getTermWeighting() const{
    return termWeighting;
}

/**
 * Mutator for the termWeighting scheme used in tf-idf search.
 * @param termWeighting New term weighting scheme for tf-idf search.
 */
void SearchParameter::setTermWeighting(TermWeighting _termWeighting) {
    this->termWeighting = _termWeighting;
}

/**
 * Accessor for the maximum number of documents retrieved.
 * @return The maximum number of documents retrieved.
 */
int SearchParameter::getDocumentsRetrieved() const{
    return documentsRetrieved;
}

/**
 * Mutator for the maximum number of documents retrieved.
 * @param documentsRetrieved New value for the maximum number of documents retrieved.
 */
void SearchParameter::setDocumentsRetrieved(int _documentsRetrieved) {
    this->documentsRetrieved = _documentsRetrieved;
}

/**
 * Accessor for the category determination type.
 * @return Category determination type.
 */
CategoryDeterminationType SearchParameter::getCategoryDeterminationType() const{
    return categoryDeterminationType;
}

/**
 * Mutator for the category determination type.
 * @param categoryDeterminationType New category determination type.
 */
void SearchParameter::setCategoryDeterminationType(CategoryDeterminationType _categoryDeterminationType) {
    this->categoryDeterminationType = _categoryDeterminationType;
}

/**
 * Accessor for the focus type.
 * @return Focus type.
 */
FocusType SearchParameter::getFocusType() const {
    return focusType;
}

/**
 * Mutator for the focus type.
 * @param focusType New focus type.
 */
void SearchParameter::setFocusType(FocusType _focusType) {
    this->focusType = _focusType;
}

/**
 * Accessor for the search attributes field. The parameter will determine if an attribute search is performed.
 * @return Search attribute.
 */
bool SearchParameter::getSearchAttributes() const{
    return searchAttributes;
}

/**
 * Mutator for the search attributes field. The parameter will determine if an attribute search is performed.
 * @param searchAttributes New value for search attribute.
 */
void SearchParameter::setSearchAttributes(bool _searchAttributes) {
    this->searchAttributes = _searchAttributes;
}
