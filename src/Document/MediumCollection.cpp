//
// Created by Olcay Taner YILDIZ on 3.12.2022.
//

#include "MediumCollection.h"

MediumCollection::MediumCollection(const string &directory, const Parameter &parameter) : DiskCollection(directory,
                                                                                                         parameter) {
    constructIndexesInDisk();
}

set<string> MediumCollection::constructDistinctWordList(TermType termType) const{
    set<string> words;
    for (Document doc : documents){
        DocumentText documentText = doc.loadDocument();
        set<string> items = documentText.constructDistinctWordList(termType);
        for (const string& item : items){
            words.insert(item);
        }
    }
    return words;
}

void MediumCollection::constructInvertedIndexInDisk(TermDictionary& _dictionary, TermType termType) {
    int i = 0, blockCount = 0;
    InvertedIndex _invertedIndex = InvertedIndex();
    for (Document doc : documents){
        if (i < parameter.getDocumentLimit()){
            i++;
        } else {
            _invertedIndex.save("tmp-" + ::to_string(blockCount));
            _invertedIndex = InvertedIndex();
            blockCount++;
            i = 0;
        }
        DocumentText documentText = doc.loadDocument();
        set<string> wordList = documentText.constructDistinctWordList(termType);
        for (const string& word : wordList){
            int termId = _dictionary.getWordIndex(word);
            _invertedIndex.add(termId, doc.getDocId());
        }
    }
    if (!documents.empty()){
        _invertedIndex.save("tmp-" + ::to_string(blockCount));
        blockCount++;
    }
    if (termType == TermType::TOKEN){
        combineMultipleInvertedIndexesInDisk(name, "", blockCount);
    } else {
        combineMultipleInvertedIndexesInDisk(name + "-phrase", "", blockCount);
    }
}

void MediumCollection::constructPositionalIndexInDisk(TermDictionary& _dictionary, TermType termType) {
    int i = 0, blockCount = 0;
    PositionalIndex _positionalIndex = PositionalIndex();
    for (Document doc : documents){
        if (i < parameter.getDocumentLimit()){
            i++;
        } else {
            _positionalIndex.save("tmp-" + ::to_string(blockCount));
            _positionalIndex = PositionalIndex();
            blockCount++;
            i = 0;
        }
        DocumentText documentText = doc.loadDocument();
        vector<TermOccurrence> terms = documentText.constructTermList(doc.getDocId(), termType);
        for (const TermOccurrence& termOccurrence : terms){
            int termId = _dictionary.getWordIndex(termOccurrence.getTerm().getName());
            _positionalIndex.addPosition(termId, termOccurrence.getDocId(), termOccurrence.getPosition());
        }
    }
    if (!documents.empty()){
        _positionalIndex.save("tmp-" + ::to_string(blockCount));
        blockCount++;
    }
    if (termType == TermType::TOKEN){
        combineMultiplePositionalIndexesInDisk(name, blockCount);
    } else {
        combineMultiplePositionalIndexesInDisk(name + "-phrase", blockCount);
    }
}

void MediumCollection::constructIndexesInDisk() {
    set<string> wordList = constructDistinctWordList(TermType::TOKEN);
    dictionary = TermDictionary(comparator, wordList);
    constructInvertedIndexInDisk(dictionary, TermType::TOKEN);
    if (parameter.constructPositionalIndex()){
        constructPositionalIndexInDisk(dictionary, TermType::TOKEN);
    }
    if (parameter.constructPhraseIndex()){
        wordList = constructDistinctWordList(TermType::PHRASE);
        phraseDictionary = TermDictionary(comparator, wordList);
        constructInvertedIndexInDisk(phraseDictionary, TermType::PHRASE);
        if (parameter.constructPositionalIndex()){
            constructPositionalIndexInDisk(phraseDictionary, TermType::PHRASE);
        }
    }
    if (parameter.constructNGramIndex()){
        constructNGramIndex();
    }
}
