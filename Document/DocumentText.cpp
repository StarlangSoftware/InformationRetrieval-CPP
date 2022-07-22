//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#include "DocumentText.h"

DocumentText::DocumentText(string fileName) : Corpus(fileName) {

}

DocumentText::DocumentText(string fileName, SentenceSplitter* sentenceSplitter) : Corpus(fileName, sentenceSplitter) {

}

set<string> DocumentText::constructDistinctWordList(TermType termType) {
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
