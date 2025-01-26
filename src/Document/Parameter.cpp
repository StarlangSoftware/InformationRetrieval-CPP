//
// Created by Olcay Taner YILDIZ on 21.07.2022.
//

#include "Parameter.h"

/**
 * Accessor for the index type search parameter. Index can be inverted index or incidence matrix.
 * @return Index type search parameter
 */
IndexType Parameter::getIndexType() const{
    return indexType;
}

/**
 * Accessor for the loadIndexesFromFile search parameter. If loadIndexesFromFile is true, all the indexes will be
 * read from the file, otherwise they will be reconstructed.
 * @return loadIndexesFromFile search parameter
 */
bool Parameter::loadIndexesFromFile() const{
    return indexesFromFile;
}

/**
 * Accessor for the disambiguator search parameter. The disambiguator is used for morphological disambiguation for
 * the terms in Turkish.
 * @return disambiguator search parameter
 */
MorphologicalDisambiguator* Parameter::getDisambiguator() const{
    return disambiguator;
}

/**
 * Accessor for the fsm search parameter. The fsm is used for morphological analysis for  the terms in Turkish.
 * @return fsm search parameter
 */
FsmMorphologicalAnalyzer* Parameter::getFsm() const{
    return fsm;
}

/**
 * Accessor for the constructPhraseIndex search parameter. If constructPhraseIndex is true, phrase indexes will be
 * reconstructed or used in query processing.
 * @return constructPhraseIndex search parameter
 */
bool Parameter::constructPhraseIndex() const{
    return phraseIndex;
}

/**
 * Accessor for the normalizeDocument search parameter. If normalizeDocument is true, the terms in the document will
 * be preprocessed by morphological anaylysis and some preprocessing techniques.
 * @return normalizeDocument search parameter
 */
bool Parameter::normalizeDocument() const{
    return _normalizeDocument;
}

/**
 * Accessor for the positionalIndex search parameter. If positionalIndex is true, positional indexes will be
 * reconstructed or used in query processing.
 * @return positionalIndex search parameter
 */
bool Parameter::constructPositionalIndex() const{
    return positionalIndex;
}

/**
 * Accessor for the limitNumberOfDocumentsLoaded search parameter. If limitNumberOfDocumentsLoaded is true,
 * the query result will be filtered according to the documentLimit search parameter.
 * @return limitNumberOfDocumentsLoaded search parameter
 */
bool Parameter::limitNumberOfDocumentsLoaded() const{
    return _limitNumberOfDocumentsLoaded;
}

/**
 * Accessor for the documentLimit search parameter. If limitNumberOfDocumentsLoaded is true,  the query result will
 * be filtered according to the documentLimit search parameter.
 * @return limitNumberOfDocumentsLoaded search parameter
 */
int Parameter::getDocumentLimit() const{
    return documentLimit;
}

/**
 * Accessor for the constructNGramIndex search parameter. If constructNGramIndex is true, N-Gram indexes will be
 * reconstructed or used in query processing.
 * @return constructNGramIndex search parameter
 */
bool Parameter::constructNGramIndex() const{
    return nGramIndex;
}

/**
 * Accessor for the wordLimit search parameter. wordLimit is the limit on the partial term dictionary size. For
 * large collections, we term dictionaries are divided into multiple files, this parameter sets the number of terms
 * in those separate dictionaries.
 * @return wordLimit search parameter
 */
int Parameter::getWordLimit() const{
    return wordLimit;
}

/**
 * Mutator for the index type search parameter. Index can be inverted index or incidence matrix.
 * @param _indexType Index type search parameter
 */
void Parameter::setIndexType(IndexType _indexType) {
    this->indexType = _indexType;
}

/**
 * Mutator for the loadIndexesFromFile search parameter. If loadIndexesFromFile is true, all the indexes will be
 * read from the file, otherwise they will be reconstructed.
 * @param loadIndexesFromFile loadIndexesFromFile search parameter
 */
void Parameter::setLoadIndexesFromFile(bool loadIndexesFromFile) {
    this->indexesFromFile = loadIndexesFromFile;
}

/**
 * Mutator for the disambiguator search parameter. The disambiguator is used for morphological disambiguation for
 * the terms in Turkish.
 * @param _disambiguator disambiguator search parameter
 */
void Parameter::setDisambiguator(MorphologicalDisambiguator* _disambiguator) {
    this->disambiguator = _disambiguator;
}

/**
 * Mutator for the fsm search parameter. The fsm is used for morphological analysis for the terms in Turkish.
 * @param _fsm fsm search parameter
 */
void Parameter::setFsm(FsmMorphologicalAnalyzer* _fsm) {
    this->fsm = _fsm;
}

/**
 * Mutator for the normalizeDocument search parameter. If normalizeDocument is true, the terms in the document will
 * be preprocessed by morphological anaylysis and some preprocessing techniques.
 * @param normalizeDocument normalizeDocument search parameter
 */
void Parameter::setNormalizeDocument(bool normalizeDocument) {
    this->_normalizeDocument = normalizeDocument;
}

/**
 * Mutator for the constructPhraseIndex search parameter. If constructPhraseIndex is true, phrase indexes will be
 * reconstructed or used in query processing.
 * @param _phraseIndex constructPhraseIndex search parameter
 */
void Parameter::setPhraseIndex(bool _phraseIndex) {
    this->phraseIndex = _phraseIndex;
}

/**
 * Mutator for the positionalIndex search parameter. If positionalIndex is true, positional indexes will be
 * reconstructed or used in query processing.
 * @param _positionalIndex positionalIndex search parameter
 */
void Parameter::setPositionalIndex(bool _positionalIndex) {
    this->positionalIndex = _positionalIndex;
}

/**
 * Mutator for the constructNGramIndex search parameter. If constructNGramIndex is true, N-Gram indexes will be
 * reconstructed or used in query processing.
 * @param _nGramIndex constructNGramIndex search parameter
 */
void Parameter::setNGramIndex(bool _nGramIndex) {
    this->nGramIndex = _nGramIndex;
}

/**
 * Mutator for the limitNumberOfDocumentsLoaded search parameter. If limitNumberOfDocumentsLoaded is true,
 * the query result will be filtered according to the documentLimit search parameter.
 * @param limitNumberOfDocumentsLoaded limitNumberOfDocumentsLoaded search parameter
 */
void Parameter::setLimitNumberOfDocumentsLoaded(bool limitNumberOfDocumentsLoaded) {
    this->_limitNumberOfDocumentsLoaded = limitNumberOfDocumentsLoaded;
}

/**
 * Mutator for the documentLimit search parameter. If limitNumberOfDocumentsLoaded is true,  the query result will
 * be filtered according to the documentLimit search parameter.
 * @param _documentLimit limitNumberOfDocumentsLoaded search parameter
 */
void Parameter::setDocumentLimit(int _documentLimit) {
    this->documentLimit = _documentLimit;
}

/**
 * Mutator for the documentLimit search parameter. If limitNumberOfDocumentsLoaded is true,  the query result will
 * be filtered according to the documentLimit search parameter.
 * @param _wordLimit limitNumberOfDocumentsLoaded search parameter
 */
void Parameter::setWordLimit(int _wordLimit) {
    this->wordLimit = _wordLimit;
}

/**
 * Accessor for the document type search parameter. Document can be normal or a categorical document.
 * @return Document type search parameter
 */
DocumentType Parameter::getDocumentType() const{
    return documentType;
}

/**
 * Mutator for the document type search parameter. Document can be normal or a categorical document.
 * @param _documentType Document type search parameter
 */
void Parameter::setDocumentType(DocumentType _documentType) {
    this->documentType = _documentType;
}

/**
 * Accessor for the representativeCount search parameter. representativeCount is the maximum number of representative
 * words in the category based query search.
 * @return representativeCount search parameter
 */
int Parameter::getRepresentationCount() const {
    return representativeCount;
}

/**
 * Mutator for the representativeCount search parameter. representativeCount is the maximum number of representative
 * words in the category based query search.
 * @param _representativeCount representativeCount search parameter
 */
void Parameter::setRepresentativeCount(int _representativeCount) {
    this->representativeCount = _representativeCount;
}
