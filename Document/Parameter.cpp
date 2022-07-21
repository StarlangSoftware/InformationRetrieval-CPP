//
// Created by Olcay Taner YILDIZ on 21.07.2022.
//

#include "Parameter.h"

IndexType Parameter::getIndexType() {
    return indexType;
}

Comparator Parameter::getWordComparator() {
    return wordComparator;
}

bool Parameter::loadIndexesFromFile() {
    return indexesFromFile;
}

MorphologicalDisambiguator* Parameter::getDisambiguator() {
    return disambiguator;
}

FsmMorphologicalAnalyzer Parameter::getFsm() {
    return fsm;
}

bool Parameter::constructPhraseIndex() {
    return phraseIndex;
}

bool Parameter::normalizeDocument() {
    return _normalizeDocument;
}

bool Parameter::constructPositionalIndex() {
    return positionalIndex;
}

bool Parameter::limitNumberOfDocumentsLoaded() {
    return _limitNumberOfDocumentsLoaded;
}

int Parameter::getDocumentLimit() {
    return documentLimit;
}

bool Parameter::constructDictionaryInDisk() {
    return dictionaryInDisk;
}

bool Parameter::constructNGramIndex() {
    return nGramIndex;
}

bool Parameter::constructIndexInDisk() {
    return indexInDisk;
}

int Parameter::getWordLimit() {
    return wordLimit;
}

void Parameter::setIndexType(IndexType indexType) {
    this->indexType = indexType;
}

void Parameter::setWordComparator(Comparator wordComparator) {
    this->wordComparator = wordComparator;
}

void Parameter::setLoadIndexesFromFile(bool loadIndexesFromFile) {
    this->indexesFromFile = loadIndexesFromFile;
}

void Parameter::setDisambiguator(MorphologicalDisambiguator* disambiguator) {
    this->disambiguator = disambiguator;
}

void Parameter::setFsm(FsmMorphologicalAnalyzer fsm) {
    this->fsm = fsm;
}

void Parameter::setNormalizeDocument(bool normalizeDocument) {
    this->_normalizeDocument = normalizeDocument;
}

void Parameter::setPhraseIndex(bool phraseIndex) {
    this->phraseIndex = phraseIndex;
}

void Parameter::setPositionalIndex(bool positionalIndex) {
    this->positionalIndex = positionalIndex;
}

void Parameter::setNGramIndex(bool nGramIndex) {
    this->nGramIndex = nGramIndex;
}

void Parameter::setConstructIndexInDisk(bool constructIndexInDisk) {
    this->indexInDisk = constructIndexInDisk;
}

void Parameter::setLimitNumberOfDocumentsLoaded(bool limitNumberOfDocumentsLoaded) {
    this->_limitNumberOfDocumentsLoaded = limitNumberOfDocumentsLoaded;
}

void Parameter::setDocumentLimit(int documentLimit) {
    this->documentLimit = documentLimit;
}

void Parameter::setConstructDictionaryInDisk(bool constructDictionaryInDisk) {
    this->dictionaryInDisk = constructDictionaryInDisk;
    if (constructDictionaryInDisk){
        this->indexInDisk = true;
    }
}

void Parameter::setWordLimit(int wordLimit) {
    this->wordLimit = wordLimit;
}
