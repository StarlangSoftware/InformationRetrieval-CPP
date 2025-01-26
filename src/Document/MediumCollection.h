//
// Created by Olcay Taner YILDIZ on 3.12.2022.
//

#ifndef INFORMATIONRETRIEVAL_MEDIUMCOLLECTION_H
#define INFORMATIONRETRIEVAL_MEDIUMCOLLECTION_H


#include "DiskCollection.h"

class MediumCollection : public DiskCollection{
private:
    void constructIndexesInDisk();
    set<string> constructDistinctWordList(TermType termType) const;
    void constructInvertedIndexInDisk(TermDictionary& _dictionary, TermType termType) const;
    void constructPositionalIndexInDisk(TermDictionary& _dictionary, TermType termType) const;
public:
    MediumCollection(const string& directory, const Parameter& parameter);
};


#endif //INFORMATIONRETRIEVAL_MEDIUMCOLLECTION_H
