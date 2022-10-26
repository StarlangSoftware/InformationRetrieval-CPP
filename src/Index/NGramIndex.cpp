//
// Created by Olcay Taner YILDIZ on 25.07.2022.
//

#include "NGramIndex.h"

NGramIndex::NGramIndex() : InvertedIndex(){

}

NGramIndex::NGramIndex(const TermDictionary& dictionary, const vector<TermOccurrence>& terms, Comparator comparator) : InvertedIndex(
        dictionary, terms, comparator) {
}

NGramIndex::NGramIndex(const string& fileName) : InvertedIndex(fileName) {
}
