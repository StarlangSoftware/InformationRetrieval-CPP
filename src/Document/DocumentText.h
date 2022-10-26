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
    explicit DocumentText(const string& fileName);
    DocumentText(const string& fileName, SentenceSplitter* sentenceSplitter);
    set<string> constructDistinctWordList(TermType termType) const;
    vector<TermOccurrence> constructTermList(int docId, TermType termType) const;
};


#endif //INFORMATIONRETRIEVAL_DOCUMENTTEXT_H
