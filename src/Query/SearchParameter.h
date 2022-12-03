//
// Created by Olcay Taner YILDIZ on 15.10.2022.
//

#ifndef INFORMATIONRETRIEVAL_SEARCHPARAMETER_H
#define INFORMATIONRETRIEVAL_SEARCHPARAMETER_H


#include "RetrievalType.h"
#include "../Document/DocumentWeighting.h"
#include "../Index/TermWeighting.h"
#include "CategoryDeterminationType.h"
#include "FocusType.h"

class SearchParameter {
private:
    RetrievalType retrievalType = RetrievalType::RANKED;
    DocumentWeighting documentWeighting = DocumentWeighting::NO_IDF;
    TermWeighting termWeighting = TermWeighting::NATURAL;
    int documentsRetrieved = 1;
    CategoryDeterminationType categoryDeterminationType = CategoryDeterminationType::KEYWORD;
    FocusType focusType = FocusType::OVERALL;
public:
    RetrievalType getRetrievalType() const;
    void setRetrievalType(RetrievalType _retrievalType);
    DocumentWeighting getDocumentWeighting() const;
    void setDocumentWeighting(DocumentWeighting _documentWeighting);
    TermWeighting getTermWeighting() const;
    void setTermWeighting(TermWeighting _termWeighting);
    int getDocumentsRetrieved() const;
    void setDocumentsRetrieved(int _documentsRetrieved);
    CategoryDeterminationType getCategoryDeterminationType() const;
    void setCategoryDeterminationType(CategoryDeterminationType _categoryDeterminationType);
    FocusType getFocusType() const;
    void setFocusType(FocusType _focusType);
};


#endif //INFORMATIONRETRIEVAL_SEARCHPARAMETER_H
