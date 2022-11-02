//
// Created by Olcay Taner YILDIZ on 2.11.2022.
//

#ifndef INFORMATIONRETRIEVAL_CATEGORYTREE_H
#define INFORMATIONRETRIEVAL_CATEGORYTREE_H


#include "CategoryNode.h"

class CategoryTree {
private:
    CategoryNode* root;
public:
    explicit CategoryTree(const string& rootName);
    CategoryNode* addCategoryHierarchy(const string& hierarchy);
    string topNString(TermDictionary& dictionary, int N) const;
};


#endif //INFORMATIONRETRIEVAL_CATEGORYTREE_H
