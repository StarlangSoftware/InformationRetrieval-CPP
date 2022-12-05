//
// Created by Olcay Taner YILDIZ on 25.07.2022.
//

#include "NGramIndex.h"

NGramIndex::NGramIndex() : InvertedIndex(){

}

NGramIndex::NGramIndex(TermDictionary& dictionary, const vector<TermOccurrence>& terms) : InvertedIndex(
        dictionary, terms) {
}

NGramIndex::NGramIndex(const string& fileName) : InvertedIndex(fileName) {
}
