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
    CategoryNode* addCategoryHierarchy(const string& hierarchy) const;
    vector<CategoryNode*> getCategories(const Query& query, TermDictionary* dictionary, CategoryDeterminationType categoryDeterminationType) const;
    void setRepresentativeCount(int representativeCount) const;
};


#endif //INFORMATIONRETRIEVAL_CATEGORYTREE_H
