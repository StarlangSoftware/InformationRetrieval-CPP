//
// Created by Olcay Taner YILDIZ on 26.07.2022.
//

#include "Collection.h"
#include "../Index/TermOccurrenceComparator.h"

using std::filesystem::directory_iterator;

Collection::Collection(string directory, Parameter parameter) {
    name = directory;
    indexType = parameter.getIndexType();
    comparator = parameter.getWordComparator();
    this->parameter = parameter;
    int j = 0;
    for (const auto & file : directory_iterator(directory)){
        if (!file.is_directory() && file.path().string().ends_with(".txt")){
            if (!parameter.limitNumberOfDocumentsLoaded() || j < parameter.getDocumentLimit()){
                Document document = Document(file.path(), file.path(), j);
                j++;
            }
        }
    }
    if (parameter.loadIndexesFromFile()){
        dictionary = TermDictionary(comparator, directory);
        invertedIndex = InvertedIndex(directory);
        if (parameter.constructPositionalIndex()){
            positionalIndex = PositionalIndex(directory);
        }
        if (parameter.constructPhraseIndex()){
            phraseDictionary = TermDictionary(comparator, directory + "-phrase");
            phraseIndex = InvertedIndex(directory + "-phrase");
            if (parameter.constructPositionalIndex()){
                phrasePositionalIndex = PositionalIndex(directory + "-phrase");
            }
        }
        if (parameter.constructNGramIndex()){
            biGramDictionary = TermDictionary(comparator, directory + "-biGram");
            triGramDictionary = TermDictionary(comparator, directory + "-triGram");
            biGramIndex = NGramIndex(directory + "-biGram");
            triGramIndex = NGramIndex(directory + "-triGram");
        }
    } else {
        if (parameter.constructDictionaryInDisk()){
            constructDictionaryInDisk();
        } else {
            if (parameter.constructIndexInDisk()){
                constructIndexesInDisk();
            } else {
                constructIndexesInMemory();
            }
        }
    }
}

int Collection::size() {
    return documents.size();
}

int Collection::vocabularySize() {
    return dictionary.size();
}

void Collection::save() {
    if (indexType == IndexType::INVERTED_INDEX){
        dictionary.save(name);
        invertedIndex.save(name);
        if (parameter.constructPositionalIndex()){
            positionalIndex.save(name);
        }
        if (parameter.constructPhraseIndex()){
            phraseDictionary.save(name + "-phrase");
            phraseIndex.save(name + "-phrase");
            if (parameter.constructPositionalIndex()){
                phrasePositionalIndex.save(name + "-phrase");
            }
        }
        if (parameter.constructNGramIndex()){
            biGramDictionary.save(name + "-biGram");
            triGramDictionary.save(name + "-triGram");
            biGramIndex.save(name + "-biGram");
            triGramIndex.save(name + "-triGram");
        }
    }
}

void Collection::constructDictionaryInDisk() {
    constructDictionaryAndInvertedIndexInDisk(TermType::TOKEN);
    if (parameter.constructPositionalIndex()){
        constructDictionaryAndPositionalIndexInDisk(TermType::TOKEN);
    }
    if (parameter.constructPhraseIndex()){
        constructDictionaryAndInvertedIndexInDisk(TermType::PHRASE);
        if (parameter.constructPositionalIndex()){
            constructDictionaryAndPositionalIndexInDisk(TermType::PHRASE);
        }
    }
    if (parameter.constructNGramIndex()){
        constructNGramDictionaryAndIndexInDisk();
    }
}

void Collection::constructIndexesInDisk() {
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

void Collection::constructIndexesInMemory() {
    vector<TermOccurrence> terms = constructTerms(TermType::TOKEN);
    dictionary = TermDictionary(comparator, terms);
    switch (indexType){
        case IndexType::INCIDENCE_MATRIX:
            incidenceMatrix = IncidenceMatrix(terms, dictionary, documents.size());
            break;
        case IndexType::INVERTED_INDEX:
            invertedIndex = InvertedIndex(dictionary, terms, comparator);
            if (parameter.constructPositionalIndex()){
                positionalIndex = PositionalIndex(dictionary, terms, comparator);
            }
            if (parameter.constructPhraseIndex()){
                terms = constructTerms(TermType::PHRASE);
                phraseDictionary = TermDictionary(comparator, terms);
                phraseIndex = InvertedIndex(phraseDictionary, terms, comparator);
                if (parameter.constructPositionalIndex()){
                    phrasePositionalIndex = PositionalIndex(phraseDictionary, terms, comparator);
                }
            }
            if (parameter.constructNGramIndex()){
                constructNGramIndex();
            }
            break;
    }
}

vector<TermOccurrence> Collection::constructTerms(TermType termType) {
    vector<TermOccurrence> terms;
    vector<TermOccurrence> docTerms;
    for (Document doc : documents){
        DocumentText documentText = doc.loadDocument();
        docTerms = documentText.constructTermList(doc, termType);
        terms.insert(terms.end(), docTerms.begin(), docTerms.end());
    }
    std::sort(terms.begin(), terms.end(),  termOccurrenceComparator(Dictionary::turkishComparatorMap));
    return terms;
}

set<string> Collection::constructDistinctWordList(TermType termType) {
    set<string> words;
    for (Document doc : documents){
        DocumentText documentText = doc.loadDocument();
        set<string> items = documentText.constructDistinctWordList(termType);
        for (string item : items){
            words.insert(item);
        }
    }
    return words;
}

bool Collection::notCombinedAllIndexes(int* currentIdList, int size) {
    for (int i = 0; i < size; i++){
        if (currentIdList[i] != -1){
            return true;
        }
    }
    return false;
}

bool Collection::notCombinedAllDictionaries(string *currentWords, int size) {
    for (int i = 0; i < size; i++){
        if (!currentWords[i].empty()){
            return true;
        }
    }
    return false;
}

vector<int> Collection::selectIndexesWithMinimumTermIds(int *currentIdList, int size) {
    vector<int> result;
    int min = INT_MAX;
    for (int i = 0; i < size; i++){
        if (currentIdList[i] != -1 && currentIdList[i] < min){
            min = currentIdList[i];
        }
    }
    for (int i = 0; i < size; i++){
        if (currentIdList[i] == min){
            result.emplace_back(i);
        }
    }
    return result;
}
