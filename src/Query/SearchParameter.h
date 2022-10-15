//
// Created by Olcay Taner YILDIZ on 15.10.2022.
//

#ifndef INFORMATIONRETRIEVAL_SEARCHPARAMETER_H
#define INFORMATIONRETRIEVAL_SEARCHPARAMETER_H


#include "RetrievalType.h"
#include "../Document/DocumentWeighting.h"
#include "../Index/TermWeighting.h"

class SearchParameter {
private:
    RetrievalType retrievalType = RetrievalType::RANKED;
    DocumentWeighting documentWeighting = DocumentWeighting::NO_IDF;
    TermWeighting termWeighting = TermWeighting::NATURAL;
    int documentsRetrieved = 1;
public:
    RetrievalType getRetrievalType();
    void setRetrievalType(RetrievalType retrievalType);
    DocumentWeighting getDocumentWeighting();
    void setDocumentWeighting(DocumentWeighting documentWeighting);
    TermWeighting getTermWeighting();
    void setTermWeighting(TermWeighting termWeighting);
    int getDocumentsRetrieved();
    void setDocumentsRetrieved(int documentsRetrieved);
};


#endif //INFORMATIONRETRIEVAL_SEARCHPARAMETER_H
