//
// Created by Olcay Taner YILDIZ on 26.07.2022.
//

#include <iostream>
#include "Collection.h"
#include "../Index/TermOccurrenceComparator.h"
#include "../Index/Term.h"

using std::filesystem::directory_iterator;

Collection::Collection(const string& directory, Parameter parameter) {
    name = directory;
    indexType = parameter.getIndexType();
    comparator = parameter.getWordComparator();
    this->parameter = parameter;
    int j = 0;
    for (const auto & file : directory_iterator(directory)){
        if (!file.is_directory() && file.path().string().ends_with(".txt")){
            if (!parameter.limitNumberOfDocumentsLoaded() || j < parameter.getDocumentLimit()){
                Document document = Document(file.path(), file.path(), j);
                documents.emplace_back(document);
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
        docTerms = documentText.constructTermList(doc.getDocId(), termType);
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
        for (const string& item : items){
            words.insert(item);
        }
    }
    return words;
}

bool Collection::notCombinedAllIndexes(const int* currentIdList, int size) {
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

vector<int> Collection::selectIndexesWithMinimumTermIds(const int *currentIdList, int size) {
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

vector<int> Collection::selectDictionariesWithMinimumWords(string *currentWords, int size) {
    vector<int> result;
    string min;
    turkishWordComparator wordComparator = turkishWordComparator(Dictionary::turkishComparatorMap);
    for (int i = 0; i < size; i++){
        if (!currentWords[i].empty() && (min.empty() || wordComparator.operator()(new Word(currentWords[i]), new Word(min)))){
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

void Collection::combineMultipleDictionariesInDisk(const string& _name, const string& tmpName, int blockCount) {
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

void
Collection::addNGramsToDictionaryAndIndex(const string& line, int k, TermDictionary nGramDictionary, NGramIndex nGramIndex) {
    int wordId = stoi(line.substr(0, line.find(' ')));
    string word = line.substr(line.find(' ') + 1);
    hash<string> hash;
    vector<TermOccurrence> biGrams = NGramIndex::constructNGrams(word, wordId, k);
    for (TermOccurrence term : biGrams){
        int termId;
        int wordIndex = nGramDictionary.getWordIndex(term.getTerm().getName());
        if (wordIndex != -1){
            termId = ((Term*) nGramDictionary.getWord(wordIndex))->getTermId();
        } else {
            termId = hash(term.getTerm().getName());
            nGramDictionary.addTerm(term.getTerm().getName(), termId);
        }
        nGramIndex.add(termId, wordId);
    }
}

void Collection::constructNGramDictionaryAndIndexInDisk() {
    int i = 0, blockCount = 0;
    ifstream inputFile;
    string line;
    TermDictionary _biGramDictionary = TermDictionary(comparator);
    TermDictionary _triGramDictionary = TermDictionary(comparator);
    NGramIndex _biGramIndex = NGramIndex();
    NGramIndex _triGramIndex = NGramIndex();
    inputFile.open(name + "-dictionary.txt", ifstream::in);
    while (inputFile.good()) {
        getline(inputFile, line);
        if (i < parameter.getWordLimit()){
            i++;
        } else {
            _biGramDictionary.save("tmp-biGram-" + ::to_string(blockCount));
            _triGramDictionary.save("tmp-triGram-" + ::to_string(blockCount));
            _biGramDictionary = TermDictionary(comparator);
            _triGramDictionary = TermDictionary(comparator);
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

void Collection::combineMultipleInvertedIndexesInDisk(const string& _name, const string& tmpName, int blockCount) {
    ifstream* files;
    int* currentIdList;
    string line;
    PostingList* currentPostingLists;
    currentIdList = new int[blockCount];
    currentPostingLists = new PostingList[blockCount];
    files = new ifstream[blockCount];
    ofstream outfile;
    outfile.open(_name + "-postings.txt", ofstream::out);
    for (int i = 0; i < blockCount; i++){
        files[i].open("tmp-" + tmpName + ::to_string(i) + "-postings.txt", ifstream::in);
        getline(files[i], line);
        vector<string> items = Word::split(line);
        currentIdList[i] = stoi(items[0]);
        getline(files[i], line);
        currentPostingLists[i] = PostingList(line);
    }
    while (notCombinedAllIndexes(currentIdList, blockCount)){
        vector<int> indexesToCombine = selectIndexesWithMinimumTermIds(currentIdList, blockCount);
        PostingList mergedPostingList = currentPostingLists[indexesToCombine[0]];
        for (int i = 1; i < indexesToCombine.size(); i++){
            mergedPostingList = mergedPostingList.unionWith(currentPostingLists[indexesToCombine[i]]);
        }
        mergedPostingList.writeToFile(outfile, currentIdList[indexesToCombine[0]]);
        for (int i : indexesToCombine) {
            getline(files[i], line);
            if (!line.empty()) {
                vector<string> items = Word::split(line);
                currentIdList[i] = stoi(items[0]);
                getline(files[i], line);
                currentPostingLists[i] = PostingList(line);
            } else {
                currentIdList[i] = -1;
            }
        }
    }
    for (int i = 0; i < blockCount; i++){
        files[i].close();
    }
    outfile.close();
}

void Collection::constructInvertedIndexInDisk(TermDictionary _dictionary, TermType termType) {
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

void Collection::constructDictionaryAndInvertedIndexInDisk(TermType termType) {
    int i = 0, blockCount = 0;
    InvertedIndex _invertedIndex = InvertedIndex();
    TermDictionary _dictionary = TermDictionary(comparator);
    hash<string> hash;
    for (Document doc : documents){
        if (i < parameter.getDocumentLimit()){
            i++;
        } else {
            _dictionary.save("tmp-" + ::to_string(blockCount));
            _dictionary = TermDictionary(comparator);
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
                termId = hash(word);
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

void Collection::combineMultiplePositionalIndexesInDisk(const string& _name, int blockCount) {
    ifstream* files;
    int* currentIdList;
    string line;
    PositionalPostingList* currentPostingLists;
    currentIdList = new int[blockCount];
    currentPostingLists = new PositionalPostingList[blockCount];
    files = new ifstream [blockCount];
    ofstream outfile;
    outfile.open(_name + "-positionalPostings.txt", ofstream::out);
    for (int i = 0; i < blockCount; i++){
        files[i].open("tmp-" + ::to_string(i) + "-positionalPostings.txt");
        getline(files[i], line);
        vector<string> items = Word::split(" ");
        currentIdList[i] = stoi(items[0]);
        currentPostingLists[i] = PositionalPostingList(files[i], stoi(items[1]));
    }
    while (notCombinedAllIndexes(currentIdList, blockCount)){
        vector<int> indexesToCombine = selectIndexesWithMinimumTermIds(currentIdList, blockCount);
        PositionalPostingList mergedPostingList = currentPostingLists[indexesToCombine[0]];
        for (int i = 1; i < indexesToCombine.size(); i++){
            mergedPostingList = mergedPostingList.unionWith(currentPostingLists[indexesToCombine[i]]);
        }
        mergedPostingList.writeToFile(outfile, currentIdList[indexesToCombine[0]]);
        for (int i : indexesToCombine) {
            getline(files[i], line);
            if (!line.empty()) {
                vector<string> items = Word::split(" ");
                currentIdList[i] = stoi(items[0]);
                currentPostingLists[i] = PositionalPostingList(files[i], stoi(items[1]));
            } else {
                currentIdList[i] = -1;
            }
        }
    }
    for (int i = 0; i < blockCount; i++){
        files[i].close();
    }
    outfile.close();
}

void Collection::constructDictionaryAndPositionalIndexInDisk(TermType termType) {
    int i = 0, blockCount = 0;
    PositionalIndex _positionalIndex = PositionalIndex();
    TermDictionary _dictionary = TermDictionary(comparator);
    hash<string> hash;
    for (Document doc : documents){
        if (i < parameter.getDocumentLimit()){
            i++;
        } else {
            _dictionary.save("tmp-" + ::to_string(blockCount));
            _dictionary = TermDictionary(comparator);
            _positionalIndex.save("tmp-" + ::to_string(blockCount));
            _positionalIndex = PositionalIndex();
            blockCount++;
            i = 0;
        }
        DocumentText documentText = doc.loadDocument();
        vector<TermOccurrence> terms = documentText.constructTermList(doc.getDocId(), termType);
        for (TermOccurrence termOccurrence : terms){
            int termId;
            int wordIndex = _dictionary.getWordIndex(termOccurrence.getTerm().getName());
            if (wordIndex != -1){
                termId = ((Term*) _dictionary.getWord(wordIndex))->getTermId();
            } else {
                termId = hash(termOccurrence.getTerm().getName());
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

void Collection::constructPositionalIndexInDisk(TermDictionary _dictionary, TermType termType) {
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
        for (TermOccurrence termOccurrence : terms){
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

void Collection::constructNGramIndex() {
    vector<TermOccurrence> terms = dictionary.constructTermsFromDictionary(2);
    biGramDictionary = TermDictionary(comparator, terms);
    biGramIndex = NGramIndex(biGramDictionary, terms, comparator);
    terms = dictionary.constructTermsFromDictionary(3);
    triGramDictionary = TermDictionary(comparator, terms);
    triGramIndex = NGramIndex(triGramDictionary, terms, comparator);
}

VectorSpaceModel
Collection::getVectorSpaceModel(int docId, TermWeighting termWeighting, DocumentWeighting documentWeighting) {
    return VectorSpaceModel(positionalIndex.getTermFrequencies(docId), positionalIndex.getDocumentFrequencies(), documents.size(), termWeighting, documentWeighting);
}

double
Collection::cosineSimilarity(Collection collection2, VectorSpaceModel spaceModel1, VectorSpaceModel spaceModel2) {
    int index1, index2;
    double sum = 0.0;
    for (index1 = 0; index1 < vocabularySize(); index1++){
        if (spaceModel1.get(index1) > 0.0){
            index2 = collection2.dictionary.getWordIndex(dictionary.getWord(index1)->getName());
            if (index2 != -1 && spaceModel2.get(index2) > 0.0){
                sum += spaceModel1.get(index1) * spaceModel2.get(index2);
            }
        }
    }
    return sum;
}

double Collection::cosineSimilarity(VectorSpaceModel spaceModel1, VectorSpaceModel spaceModel2) {
    int index;
    double sum = 0.0;
    for (index = 0; index < vocabularySize(); index++){
        sum += spaceModel1.get(index) * spaceModel2.get(index);
    }
    return sum;
}

Matrix Collection::cosineSimilarity(TermWeighting termWeighting, DocumentWeighting documentWeighting) {
    Matrix result = Matrix(size(), size());
    VectorSpaceModel* models;
    models = new VectorSpaceModel[documents.size()];
    for (int i = 0; i < documents.size(); i++){
        models[i] = getVectorSpaceModel(i, termWeighting, documentWeighting);
    }
    for (int i = 0; i < documents.size(); i++){
        for (int j = 0; j < documents.size(); j++){
            result.setValue(i, j, cosineSimilarity(models[i], models[j]));
        }
    }
    return result;
}

vector<string>
Collection::sharedWordList(Collection collection2, VectorSpaceModel spaceModel1, VectorSpaceModel spaceModel2) {
    int index1, index2;
    vector<string> list;
    for (index1 = 0; index1 < vocabularySize(); index1++){
        if (spaceModel1.get(index1) > 0.0){
            index2 = collection2.dictionary.getWordIndex(dictionary.getWord(index1)->getName());
            if (index2 != -1 && spaceModel2.get(index2) > 0.0){
                list.emplace_back(dictionary.getWord(index1)->getName());
            }
        }
    }
    return list;
}

QueryResult Collection::searchCollection(const Query& query, RetrievalType retrievalType, TermWeighting termWeighting,
                                         DocumentWeighting documentWeighting) {
    switch (indexType){
        case IndexType::INCIDENCE_MATRIX:
            return incidenceMatrix.search(query, dictionary);
        case   IndexType::INVERTED_INDEX:
            switch (retrievalType){
                case    RetrievalType::BOOLEAN:return invertedIndex.search(query, dictionary);
                case RetrievalType::POSITIONAL:return positionalIndex.positionalSearch(query, dictionary);
                case     RetrievalType::RANKED:return positionalIndex.rankedSearch(query, dictionary, documents, termWeighting, documentWeighting);
            }
    }
    return QueryResult();
}
