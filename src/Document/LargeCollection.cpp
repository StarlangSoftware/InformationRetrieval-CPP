//
// Created by Olcay Taner YILDIZ on 3.12.2022.
//

#include "LargeCollection.h"
#include "../Index/Term.h"

LargeCollection::LargeCollection(const string &directory, const Parameter &parameter) : DiskCollection(directory,
                                                                                                       parameter) {
    constructDictionaryAndIndexesInDisk();
}

void LargeCollection::constructDictionaryAndIndexesInDisk() {
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

bool LargeCollection::notCombinedAllDictionaries(string *currentWords, int size) const{
    for (int i = 0; i < size; i++){
        if (!currentWords[i].empty()){
            return true;
        }
    }
    return false;
}

bool compareWord2(Word* wordA, Word* wordB)
{
    return wordA->getName() < wordB->getName();
}

vector<int> LargeCollection::selectDictionariesWithMinimumWords(string *currentWords, int size) const{
    vector<int> result;
    string min;
    for (int i = 0; i < size; i++){
        if (!currentWords[i].empty() && (min.empty() || compareWord2(new Word(currentWords[i]), new Word(min)))){
            min = currentWords[i];
        }
    }
    for (int i = 0; i < size; i++){
        if (currentWords[i] == min){
            result.emplace_back(i);
        }
    }
    return result;
}

void LargeCollection::combineMultipleDictionariesInDisk(const string& _name, const string& tmpName, int blockCount) {
    ifstream * files;
    int* currentIdList;
    string* currentWords;
    string line;
    currentIdList = new int[blockCount];
    currentWords = new string[blockCount];
    files = new ifstream[blockCount];
    ofstream outfile;
    outfile.open(_name + "-dictionary.txt", ofstream :: out);
    for (int i = 0; i < blockCount; i++){
        files[i].open("tmp-" + tmpName + ::to_string(i) + "-dictionary.txt", ifstream :: in);
        getline(files[i], line);
        currentIdList[i] = stoi(line.substr(0, line.find(' ')));
        currentWords[i] = line.substr(line.find(' ') + 1);
    }
    while (notCombinedAllDictionaries(currentWords, blockCount)){
        vector<int> indexesToCombine = selectDictionariesWithMinimumWords(currentWords, blockCount);
        outfile << ::to_string(currentIdList[indexesToCombine[0]]) << " " << currentWords[indexesToCombine[0]] << "\n";
        for (int i : indexesToCombine) {
            getline(files[i], line);
            if (!line.empty()) {
                currentIdList[i] = stoi(line.substr(0, line.find(' ')));
                currentWords[i] = line.substr(line.find(' ') + 1);
            } else {
                currentWords[i] = "";
            }
        }
    }
    for (int i = 0; i < blockCount; i++){
        files[i].close();
    }
    outfile.close();
}

void LargeCollection::constructDictionaryAndInvertedIndexInDisk(TermType termType) {
    int i = 0, blockCount = 0;
    InvertedIndex _invertedIndex = InvertedIndex();
    TermDictionary _dictionary = TermDictionary();
    hash<string> hash;
    for (Document doc : documents){
        if (i < parameter.getDocumentLimit()){
            i++;
        } else {
            _dictionary.save("tmp-" + ::to_string(blockCount));
            _dictionary = TermDictionary();
            _invertedIndex.save("tmp-" + ::to_string(blockCount));
            _invertedIndex = InvertedIndex();
            blockCount++;
            i = 0;
        }
        DocumentText documentText = doc.loadDocument();
        set<string> wordList = documentText.constructDistinctWordList(termType);
        for (const string& word : wordList){
            int termId;
            int wordIndex = _dictionary.getWordIndex(word);
            if (wordIndex != -1){
                termId = ((Term*) _dictionary.getWord(wordIndex))->getTermId();
            } else {
                termId = abs((int) hash(word));
                _dictionary.addTerm(word, termId);
            }
            _invertedIndex.add(termId, doc.getDocId());
        }
    }
    if (!documents.empty()){
        _dictionary.save("tmp-" + ::to_string(blockCount));
        _invertedIndex.save("tmp-" + ::to_string(blockCount));
        blockCount++;
    }
    if (termType == TermType::TOKEN){
        combineMultipleDictionariesInDisk(name, "", blockCount);
        combineMultipleInvertedIndexesInDisk(name, "", blockCount);
    } else {
        combineMultipleDictionariesInDisk(name + "-phrase", "", blockCount);
        combineMultipleInvertedIndexesInDisk(name + "-phrase", "", blockCount);
    }
}

void LargeCollection::constructDictionaryAndPositionalIndexInDisk(TermType termType) {
    int i = 0, blockCount = 0;
    PositionalIndex _positionalIndex = PositionalIndex();
    TermDictionary _dictionary = TermDictionary();
    hash<string> hash;
    for (Document doc : documents){
        if (i < parameter.getDocumentLimit()){
            i++;
        } else {
            _dictionary.save("tmp-" + ::to_string(blockCount));
            _dictionary = TermDictionary();
            _positionalIndex.save("tmp-" + ::to_string(blockCount));
            _positionalIndex = PositionalIndex();
            blockCount++;
            i = 0;
        }
        DocumentText documentText = doc.loadDocument();
        vector<TermOccurrence> terms = documentText.constructTermList(doc.getDocId(), termType);
        for (const TermOccurrence& termOccurrence : terms){
            int termId;
            int wordIndex = _dictionary.getWordIndex(termOccurrence.getTerm().getName());
            if (wordIndex != -1){
                termId = ((Term*) _dictionary.getWord(wordIndex))->getTermId();
            } else {
                termId = abs((int) hash(termOccurrence.getTerm().getName()));
                _dictionary.addTerm(termOccurrence.getTerm().getName(), termId);
            }
            _positionalIndex.addPosition(termId, termOccurrence.getDocId(), termOccurrence.getPosition());
        }
    }
    if (!documents.empty()){
        _dictionary.save("tmp-" + ::to_string(blockCount));
        _positionalIndex.save("tmp-" + ::to_string(blockCount));
        blockCount++;
    }
    if (termType == TermType::TOKEN){
        combineMultipleDictionariesInDisk(name, "", blockCount);
        combineMultiplePositionalIndexesInDisk(name, blockCount);
    } else {
        combineMultipleDictionariesInDisk(name + "-phrase", "", blockCount);
        combineMultiplePositionalIndexesInDisk(name + "-phrase", blockCount);
    }
}

void LargeCollection::addNGramsToDictionaryAndIndex(const string& line, int k, TermDictionary& nGramDictionary, NGramIndex& nGramIndex) {
    int wordId = stoi(line.substr(0, line.find(' ')));
    string word = line.substr(line.find(' ') + 1);
    hash<string> hash;
    vector<TermOccurrence> biGrams = TermDictionary::constructNGrams(word, wordId, k);
    for (const TermOccurrence& term : biGrams){
        int termId;
        int wordIndex = nGramDictionary.getWordIndex(term.getTerm().getName());
        if (wordIndex != -1){
            termId = ((Term*) nGramDictionary.getWord(wordIndex))->getTermId();
        } else {
            termId = abs((int) hash(term.getTerm().getName()));
            nGramDictionary.addTerm(term.getTerm().getName(), termId);
        }
        nGramIndex.add(termId, wordId);
    }
}

void LargeCollection::constructNGramDictionaryAndIndexInDisk() {
    int i = 0, blockCount = 0;
    ifstream inputFile;
    string line;
    TermDictionary _biGramDictionary = TermDictionary();
    TermDictionary _triGramDictionary = TermDictionary();
    NGramIndex _biGramIndex = NGramIndex();
    NGramIndex _triGramIndex = NGramIndex();
    inputFile.open(name + "-dictionary.txt", ifstream::in);
    while (inputFile.good()) {
        getline(inputFile, line);
        if (line.empty()){
            continue;
        }
        if (i < parameter.getWordLimit()){
            i++;
        } else {
            _biGramDictionary.save("tmp-biGram-" + ::to_string(blockCount));
            _triGramDictionary.save("tmp-triGram-" + ::to_string(blockCount));
            _biGramDictionary = TermDictionary();
            _triGramDictionary = TermDictionary();
            _biGramIndex.save("tmp-biGram-" + ::to_string(blockCount));
            _biGramIndex = NGramIndex();
            _triGramIndex.save("tmp-triGram-" + ::to_string(blockCount));
            _triGramIndex = NGramIndex();
            blockCount++;
            i = 0;
        }
        addNGramsToDictionaryAndIndex(line, 2, _biGramDictionary, _biGramIndex);
        addNGramsToDictionaryAndIndex(line, 3, _triGramDictionary, _triGramIndex);
    }
    inputFile.close();
    if (!documents.empty()){
        _biGramDictionary.save("tmp-biGram-" + ::to_string(blockCount));
        _triGramDictionary.save("tmp-triGram-" + ::to_string(blockCount));
        _biGramIndex.save("tmp-biGram-" + ::to_string(blockCount));
        _triGramIndex.save("tmp-triGram-" + ::to_string(blockCount));
        blockCount++;
    }
    combineMultipleDictionariesInDisk(name + "-biGram", "biGram-", blockCount);
    combineMultipleDictionariesInDisk(name + "-triGram", "triGram-", blockCount);
    combineMultipleInvertedIndexesInDisk(name + "-biGram", "biGram-", blockCount);
    combineMultipleInvertedIndexesInDisk(name + "-triGram", "triGram-", blockCount);
}
