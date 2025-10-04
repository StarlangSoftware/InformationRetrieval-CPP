//
// Created by Olcay Taner YILDIZ on 3.12.2022.
//

#include "DiskCollection.h"

#include <climits>
#include <StringUtils.h>

DiskCollection::DiskCollection(const string& directory, const Parameter& parameter) : AbstractCollection(directory, parameter) {
}

/**
 * In single pass in memory indexing, the index files are merged to get the final index file. This method
 * checks if all parallel index files are combined or not.
 * @param currentIdList Current pointers for the terms in parallel index files. currentIdList[0] is the current term
 *                     in the first index file to be combined, currentIdList[2] is the current term in the second
 *                     index file to be combined etc.
 * @param size size of the currentIdList
 * @return True, if all merge operation is completed, false otherwise.
 */
bool DiskCollection::notCombinedAllIndexes(const int* currentIdList, int size) const{
    for (int i = 0; i < size; i++){
        if (currentIdList[i] != -2){
            return true;
        }
    }
    return false;
}

/**
 * In single pass in memory indexing, the index files are merged to get the final index file. This method
 * identifies the indexes whose terms to be merged have the smallest term id. They will be selected and
 * combined in the next phase.
 * @param currentIdList Current pointers for the terms in parallel index files. currentIdList[0] is the current term
 *                     in the first index file to be combined, currentIdList[2] is the current term in the second
 *                     index file to be combined etc.
 * @param size size of the currentIdList
 * @return An array list of indexes for the index files, whose terms to be merged have the smallest term id.
 */
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

/**
 * In single pass in memory indexing, the index files are merged to get the final index file. This method
 * implements the merging algorithm. Reads the index files in parallel and at each iteration merges the posting
 * lists of the smallest term and put it to the merged index file. Updates the pointers of the indexes accordingly.
 * @param _name Name of the collection.
 * @param tmpName Temporary name of the index files.
 * @param blockCount Number of index files to be merged.
 */
void DiskCollection::combineMultipleInvertedIndexesInDisk(const string& _name, const string& tmpName, int blockCount) const {
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
        vector<string> items = StringUtils::split(line);
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
                vector<string> items = StringUtils::split(line);
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
    delete[] currentIdList;
    delete[] currentPostingLists;
    delete[] files;
    outfile.close();
}

/**
 * In single pass in memory indexing, the index files are merged to get the final index file. This method
 * implements the merging algorithm. Reads the index files in parallel and at each iteration merges the positional
 * posting lists of the smallest term and put it to the merged index file. Updates the pointers of the indexes accordingly.
 * @param _name Name of the collection.
 * @param blockCount Number of index files to be merged.
 */
void DiskCollection::combineMultiplePositionalIndexesInDisk(const string& _name, int blockCount) const {
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
        vector<string> items = StringUtils::split(line);
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
                vector<string> items = StringUtils::split(line);
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
    delete[] currentIdList;
    delete[] currentPostingLists;
    delete[] files;
    outfile.close();
}
