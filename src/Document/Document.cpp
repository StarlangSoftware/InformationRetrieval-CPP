//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#include "Document.h"
#include "../include/TurkishSplitter.h"

Document::Document(DocumentType documentType, const string& absoluteFileName, const string& fileName, int docId) {
    this->docId = docId;
    this->absoluteFileName = absoluteFileName;
    this->fileName = fileName;
    this->documentType = documentType;
}

DocumentText Document::loadDocument(){
    DocumentText documentText;
    switch (documentType){
        case DocumentType::NORMAL:
        default:
            documentText = DocumentText(absoluteFileName, new TurkishSplitter());
            this->size = documentText.numberOfWords();
            break;
        case DocumentType::CATEGORICAL:
            Corpus corpus = Corpus(absoluteFileName);
            if (corpus.sentenceCount() >= 2){
                documentText = DocumentText();
                vector<Sentence*> sentences = TurkishSplitter().split(corpus.getSentence(1)->to_string());
                for (Sentence* sentence : sentences){
                    documentText.addSentence(sentence);
                }
                size = documentText.numberOfWords();
            }
            break;
    }
    return documentText;
}


void Document::loadCategory(CategoryTree* categoryTree) {
    if (documentType == DocumentType::CATEGORICAL){
        Corpus corpus = Corpus(absoluteFileName);
        if (corpus.sentenceCount() >= 2) {
            category = categoryTree->addCategoryHierarchy(corpus.getSentence(0)->to_string());
        }
    }
}

int Document::getDocId() const{
    return docId;
}

string Document::getFileName() const{
    return fileName;
}

string Document::getAbsoluteFileName() const{
    return absoluteFileName;
}

int Document::getSize() const{
    return size;
}

void Document::setSize(int _size) {
    this->size = _size;
}

CategoryNode* Document::getCategory() const{
    return category;
}

void Document::setCategory(CategoryTree* categoryTree, const string& _category) {
    category = categoryTree->addCategoryHierarchy(_category);
}
