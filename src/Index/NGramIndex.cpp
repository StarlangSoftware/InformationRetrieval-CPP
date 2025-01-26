//
// Created by Olcay Taner YILDIZ on 25.07.2022.
//

#include "NGramIndex.h"

/**
 * Empty constructor for the NGram index.
 */
NGramIndex::NGramIndex() : InvertedIndex(){

}

/**
 * Constructs an NGram index from a list of sorted tokens. The terms array should be sorted before calling this
 * method. Calls the constructor for the InvertedIndex.
 * @param dictionary Term dictionary
 * @param terms Sorted list of tokens in the memory collection.
 */
NGramIndex::NGramIndex(TermDictionary& dictionary, const vector<TermOccurrence>& terms) : InvertedIndex(
        dictionary, terms) {
}

/**
 * Reads the NGram index from an input file.
 * @param fileName Input file name for the NGram index.
 */
NGramIndex::NGramIndex(const string& fileName) : InvertedIndex(fileName) {
}
