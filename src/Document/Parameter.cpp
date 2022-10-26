//
// Created by Olcay Taner YILDIZ on 21.07.2022.
//

#include "Parameter.h"

IndexType Parameter::getIndexType() const{
    return indexType;
}

Comparator Parameter::getWordComparator() const{
    return wordComparator;
}

bool Parameter::loadIndexesFromFile() const{
    return indexesFromFile;
}

MorphologicalDisambiguator* Parameter::getDisambiguator() const{
    return disambiguator;
}

FsmMorphologicalAnalyzer* Parameter::getFsm() const{
    return fsm;
}

bool Parameter::constructPhraseIndex() const{
    return phraseIndex;
}

bool Parameter::normalizeDocument() const{
    return _normalizeDocument;
}

bool Parameter::constructPositionalIndex() const{
    return positionalIndex;
}

bool Parameter::limitNumberOfDocumentsLoaded() const{
    return _limitNumberOfDocumentsLoaded;
}

int Parameter::getDocumentLimit() const{
    return documentLimit;
}

bool Parameter::constructDictionaryInDisk() const{
    return dictionaryInDisk;
}

bool Parameter::constructNGramIndex() const{
    return nGramIndex;
}

bool Parameter::constructIndexInDisk() const{
    return indexInDisk;
}

int Parameter::getWordLimit() const{
    return wordLimit;
}

void Parameter::setIndexType(IndexType _indexType) {
    this->indexType = _indexType;
}

void Parameter::setWordComparator(Comparator _wordComparator) {
    this->wordComparator = _wordComparator;
}

void Parameter::setLoadIndexesFromFile(bool loadIndexesFromFile) {
    this->indexesFromFile = loadIndexesFromFile;
}

void Parameter::setDisambiguator(MorphologicalDisambiguator* _disambiguator) {
    this->disambiguator = _disambiguator;
}

void Parameter::setFsm(FsmMorphologicalAnalyzer* _fsm) {
    this->fsm = _fsm;
}

void Parameter::setNormalizeDocument(bool normalizeDocument) {
    this->_normalizeDocument = normalizeDocument;
}

void Parameter::setPhraseIndex(bool _phraseIndex) {
    this->phraseIndex = _phraseIndex;
}

void Parameter::setPositionalIndex(bool _positionalIndex) {
    this->positionalIndex = _positionalIndex;
}

void Parameter::setNGramIndex(bool _nGramIndex) {
    this->nGramIndex = _nGramIndex;
}

void Parameter::setConstructIndexInDisk(bool constructIndexInDisk) {
    this->indexInDisk = constructIndexInDisk;
}

void Parameter::setLimitNumberOfDocumentsLoaded(bool limitNumberOfDocumentsLoaded) {
    this->_limitNumberOfDocumentsLoaded = limitNumberOfDocumentsLoaded;
}

void Parameter::setDocumentLimit(int _documentLimit) {
    this->documentLimit = _documentLimit;
}

void Parameter::setConstructDictionaryInDisk(bool constructDictionaryInDisk) {
    this->dictionaryInDisk = constructDictionaryInDisk;
    if (constructDictionaryInDisk){
        this->indexInDisk = true;
    }
}

void Parameter::setWordLimit(int _wordLimit) {
    this->wordLimit = _wordLimit;
}

DocumentType Parameter::getDocumentType() const{
    return documentType;
}

void Parameter::setDocumentType(DocumentType _documentType) {
    this->documentType = _documentType;
}
