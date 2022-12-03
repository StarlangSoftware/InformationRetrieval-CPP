//
// Created by Olcay Taner YILDIZ on 2.11.2022.
//

#include "CategoryTree.h"

CategoryTree::CategoryTree(const string &rootName) {
    root = new CategoryNode(rootName, nullptr);
}

CategoryNode *CategoryTree::addCategoryHierarchy(const string &hierarchy) {
    vector<string> categories = Word::split(hierarchy, "%");
    CategoryNode* current = root;
    for (const string& category : categories){
        CategoryNode* node = current->getChild(category);
        if (node == nullptr){
            node = new CategoryNode(category, current);
        }
        current = node;
    }
    return current;
}

vector<CategoryNode *> CategoryTree::getCategories(const Query &query, TermDictionary *dictionary,
                                                   CategoryDeterminationType categoryDeterminationType) {
    vector<CategoryNode*> result;
    switch (categoryDeterminationType) {
        case CategoryDeterminationType::KEYWORD:
            root->getCategoriesWithKeyword(query, result);
            break;
        case CategoryDeterminationType::COSINE:
            root->getCategoriesWithCosine(query, dictionary, result);
            break;
    }
    return result;
}

void CategoryTree::setRepresentativeCount(int representativeCount) {
    root->setRepresentativeCount(representativeCount);
}
