//
// Created by Olcay Taner YILDIZ on 25.07.2022.
//

#include <fstream>
#include "PostingList.h"
#include "Dictionary/Word.h"

PostingList::PostingList() = default;

PostingList::PostingList(const string& line) {
    vector<string> ids = Word::split(line);
    for (const string& id : ids){
        add(stoi(id));
    }
}

void PostingList::add(int docId) {
    postings.emplace_back(new Posting(docId));
}

int PostingList::size() const{
    return postings.size();
}

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

PostingList PostingList::unionWith(const PostingList& secondList) const{
    PostingList result = PostingList();
    result.postings.insert(result.postings.end(), postings.begin(), postings.end());
    result.postings.insert(result.postings.end(), secondList.postings.begin(), secondList.postings.end());
    return result;
}

QueryResult PostingList::toQueryResult() const{
    QueryResult result = QueryResult();
    for (Posting* posting : postings){
        result.add(posting->getId());
    }
    return result;
}

string PostingList::to_string() const{
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


