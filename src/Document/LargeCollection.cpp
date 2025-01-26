//
// Created by Olcay Taner YILDIZ on 3.12.2022.
//

#include "LargeCollection.h"
#include "../Index/Term.h"

/**
 * Constructor for the LargeCollection class. In large collections, both dictionary and indexes are stored in the
 * disk and don't fit in memory in their construction phase and usage phase. For that reason, in their construction
 * phase, multiple disk reads and optimizations are needed.
 * @param directory Directory where the document collection resides.
 * @param parameter Search parameter
 */
LargeCollection::LargeCollection(const string &directory, const Parameter &parameter) : DiskCollection(directory,
                                                                                                       parameter) {
    constructDictionaryAndIndexesInDisk();
}

/**
 * The method constructs the term dictionary and all indexes on disk.
 */
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

/**
 * In single pass in memory indexing, the dictionary files are merged to get the final dictionary file. This method
 * checks if all parallel dictionaries are combined or not.
 * @param currentWords Current pointers for the words in parallel dictionaries. currentWords[0] is the current word
 *                     in the first dictionary to be combined, currentWords[2] is the current word in the second
 *                     dictionary to be combined etc.
 * @param size size of the currentWords
 * @return True, if all merge operation is completed, false otherwise.
 */
bool LargeCollection::notCombinedAllDictionaries(const string *currentWords, int size) const{
    for (int i = 0; i < size; i++){
        if (!currentWords[i].empty()){
            return true;
        }
    }
    return false;
}

bool compareWord2(const Word* wordA, const Word* wordB)
{
    return wordA->getName() < wordB->getName();
}

/**
 * In single pass in memory indexing, the dictionary files are merged to get the final dictionary file. This method
 * identifies the dictionaries whose words to be merged are lexicographically the first. They will be selected and
 * combined in the next phase.
 * @param currentWords Current pointers for the words in parallel dictionaries. currentWords[0] is the current word
 *                     in the first dictionary to be combined, currentWords[2] is the current word in the second
 *                     dictionary to be combined etc.
 * @param size Size of the currentWords.
 * @return An array list of indexes for the dictionaries, whose words to be merged are lexicographically the first.
 */
vector<int> LargeCollection::selectDictionariesWithMinimumWords(const string *currentWords, int size) const{
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

/**
 * In single pass in memory indexing, the dictionary files are merged to get the final dictionary file. This method
 * implements the merging algorithm. Reads the dictionary files in parallel and at each iteration puts the smallest
 * word to the final dictionary. Updates the pointers of the dictionaries accordingly.
 * @param _name Name of the collection.
 * @param tmpName Temporary name of the dictionary files.
 * @param blockCount Number of dictionaries to be merged.
 */
void LargeCollection::combineMultipleDictionariesInDisk(const string& _name, const string& tmpName, int blockCount) const {
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
    delete[] currentIdList;
    delete[] currentWords;
    delete[] files;
    outfile.close();
}

/**
 * In single pass in memory indexing, the dictionaries and inverted indexes are created in a block wise manner. They
 * do not fit in memory, therefore documents are read one by one. For each document, the terms are added to the
 * current dictionary and inverted index. If the number of documents read are above the limit, current partial
 * dictionary and inverted index file are saved and new dictionary and inverted index file are open. After reading
 * all  documents, we combine the dictionary and inverted index files to get the final dictionary and inverted index
 * file.
 * @param termType If term type is TOKEN, the terms are single word, if the term type is PHRASE, the terms are
 *                 bi-words.
 */
void LargeCollection::constructDictionaryAndInvertedIndexInDisk(TermType termType) const {
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

/**
 * In single pass in memory indexing, the dictionaries and positional indexes are created in a block wise manner.
 * They do not fit in memory, therefore documents are read one by one. For each document, the terms are added to the
 * current dictionary and positional index. If the number of documents read are above the limit, current partial
 * dictionary and positional index file are saved and new dictionary and positional index file are open. After
 * reading all documents, we combine the dictionary and positional index files to get the final dictionary and
 * positional index file.
 * @param termType If term type is TOKEN, the terms are single word, if the term type is PHRASE, the terms are
 *                 bi-words.
 */
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

/**
 * The method constructs the N-Grams from the given tokens in a string. The method first identifies the tokens in
 * the line by splitting from space, then constructs N-Grams for those tokens and adds N-Grams to the N-Gram
 * dictionary and N-Gram index.
 * @param line String containing the tokens.
 * @param k N in N-Gram.
 * @param nGramDictionary N-Gram term dictionary
 * @param nGramIndex N-Gram inverted index
 */
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

/**
 * In single pass in memory indexing, the dictionaries and N-gram indexes are created in a block wise manner.
 * They do not fit in memory, therefore documents are read one by one. For each document, the terms are added to the
 * current dictionary and N-gram index. If the number of documents read are above the limit, current partial
 * dictionary and N-gram index file are saved and new dictionary and N-gram index file are open. After
 * reading all documents, we combine the dictionary and N-gram index files to get the final dictionary and
 * N-gram index file.
 */
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
