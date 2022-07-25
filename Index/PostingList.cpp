//
// Created by Olcay Taner YILDIZ on 25.07.2022.
//

#include <fstream>
#include "PostingList.h"
#include "Word.h"

PostingList::PostingList() = default;

PostingList::PostingList(string line) {
    vector<string> ids = Word::split(line);
    for (string id : ids){
        add(stoi(id));
    }
}

void PostingList::add(int docId) {
    postings.emplace_back(new Posting(docId));
}

int PostingList::size() {
    return postings.size();
}

PostingList PostingList::intersection(PostingList secondList) {
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

PostingList PostingList::unionWith(PostingList secondList) {
    PostingList result = PostingList();
    result.postings.insert(result.postings.end(), postings.begin(), postings.end());
    result.postings.insert(result.postings.end(), secondList.postings.begin(), secondList.postings.end());
    return result;
}

QueryResult PostingList::toQueryResult() {
    QueryResult result = QueryResult();
    for (Posting* posting : postings){
        result.add(posting->getId());
    }
    return result;
}

string PostingList::to_string() {
    string result;
    for (Posting* posting : postings){
        result += ::to_string(posting->getId()) + " ";
    }
    return result + "\n";
}

void PostingList::writeToFile(ofstream& outfile, int index) {
    if (size() > 0){
        outfile << index << " " << size() << "\n";
        outfile << to_string();
    }
}


