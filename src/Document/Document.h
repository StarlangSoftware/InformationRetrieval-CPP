//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#ifndef INFORMATIONRETRIEVAL_DOCUMENT_H
#define INFORMATIONRETRIEVAL_DOCUMENT_H

#include <string>
#include "DocumentText.h"
#include "MorphologicalDisambiguator.h"
#include "FsmMorphologicalAnalyzer.h"
#include "CategoryHierarchy.h"
#include "DocumentType.h"

using namespace std;

class Document {
private:
    string absoluteFileName;
    string fileName;
    int docId;
    int size = 0;
    DocumentType documentType;
    CategoryHierarchy categoryHierarchy;
public:
    Document(DocumentType documentType, string absoluteFileName, string fileName, int docId);
    DocumentText loadDocument();
    int getDocId();
    string getFileName();
    string getAbsoluteFileName();
    int getSize();
    void setSize(int size);
    void setCategoryHierarchy(const string& categoryHierarchy);
    CategoryHierarchy getCategoryHierarchy();
};


#endif //INFORMATIONRETRIEVAL_DOCUMENT_H
