//
// Created by Olcay Taner YILDIZ on 2.11.2022.
//

#ifndef INFORMATIONRETRIEVAL_CATEGORYNODE_H
#define INFORMATIONRETRIEVAL_CATEGORYNODE_H

#include <string>
#include <vector>
#include "CounterHashMap.h"
#include "TermDictionary.h"

using namespace std;

class CategoryNode {
private:
    string name;
    vector<CategoryNode*> children;
    CategoryNode* parent;
    CounterHashMap<int> counts;
    void addChild(CategoryNode* child);
public:
    CategoryNode(const string& _name, CategoryNode* _parent);
    string getName() const;
    CategoryNode* getChild(const string& childName) const;
    void addCounts(int termId, int count);
    vector<CategoryNode*> getChildren() const;
    vector<pair<int, int>> topN(int N) const;
    string topNString(TermDictionary& dictionary, int N) const;
    string to_string() const;
};


#endif //INFORMATIONRETRIEVAL_CATEGORYNODE_H
