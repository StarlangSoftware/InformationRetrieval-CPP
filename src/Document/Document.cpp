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
                categoryHierarchy = CategoryHierarchy(corpus.getSentence(0)->to_string());
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

void Document::setCategoryHierarchy(const string& _categoryHierarchy) {
    this->categoryHierarchy = CategoryHierarchy(_categoryHierarchy);
}

CategoryHierarchy Document::getCategoryHierarchy() const{
    return categoryHierarchy;
}
