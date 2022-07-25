//
// Created by Olcay Taner YILDIZ on 25.07.2022.
//

#include "NGramIndex.h"

NGramIndex::NGramIndex() : InvertedIndex(){

}

NGramIndex::NGramIndex(TermDictionary dictionary, vector<TermOccurrence> terms, Comparator comparator) : InvertedIndex(
        dictionary, terms, comparator) {
}

NGramIndex::NGramIndex(string fileName) : InvertedIndex(fileName) {
}

vector<TermOccurrence> NGramIndex::constructNGrams(string word, int termId, int k) {
    vector<TermOccurrence> nGrams;
    if (word.length() >= k - 1){
        for (int l = -1; l < word.length() - k + 2; l++){
            string term;
            if (l == -1){
                term = "$" + Word::substring(word, 0, k - 1);
            } else {
                if (l == word.length() - k + 1){
                    term = Word::substring(word, l, k - 1) + "$";
                } else {
                    term = Word::substring(word, l, k);
                }
            }
            nGrams.emplace_back(TermOccurrence(Word(term), termId, l));
        }
    }
    return nGrams;
}
