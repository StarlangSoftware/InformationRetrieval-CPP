//
// Created by Olcay Taner YILDIZ on 3.12.2022.
//

#ifndef INFORMATIONRETRIEVAL_DISKCOLLECTION_H
#define INFORMATIONRETRIEVAL_DISKCOLLECTION_H


#include "AbstractCollection.h"

class DiskCollection : public AbstractCollection{
private:
    bool notCombinedAllIndexes(const int* currentIdList, int size) const;
    vector<int> selectIndexesWithMinimumTermIds(const int* currentIdList, int size) const;
protected:
    void combineMultipleInvertedIndexesInDisk(const string& _name, const string& tmpName, int blockCount) const;
    void combineMultiplePositionalIndexesInDisk(const string& _name, int blockCount) const;
public:
    DiskCollection(const string& directory, const Parameter& parameter);
};


#endif //INFORMATIONRETRIEVAL_DISKCOLLECTION_H
