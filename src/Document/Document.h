//
// Created by Olcay Taner YILDIZ on 22.07.2022.
//

#ifndef INFORMATIONRETRIEVAL_DOCUMENT_H
#define INFORMATIONRETRIEVAL_DOCUMENT_H

#include <string>
#include "DocumentText.h"
#include "MorphologicalDisambiguator.h"
#include "FsmMorphologicalAnalyzer.h"

using namespace std;

class Document {
private:
    string absoluteFileName;
    string fileName;
    int docId;
    int size = 0;
public:
    Document(string absoluteFileName, string fileName, int docId);
    DocumentText loadDocument();
    Corpus normalizeDocument(MorphologicalDisambiguator disambiguator, FsmMorphologicalAnalyzer fsm);
    int getDocId();
    string getFileName();
    string getAbsoluteFileName();
    int getSize();
};


#endif //INFORMATIONRETRIEVAL_DOCUMENT_H
