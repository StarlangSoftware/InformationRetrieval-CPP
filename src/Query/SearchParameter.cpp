//
// Created by Olcay Taner YILDIZ on 15.10.2022.
//

#include "SearchParameter.h"

RetrievalType SearchParameter::getRetrievalType(){
    return retrievalType;
}

void SearchParameter::setRetrievalType(RetrievalType retrievalType) {
    this->retrievalType = retrievalType;
}

DocumentWeighting SearchParameter::getDocumentWeighting(){
    return documentWeighting;
}

void SearchParameter::setDocumentWeighting(DocumentWeighting documentWeighting) {
    this->documentWeighting = documentWeighting;
}

TermWeighting SearchParameter::getTermWeighting(){
    return termWeighting;
}

void SearchParameter::setTermWeighting(TermWeighting termWeighting) {
    this->termWeighting = termWeighting;
}

int SearchParameter::getDocumentsRetrieved(){
    return documentsRetrieved;
}

void SearchParameter::setDocumentsRetrieved(int documentsRetrieved) {
    this->documentsRetrieved = documentsRetrieved;
}
