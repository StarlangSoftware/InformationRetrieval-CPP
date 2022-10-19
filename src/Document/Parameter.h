//
// Created by Olcay Taner YILDIZ on 21.07.2022.
//

#ifndef INFORMATIONRETRIEVAL_PARAMETER_H
#define INFORMATIONRETRIEVAL_PARAMETER_H


#include "IndexType.h"
#include "Dictionary/Dictionary.h"
#include "MorphologicalDisambiguator.h"
#include "FsmMorphologicalAnalyzer.h"
#include "DocumentType.h"

class Parameter {
private:
    IndexType indexType = IndexType::INVERTED_INDEX;
    Comparator wordComparator = Comparator::TURKISH;
    bool indexesFromFile = false;
    MorphologicalDisambiguator* disambiguator = nullptr;
    FsmMorphologicalAnalyzer* fsm = nullptr;
    bool _normalizeDocument = false;
    bool phraseIndex = true;
    bool positionalIndex = true;
    bool nGramIndex = true;
    bool indexInDisk = false;
    bool dictionaryInDisk = false;
    bool _limitNumberOfDocumentsLoaded = false;
    int documentLimit = 1000;
    int wordLimit = 10000;
    DocumentType documentType = DocumentType::NORMAL;
public:
    Parameter() = default;
    IndexType getIndexType();
    Comparator getWordComparator();
    bool loadIndexesFromFile();
    MorphologicalDisambiguator* getDisambiguator();
    FsmMorphologicalAnalyzer* getFsm();
    bool constructPhraseIndex();
    bool normalizeDocument();
    bool constructPositionalIndex();
    bool constructNGramIndex();
    bool constructIndexInDisk();
    bool limitNumberOfDocumentsLoaded();
    int getDocumentLimit();
    bool constructDictionaryInDisk();
    int getWordLimit();
    void setIndexType(IndexType indexType);
    void setWordComparator(Comparator wordComparator);
    void setLoadIndexesFromFile(bool loadIndexesFromFile);
    void setDisambiguator(MorphologicalDisambiguator* disambiguator);
    void setFsm(FsmMorphologicalAnalyzer* fsm);
    void setNormalizeDocument(bool normalizeDocument);
    void setPhraseIndex(bool phraseIndex);
    void setPositionalIndex(bool positionalIndex);
    void setNGramIndex(bool nGramIndex);
    void setConstructIndexInDisk(bool constructIndexInDisk);
    void setLimitNumberOfDocumentsLoaded(bool limitNumberOfDocumentsLoaded);
    void setDocumentLimit(int documentLimit);
    void setConstructDictionaryInDisk(bool constructDictionaryInDisk);
    void setWordLimit(int wordLimit);
    DocumentType getDocumentType();
    void setDocumentType(DocumentType documentType);
};


#endif //INFORMATIONRETRIEVAL_PARAMETER_H
