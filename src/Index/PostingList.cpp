//
// Created by Olcay Taner YILDIZ on 25.07.2022.
//

#include <fstream>
#include "PostingList.h"
#include "Dictionary/Word.h"

/**
 * Constructor of the PostingList class. Initializes the list.
 */
PostingList::PostingList() = default;

/**
 * Constructs a posting list from a line, which contains postings separated with space.
 * @param line A string containing postings separated with space character.
 */
PostingList::PostingList(const string& line) {
    vector<string> ids = Word::split(line);
    for (const string& id : ids){
        add(stoi(id));
    }
}

/**
 * Adds a new posting (document id) to the posting list.
 * @param docId New document id to be added to the posting list.
 */
void PostingList::add(int docId) {
    postings.emplace_back(new Posting(docId));
}

/**
 * Returns the number of postings in the posting list.
 * @return Number of postings in the posting list.
 */
int PostingList::size() const{
    return postings.size();
}

/**
 * Algorithm for the intersection of two postings lists p1 and p2. We maintain pointers into both lists and walk
 * through the two postings lists simultaneously, in time linear in the total number of postings entries. At each
 * step, we compare the docID pointed to by both pointers. If they are the same, we put that docID in the results
 * list, and advance both pointers. Otherwise, we advance the pointer pointing to the smaller docID.
 * @param secondList p2, second posting list.
 * @return Intersection of two postings lists p1 and p2.
 */
PostingList PostingList::intersection(const PostingList& secondList) const{
    int i = 0, j = 0;
    PostingList result = PostingList();
    while (i < size() && j < secondList.size()){
        Posting* p1 = postings[i];
        Posting* p2 = secondList.postings[j];
        if (p1->getId() == p2->getId()){
            result.add(p1->getId());
            i++;
            j++;
        } else {
            if (p1->getId() < p2->getId()){
                i++;
            } else {
                j++;
            }
        }
    }
    return result;
}

/**
 * Returns simple union of two postings list p1 and p2. The algorithm assumes the intersection of two postings list
 * is empty, therefore the union is just concatenation of two postings lists.
 * @param secondList p2
 * @return Union of two postings lists.
 */
PostingList PostingList::unionWith(const PostingList& secondList) const{
    PostingList result = PostingList();
    result.postings.insert(result.postings.end(), postings.begin(), postings.end());
    result.postings.insert(result.postings.end(), secondList.postings.begin(), secondList.postings.end());
    return result;
}

/**
 * Converts the postings list to a query result object. Simply adds all postings one by one to the result.
 * @return QueryResult object containing the postings in this object.
 */
QueryResult PostingList::toQueryResult() const{
    QueryResult result = QueryResult();
    for (Posting* posting : postings){
        result.add(posting->getId());
    }
    return result;
}

/**
 * Converts the posting list to a string. String is of the form all postings separated via space.
 * @return String form of the posting list.
 */
string PostingList::to_string() const{
    string result;
    for (Posting* posting : postings){
        result += ::to_string(posting->getId()) + " ";
    }
    return result + "\n";
}

/**
 * Prints this object into a file with the given index.
 * @param printWriter Output stream to write the file.
 * @param index Position of this posting list in the inverted index.
 */
void PostingList::writeToFile(ofstream& outfile, int index) {
    if (size() > 0){
        outfile << index << " " << size() << "\n";
        outfile << to_string();
    }
}


