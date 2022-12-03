//
// Created by Olcay Taner YILDIZ on 2.11.2022.
//

#ifndef INFORMATIONRETRIEVAL_CATEGORYNODE_H
#define INFORMATIONRETRIEVAL_CATEGORYNODE_H

#include <string>
#include <vector>
#include "CounterHashMap.h"
#include "TermDictionary.h"
#include "../Query/Query.h"

using namespace std;

class CategoryNode {
private:
    vector<CategoryNode*> children;
    CategoryNode* parent;
    CounterHashMap<int> counts;
    vector<string> categoryWords;
    void addChild(CategoryNode* child);
public:
    CategoryNode(const string& _name, CategoryNode* _parent);
    string getName() const;
    CategoryNode* getChild(const string& childName) const;
    void addCounts(int termId, int count);
    vector<CategoryNode*> getChildren() const;
    string to_string() const;
    bool isDescendant(CategoryNode* ancestor) const;
    void setRepresentativeCount(int representativeCount);
    void getCategoriesWithKeyword(const Query& query, vector<CategoryNode*>& result);
    void getCategoriesWithCosine(const Query& query, TermDictionary* dictionary, vector<CategoryNode*>& result);
};


#endif //INFORMATIONRETRIEVAL_CATEGORYNODE_H
