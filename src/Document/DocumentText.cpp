//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#include "DocumentText.h"

/**
 * Empty constructor for the DocumentText class.
 */
DocumentText::DocumentText() = default;

/**
 * Another constructor for the DocumentText class. Calls super with the given file name.
 * @param fileName File name of the corpus
 */
DocumentText::DocumentText(const string& fileName) : Corpus(fileName) {

}

/**
 * Another constructor for the DocumentText class. Calls super with the given file name and sentence splitter.
 * @param fileName File name of the corpus
 * @param sentenceSplitter Sentence splitter class that separates sentences.
 */
DocumentText::DocumentText(const string& fileName, SentenceSplitter* sentenceSplitter) : Corpus(fileName, sentenceSplitter) {

}

/**
 * Given the corpus, creates a hash set of distinct terms. If term type is TOKEN, the terms are single word, if
 * the term type is PHRASE, the terms are bi-words.
 * @param termType If term type is TOKEN, the terms are single word, if the term type is PHRASE, the terms are
 *                 bi-words.
 * @return Hash set of terms occurring in the document.
 */
set<string> DocumentText::constructDistinctWordList(TermType termType) const{
    set<string> words;
    for (int i = 0; i < sentenceCount(); i++){
        Sentence* sentence = getSentence(i);
        for (int j = 0; j < sentence->wordCount(); j++){
            switch (termType) {
                case TermType::TOKEN:
                    words.insert(sentence->getWord(j)->getName());
                    break;
                case TermType::PHRASE:
                    if (j < sentence->wordCount() - 1){
                        words.insert(sentence->getWord(j)->getName() + " " + sentence->getWord(j + 1)->getName());
                    }
            }
        }
    }
    return words;
}

/**
 * Given the corpus, creates an array of terms occurring in the document in that order. If term type is TOKEN, the
 * terms are single word, if the term type is PHRASE, the terms are bi-words.
 * @param docId Id of the document
 * @param termType If term type is TOKEN, the terms are single word, if the term type is PHRASE, the terms are
 *                 bi-words.
 * @return Array list of terms occurring in the document.
 */
vector<TermOccurrence> DocumentText::constructTermList(int docId, TermType termType) const{
    vector<TermOccurrence> terms;
    int size = 0;
    for (int i = 0; i < sentenceCount(); i++){
        Sentence* sentence = getSentence(i);
        for (int j = 0; j < sentence->wordCount(); j++){
            switch (termType){
                case TermType::TOKEN:
                    terms.emplace_back(TermOccurrence(*(sentence->getWord(j)), docId, size));
                    size++;
                    break;
                case TermType::PHRASE:
                    if (j < sentence->wordCount() - 1){
                        terms.emplace_back(TermOccurrence(Word(sentence->getWord(j)->getName() + " " + sentence->getWord(j + 1)->getName()), docId, size));
                        size++;
                    }
            }
        }
    }
    return terms;
}
