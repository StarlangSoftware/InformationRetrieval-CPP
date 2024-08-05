//
// Created by Olcay Taner YILDIZ on 25.07.2022.
//

#include <fstream>
#include <string>
#include "PositionalPostingList.h"
#include "Dictionary/Word.h"

/**
 * Constructor of the PositionalPostingList class. Initializes the list.
 */
PositionalPostingList::PositionalPostingList() = default;

/**
 * Reads a positional posting list from a file. Reads N lines, where each line stores a positional posting. The
 * first item in the line shows document id. The second item in the line shows the number of positional postings.
 * Other items show the positional postings.
 * @param br Input stream to read from.
 * @param count Number of positional postings for this positional posting list.
 */
PositionalPostingList::PositionalPostingList(ifstream &infile, int count) {
    string line;
    for (int i = 0; i < count; i++){
        getline(infile, line);
        vector<string> ids = Word::split(line);
        int numberOfPositionalPostings = stoi(ids[1]);
        int docId = stoi(ids[0]);
        for (int j = 0; j < numberOfPositionalPostings; j++){
            int positionalPosting = stoi(ids[j + 2]);
            add(docId, positionalPosting);
        }
    }
}

/**
 * Returns the number of positional postings in the posting list.
 * @return Number of positional postings in the posting list.
 */
int PositionalPostingList::size() const{
    return postings.size();
}

/**
 * Does a binary search on the positional postings list for a specific document id.
 * @param docId Document id to be searched.
 * @return The position of the document id in the positional posting list. If it does not exist, the method returns
 * -1.
 */
int PositionalPostingList::getIndex(int docId) const{
    int begin = 0, end = size() - 1, middle;
    while (begin <= end){
        middle = (begin + end) / 2;
        if (docId == postings[middle].getDocId()){
            return middle;
        } else {
            if (docId < postings[middle].getDocId()){
                end = middle - 1;
            } else {
                begin = middle + 1;
            }
        }
    }
    return -1;
}

/**
 * Converts the positional postings list to a query result object. Simply adds all positional postings one by one
 * to the result.
 * @return QueryResult object containing the positional postings in this object.
 */
QueryResult PositionalPostingList::toQueryResult() const{
    QueryResult result = QueryResult();
    for (const PositionalPosting& posting: postings){
        result.add(posting.getDocId());
    }
    return result;
}

/**
 * Adds a new positional posting (document id and position) to the posting list.
 * @param docId New document id to be added to the positional posting list.
 * @param position New position to be added to the positional posting list.
 */
void PositionalPostingList::add(int docId, int position) {
    int index = getIndex(docId);
    if (index == -1){
        postings.emplace_back(PositionalPosting(docId));
        postings[postings.size() - 1].add(position);
    } else {
        postings[index].add(position);
    }
}

/**
 * Gets the positional posting at position index.
 * @param index Position of the positional posting.
 * @return The positional posting at position index.
 */
PositionalPosting PositionalPostingList::get(int index) const {
    return postings[index];
}

/**
 * Returns simple union of two positional postings list p1 and p2. The algorithm assumes the intersection of two
 * positional postings list is empty, therefore the union is just concatenation of two positional postings lists.
 * @param secondList p2
 * @return Union of two positional postings lists.
 */
PositionalPostingList PositionalPostingList::unionWith(const PositionalPostingList& secondList) const{
    PositionalPostingList result = PositionalPostingList();
    result.postings.insert(result.postings.end(), postings.begin(), postings.end());
    result.postings.insert(result.postings.end(), secondList.postings.begin(), secondList.postings.end());
    return result;
}

/**
 * Algorithm for the intersection of two positional postings lists p1 and p2. We maintain pointers into both lists
 * and walk through the two positional postings lists simultaneously, in time linear in the total number of postings
 * entries. At each step, we compare the docID pointed to by both pointers. If they are not the same, we advance the
 * pointer pointing to the smaller docID. Otherwise, we advance both pointers and do the same intersection search on
 * the positional lists of two documents. Similarly, we compare the positions pointed to by both position pointers.
 * If they are successive, we add the position to the result and advance both position pointers. Otherwise, we
 * advance the pointer pointing to the smaller position.
 * @param secondList p2, second posting list.
 * @return Intersection of two postings lists p1 and p2.
 */
PositionalPostingList PositionalPostingList::intersection(const PositionalPostingList& secondList) const{
    int i = 0, j = 0;
    PositionalPostingList result = PositionalPostingList();
    while (i < postings.size() && j < secondList.postings.size()){
        PositionalPosting p1 = postings[i];
        PositionalPosting p2 = secondList.postings[j];
        if (p1.getDocId() == p2.getDocId()){
            int position1 = 0;
            int position2 = 0;
            vector<Posting> postings1 = p1.getPositions();
            vector<Posting> postings2 = p2.getPositions();
            while (position1 < postings1.size() && position2 < postings2.size()){
                if (postings1[position1].getId() + 1 == postings2[position2].getId()){
                    result.add(p1.getDocId(), postings2[position2].getId());
                    position1++;
                    position2++;
                } else {
                    if (postings1[position1].getId() + 1 < postings2[position2].getId()){
                        position1++;
                    } else {
                        position2++;
                    }
                }
            }
            i++;
            j++;
        } else {
            if (p1.getDocId() < p2.getDocId()){
                i++;
            } else {
                j++;
            }
        }
    }
    return result;
}

/**
 * Converts the positional posting list to a string. String is of the form all postings separated via space.
 * @return String form of the positional posting list.
 */
string PositionalPostingList::to_string() const{
    string result;
    for (const PositionalPosting& positionalPosting : postings){
        result += "\t" + positionalPosting.to_string() + "\n";
    }
    return result;
}

/**
 * Prints this object into a file with the given index.
 * @param printWriter Output stream to write the file.
 * @param index Position of this positional posting list in the inverted index.
 */
void PositionalPostingList::writeToFile(ofstream &outfile, int index) {
    if (size() > 0){
        outfile << ::to_string(index) << " " << ::to_string(size()) << "\n";
        outfile << to_string();
    }
}
