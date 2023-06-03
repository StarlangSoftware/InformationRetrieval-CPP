//
// Created by Olcay Taner YILDIZ on 15.10.2022.
//

#include "SearchParameter.h"

RetrievalType SearchParameter::getRetrievalType() const{
    return retrievalType;
}

void SearchParameter::setRetrievalType(RetrievalType _retrievalType) {
    this->retrievalType = _retrievalType;
}

DocumentWeighting SearchParameter::getDocumentWeighting() const{
    return documentWeighting;
}

void SearchParameter::setDocumentWeighting(DocumentWeighting _documentWeighting) {
    this->documentWeighting = _documentWeighting;
}

TermWeighting SearchParameter::getTermWeighting() const{
    return termWeighting;
}

void SearchParameter::setTermWeighting(TermWeighting _termWeighting) {
    this->termWeighting = _termWeighting;
}

int SearchParameter::getDocumentsRetrieved() const{
    return documentsRetrieved;
}

void SearchParameter::setDocumentsRetrieved(int _documentsRetrieved) {
    this->documentsRetrieved = _documentsRetrieved;
}

CategoryDeterminationType SearchParameter::getCategoryDeterminationType() const{
    return categoryDeterminationType;
}

void SearchParameter::setCategoryDeterminationType(CategoryDeterminationType _categoryDeterminationType) {
    this->categoryDeterminationType = _categoryDeterminationType;
}

FocusType SearchParameter::getFocusType() const {
    return focusType;
}

void SearchParameter::setFocusType(FocusType _focusType) {
    this->focusType = _focusType;
}

bool SearchParameter::getSearchAttributes() const{
    return searchAttributes;
}

void SearchParameter::setSearchAttributes(bool _searchAttributes) {
    this->searchAttributes = _searchAttributes;
}
