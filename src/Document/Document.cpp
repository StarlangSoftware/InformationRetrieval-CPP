//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#include "Document.h"
#include "../include/TurkishSplitter.h"

Document::Document(DocumentType documentType, string absoluteFileName, string fileName, int docId) {
    this->docId = docId;
    this->absoluteFileName = absoluteFileName;
    this->fileName = fileName;
    this->documentType = documentType;
}

DocumentText Document::loadDocument() {
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

int Document::getDocId() {
    return docId;
}

string Document::getFileName() {
    return fileName;
}

string Document::getAbsoluteFileName() {
    return absoluteFileName;
}

int Document::getSize() {
    return size;
}

void Document::setSize(int size) {
    this->size = size;
}

void Document::setCategoryHierarchy(const string& categoryHierarchy) {
    this->categoryHierarchy = CategoryHierarchy(categoryHierarchy);
}

CategoryHierarchy Document::getCategoryHierarchy() {
    return categoryHierarchy;
}
