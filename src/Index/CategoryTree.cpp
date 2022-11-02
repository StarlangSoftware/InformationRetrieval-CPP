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

string CategoryTree::topNString(TermDictionary& dictionary, int N) const{
    deque<CategoryNode*> queue;
    queue.push_back(root);
    string result;
    while (!queue.empty()){
        CategoryNode* node = queue.front();
        queue.pop_front();
        if (node != root){
            result += node->topNString(dictionary, N) + "\n";
        }
        for (CategoryNode* child : node->getChildren()){
            queue.push_back(child);
        }
    }
    return result;
}
