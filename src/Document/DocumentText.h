//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#ifndef INFORMATIONRETRIEVAL_DOCUMENTTEXT_H
#define INFORMATIONRETRIEVAL_DOCUMENTTEXT_H


#include <set>
#include "Corpus.h"
#include "../Index/TermType.h"
#include "../Index/TermOccurrence.h"

class DocumentText : public Corpus{
public:
    DocumentText();
    explicit DocumentText(string fileName);
    DocumentText(string fileName, SentenceSplitter* sentenceSplitter);
    set<string> constructDistinctWordList(TermType termType);
    vector<TermOccurrence> constructTermList(int docId, TermType termType);
};


#endif //INFORMATIONRETRIEVAL_DOCUMENTTEXT_H
