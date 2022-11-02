//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#ifndef INFORMATIONRETRIEVAL_DOCUMENT_H
#define INFORMATIONRETRIEVAL_DOCUMENT_H

#include <string>
#include "DocumentText.h"
#include "MorphologicalDisambiguator.h"
#include "FsmMorphologicalAnalyzer.h"
#include "DocumentType.h"
#include "../Index/CategoryNode.h"
#include "../Index/CategoryTree.h"

using namespace std;

class Document {
private:
    string absoluteFileName;
    string fileName;
    int docId;
    int size = 0;
    DocumentType documentType;
    CategoryNode* category;
public:
    Document(DocumentType documentType, const string& absoluteFileName, const string& fileName, int docId);
    DocumentText loadDocument();
    int getDocId() const;
    string getFileName() const;
    string getAbsoluteFileName() const;
    int getSize() const;
    void setSize(int _size);
    CategoryNode* getCategory() const;
    void setCategory(CategoryTree* categoryTree, const string& _category);
    void loadCategory(CategoryTree* categoryTree);
};


#endif //INFORMATIONRETRIEVAL_DOCUMENT_H
