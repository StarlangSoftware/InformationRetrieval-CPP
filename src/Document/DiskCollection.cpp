//
// Created by Olcay Taner YILDIZ on 3.12.2022.
//

#include "DiskCollection.h"

DiskCollection::DiskCollection(const string& directory, const Parameter& parameter) : AbstractCollection(directory, parameter) {
}

bool DiskCollection::notCombinedAllIndexes(const int* currentIdList, int size) const{
    for (int i = 0; i < size; i++){
        if (currentIdList[i] != -2){
            return true;
        }
    }
    return false;
}

vector<int> DiskCollection::selectIndexesWithMinimumTermIds(const int *currentIdList, int size) const{
    vector<int> result;
    int min = INT_MAX;
    for (int i = 0; i < size; i++){
        if (currentIdList[i] != -2 && currentIdList[i] < min){
            min = currentIdList[i];
        }
    }
    for (int i = 0; i < size; i++){
        if (currentIdList[i] == min){
            result.emplace_back(i);
        }
    }
    return result;
}

void DiskCollection::combineMultipleInvertedIndexesInDisk(const string& _name, const string& tmpName, int blockCount) {
    ifstream* files;
    int* currentIdList;
    string line;
    PostingList* currentPostingLists;
    currentIdList = new int[blockCount];
    currentPostingLists = new PostingList[blockCount];
    files = new ifstream[blockCount];
    ofstream outfile;
    outfile.open(_name + "-postings.txt", ofstream::out);
    for (int i = 0; i < blockCount; i++){
        files[i].open("tmp-" + tmpName + ::to_string(i) + "-postings.txt", ifstream::in);
        getline(files[i], line);
        vector<string> items = Word::split(line);
        currentIdList[i] = stoi(items[0]);
        getline(files[i], line);
        currentPostingLists[i] = PostingList(line);
    }
    while (notCombinedAllIndexes(currentIdList, blockCount)){
        vector<int> indexesToCombine = selectIndexesWithMinimumTermIds(currentIdList, blockCount);
        PostingList mergedPostingList = currentPostingLists[indexesToCombine[0]];
        for (int i = 1; i < indexesToCombine.size(); i++){
            mergedPostingList = mergedPostingList.unionWith(currentPostingLists[indexesToCombine[i]]);
        }
        mergedPostingList.writeToFile(outfile, currentIdList[indexesToCombine[0]]);
        for (int i : indexesToCombine) {
            getline(files[i], line);
            if (!line.empty()) {
                vector<string> items = Word::split(line);
                currentIdList[i] = stoi(items[0]);
                getline(files[i], line);
                currentPostingLists[i] = PostingList(line);
            } else {
                currentIdList[i] = -2;
            }
        }
    }
    for (int i = 0; i < blockCount; i++){
        files[i].close();
    }
    outfile.close();
}

void DiskCollection::combineMultiplePositionalIndexesInDisk(const string& _name, int blockCount) {
    ifstream* files;
    int* currentIdList;
    string line;
    PositionalPostingList* currentPostingLists;
    currentIdList = new int[blockCount];
    currentPostingLists = new PositionalPostingList[blockCount];
    files = new ifstream [blockCount];
    ofstream outfile;
    outfile.open(_name + "-positionalPostings.txt", ofstream::out);
    for (int i = 0; i < blockCount; i++){
        files[i].open("tmp-" + ::to_string(i) + "-positionalPostings.txt");
        getline(files[i], line);
        vector<string> items = Word::split(line);
        currentIdList[i] = stoi(items[0]);
        currentPostingLists[i] = PositionalPostingList(files[i], stoi(items[1]));
    }
    while (notCombinedAllIndexes(currentIdList, blockCount)){
        vector<int> indexesToCombine = selectIndexesWithMinimumTermIds(currentIdList, blockCount);
        PositionalPostingList mergedPostingList = currentPostingLists[indexesToCombine[0]];
        for (int i = 1; i < indexesToCombine.size(); i++){
            mergedPostingList = mergedPostingList.unionWith(currentPostingLists[indexesToCombine[i]]);
        }
        mergedPostingList.writeToFile(outfile, currentIdList[indexesToCombine[0]]);
        for (int i : indexesToCombine) {
            getline(files[i], line);
            if (!line.empty()) {
                vector<string> items = Word::split(line);
                currentIdList[i] = stoi(items[0]);
                currentPostingLists[i] = PositionalPostingList(files[i], stoi(items[1]));
            } else {
                currentIdList[i] = -2;
            }
        }
    }
    for (int i = 0; i < blockCount; i++){
        files[i].close();
    }
    outfile.close();
}
