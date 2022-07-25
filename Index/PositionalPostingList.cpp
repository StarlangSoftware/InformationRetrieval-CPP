//
// Created by Olcay Taner YILDIZ on 25.07.2022.
//

#include <fstream>
#include <string>
#include "PositionalPostingList.h"
#include "Word.h"

PositionalPostingList::PositionalPostingList() = default;

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

int PositionalPostingList::size() {
    return postings.size();
}

int PositionalPostingList::getIndex(int docId) {
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

QueryResult PositionalPostingList::toQueryResult() {
    QueryResult result = QueryResult();
    for (PositionalPosting posting: postings){
        result.add(posting.getDocId());
    }
    return result;
}

void PositionalPostingList::add(int docId, int position) {
    int index = getIndex(docId);
    if (index == -1){
        postings.emplace_back(PositionalPosting(docId));
        postings[postings.size() - 1].add(position);
    } else {
        postings[index].add(position);
    }
}

PositionalPosting PositionalPostingList::get(int index) {
    return postings[index];
}

PositionalPostingList PositionalPostingList::unionWith(PositionalPostingList secondList) {
    PositionalPostingList result = PositionalPostingList();
    result.postings.insert(result.postings.end(), postings.begin(), postings.end());
    result.postings.insert(result.postings.end(), secondList.postings.begin(), secondList.postings.end());
    return result;
}

PositionalPostingList PositionalPostingList::intersection(PositionalPostingList secondList) {
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
}

string PositionalPostingList::to_string() {
    string result = "";
    for (PositionalPosting positionalPosting : postings){
        result += "\t" + positionalPosting.to_string() + "\n";
    }
    return result;
}

void PositionalPostingList::writeToFile(ofstream &outfile, int index) {
    if (size() > 0){
        outfile << ::to_string(index) << " " << ::to_string(size()) << "\n";
        outfile << to_string();
    }
}
