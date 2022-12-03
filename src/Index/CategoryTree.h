//
// Created by Olcay Taner YILDIZ on 2.11.2022.
//

#ifndef INFORMATIONRETRIEVAL_CATEGORYTREE_H
#define INFORMATIONRETRIEVAL_CATEGORYTREE_H


#include "CategoryNode.h"
#include "../Query/CategoryDeterminationType.h"

class CategoryTree {
private:
    CategoryNode* root;
public:
    explicit CategoryTree(const string& rootName);
    CategoryNode* addCategoryHierarchy(const string& hierarchy);
    vector<CategoryNode*> getCategories(const Query& query, TermDictionary* dictionary, CategoryDeterminationType categoryDeterminationType);
    void setRepresentativeCount(int representativeCount);
};


#endif //INFORMATIONRETRIEVAL_CATEGORYTREE_H
