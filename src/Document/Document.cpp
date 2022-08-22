//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#include "Document.h"
#include "../include/TurkishSplitter.h"

Document::Document(string absoluteFileName, string fileName, int docId) {
    this->docId = docId;
    this->absoluteFileName = absoluteFileName;
    this->fileName = fileName;
}

DocumentText Document::loadDocument() {
    DocumentText documentText = DocumentText(absoluteFileName, new TurkishSplitter());
    this->size = documentText.numberOfWords();
    return documentText;
}

Corpus Document::normalizeDocument(MorphologicalDisambiguator disambiguator, FsmMorphologicalAnalyzer fsm) {
    Corpus corpus = Corpus(absoluteFileName);
    for (int i = 0; i < corpus.sentenceCount(); i++){
        Sentence* sentence = corpus.getSentence(i);
        FsmParseList* parses = fsm.robustMorphologicalAnalysis(*sentence);
        vector<FsmParse> correctParses = disambiguator.disambiguate(parses, sentence->wordCount());
        auto* newSentence = new Sentence();
        for (FsmParse fsmParse : correctParses){
            newSentence->addWord(new Word(fsmParse.getWord()->getName()));
        }
        corpus.addSentence(newSentence);
    }
    size = corpus.numberOfWords();
    return corpus;
}

int Document::getDocId() {
    return docId;
}

string Document::getFileName() {
    return fileName;
}

string Document::getAbsoluteFileName() {
    return absoluteFileName;
}

int Document::getSize() {
    return size;
}
