//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#include "Document.h"
#include "../include/TurkishSplitter.h"

/**
 * Constructor for the Document class. Sets the attributes.
 * @param documentType Type of the document. Can be normal for normal documents, categorical for categorical
 *                     documents.
 * @param absoluteFileName Absolute file name of the document
 * @param fileName Relative file name of the document.
 * @param docId Id of the document
 */
Document::Document(DocumentType documentType, const string& absoluteFileName, const string& fileName, int docId) {
    this->docId = docId;
    this->absoluteFileName = absoluteFileName;
    this->fileName = fileName;
    this->documentType = documentType;
}

/**
 * Loads the document from input stream. For normal documents, it reads as a corpus. For categorical documents, the
 * first line contains categorical information, second line contains name of the product, third line contains
 * detailed info about the product.
 * @return Loaded document text.
 */
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

/**
 * Loads the category of the document and adds it to the category tree. Category information is stored in the first
 * line of the document.
 * @param categoryTree Category tree to which new product will be added.
 */
void Document::loadCategory(CategoryTree* categoryTree) {
    if (documentType == DocumentType::CATEGORICAL){
        Corpus corpus = Corpus(absoluteFileName);
        if (corpus.sentenceCount() >= 2) {
            category = categoryTree->addCategoryHierarchy(corpus.getSentence(0)->to_string());
        }
    }
}

/**
 * Accessor for the docId attribute.
 * @return docId attribute.
 */
int Document::getDocId() const{
    return docId;
}

/**
 * Accessor for the fileName attribute.
 * @return fileName attribute.
 */
string Document::getFileName() const{
    return fileName;
}

/**
 * Accessor for the absoluteFileName attribute.
 * @return absoluteFileName attribute.
 */
string Document::getAbsoluteFileName() const{
    return absoluteFileName;
}

/**
 * Accessor for the size attribute.
 * @return size attribute.
 */
int Document::getSize() const{
    return size;
}

/**
 * Mutator for the size attribute.
 * @param size New size attribute.
 */
void Document::setSize(int _size) {
    this->size = _size;
}

/**
 * Accessor for the category attribute.
 * @return Category attribute as a String
 */
CategoryNode* Document::getCategory() const{
    return category;
}

/**
 * Mutator for the category attribute.
 * @param categoryTree Category tree to which new category will be added.
 * @param category New category that will be added
 */
void Document::setCategory(CategoryTree* categoryTree, const string& _category) {
    category = categoryTree->addCategoryHierarchy(_category);
}
