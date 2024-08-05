//
// Created by Olcay Taner YILDIZ on 3.12.2022.
//

#include "MediumCollection.h"

/**
 * Constructor for the MediumCollection class. In medium collections, dictionary is kept in memory and indexes are
 * stored in the disk and don't fit in memory in their construction phase and usage phase. For that reason, in their
 * construction phase, multiple disk reads and optimizations are needed.
 * @param directory Directory where the document collection resides.
 * @param parameter Search parameter
 */
MediumCollection::MediumCollection(const string &directory, const Parameter &parameter) : DiskCollection(directory,
                                                                                                         parameter) {
    constructIndexesInDisk();
}

/**
 * Given the document collection, creates a hash set of distinct terms. If term type is TOKEN, the terms are single
 * word, if the term type is PHRASE, the terms are bi-words. Each document is loaded into memory and distinct
 * word list is created. Since the dictionary can be kept in memory, all operations can be done in memory.
 * @param termType If term type is TOKEN, the terms are single word, if the term type is PHRASE, the terms are
 *                 bi-words.
 * @return Hash set of terms occurring in the document collection.
 */
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

/**
 * In block sort based indexing, the inverted index is created in a block wise manner. It does not fit in memory,
 * therefore documents are read one by one. For each document, the terms are added to the inverted index. If the
 * number of documents read are above the limit, current partial inverted index file is saved and new inverted index
 * file is open. After reading all documents, we combine the inverted index files to get the final inverted index
 * file.
 * @param dictionary Term dictionary.
 * @param termType If term type is TOKEN, the terms are single word, if the term type is PHRASE, the terms are
 *                 bi-words.
 */
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

/**
 * In block sort based indexing, the positional index is created in a block wise manner. It does not fit in memory,
 * therefore documents are read one by one. For each document, the terms are added to the positional index. If the
 * number of documents read are above the limit, current partial positional index file is saved and new positional
 * index file is open. After reading all documents, we combine the posiitonal index files to get the final
 * positional index file.
 * @param dictionary Term dictionary.
 * @param termType If term type is TOKEN, the terms are single word, if the term type is PHRASE, the terms are
 *                 bi-words.
 */
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

/**
 * In block sort based indexing, the indexes are created in a block wise manner. They do not fit in memory, therefore
 * documents are read one by one. According to the search parameter, inverted index, positional index, phrase
 * indexes, N-Gram indexes are constructed in disk.
 */
void MediumCollection::constructIndexesInDisk() {
    set<string> wordList = constructDistinctWordList(TermType::TOKEN);
    dictionary = TermDictionary(wordList);
    constructInvertedIndexInDisk(dictionary, TermType::TOKEN);
    if (parameter.constructPositionalIndex()){
        constructPositionalIndexInDisk(dictionary, TermType::TOKEN);
    }
    if (parameter.constructPhraseIndex()){
        wordList = constructDistinctWordList(TermType::PHRASE);
        phraseDictionary = TermDictionary(wordList);
        constructInvertedIndexInDisk(phraseDictionary, TermType::PHRASE);
        if (parameter.constructPositionalIndex()){
            constructPositionalIndexInDisk(phraseDictionary, TermType::PHRASE);
        }
    }
    if (parameter.constructNGramIndex()){
        constructNGramIndex();
    }
}
