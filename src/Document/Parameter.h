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
    IndexType getIndexType() const;
    Comparator getWordComparator() const;
    bool loadIndexesFromFile() const;
    MorphologicalDisambiguator* getDisambiguator() const;
    FsmMorphologicalAnalyzer* getFsm() const;
    bool constructPhraseIndex() const;
    bool normalizeDocument() const;
    bool constructPositionalIndex() const;
    bool constructNGramIndex() const;
    bool constructIndexInDisk() const;
    bool limitNumberOfDocumentsLoaded() const;
    int getDocumentLimit() const;
    bool constructDictionaryInDisk() const;
    int getWordLimit() const;
    void setIndexType(IndexType _indexType);
    void setWordComparator(Comparator _wordComparator);
    void setLoadIndexesFromFile(bool loadIndexesFromFile);
    void setDisambiguator(MorphologicalDisambiguator* _disambiguator);
    void setFsm(FsmMorphologicalAnalyzer* _fsm);
    void setNormalizeDocument(bool normalizeDocument);
    void setPhraseIndex(bool _phraseIndex);
    void setPositionalIndex(bool _positionalIndex);
    void setNGramIndex(bool _nGramIndex);
    void setConstructIndexInDisk(bool constructIndexInDisk);
    void setLimitNumberOfDocumentsLoaded(bool limitNumberOfDocumentsLoaded);
    void setDocumentLimit(int _documentLimit);
    void setConstructDictionaryInDisk(bool constructDictionaryInDisk);
    void setWordLimit(int _wordLimit);
    DocumentType getDocumentType() const;
    void setDocumentType(DocumentType _documentType);
};


#endif //INFORMATIONRETRIEVAL_PARAMETER_H
