//
// Created by Olcay Taner YILDIZ on 2.11.2022.
//

#include "CategoryTree.h"
#include <StringUtils.h>

/**
 * Simple constructor of the tree. Sets the root node of the tree.
 * @param rootName Category name of the root node.
 */
CategoryTree::CategoryTree(const string &rootName) {
    root = new CategoryNode(rootName, nullptr);
}

/**
 * Adds a path (and if required nodes in the path) to the category tree according to the hierarchy string. Hierarchy
 * string is obtained by concatenating the names of all nodes in the path from root node to a leaf node separated
 * with '%'.
 * @param hierarchy Hierarchy string
 * @return The leaf node added when the hierarchy string is processed.
 */
CategoryNode *CategoryTree::addCategoryHierarchy(const string &hierarchy) const {
    vector<string> categories = StringUtils::split(hierarchy, "%");
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

/**
 * The method checks the query words in the category words of all nodes in the tree and returns the nodes that
 * satisfies the condition. If any word in the query appears in any category word, the node will be returned.
 * @param query Query string
 * @param dictionary Term dictionary
 * @param categoryDeterminationType Category determination type
 * @return The category nodes whose names contain at least one word from the query string
 */
vector<CategoryNode *> CategoryTree::getCategories(const Query &query, TermDictionary *dictionary,
                                                   CategoryDeterminationType categoryDeterminationType) const {
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

/**
 * The method sets the representative count. The representative count filters the most N frequent words.
 * @param representativeCount Number of representatives.
 */
void CategoryTree::setRepresentativeCount(int representativeCount) const {
    root->setRepresentativeCount(representativeCount);
}
