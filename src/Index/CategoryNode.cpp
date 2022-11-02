//
// Created by Olcay Taner YILDIZ on 2.11.2022.
//

#include "CategoryNode.h"

CategoryNode::CategoryNode(const string& _name, CategoryNode *_parent) {
    name = _name;
    parent = _parent;
    if (parent != nullptr){
        parent->addChild(this);
    }
}

void CategoryNode::addChild(CategoryNode *child) {
    children.push_back(child);
}

string CategoryNode::getName() const{
    return name;
}

CategoryNode *CategoryNode::getChild(const string& childName) const{
    for (CategoryNode* child : children){
        if (child->getName() == childName){
            return child;
        }
    }
    return nullptr;
}

void CategoryNode::addCounts(int termId, int count) {
    CategoryNode* current = this;
    while (current->parent != nullptr){
        current->counts.putNTimes(termId, count);
        current = current->parent;
    }
}

vector<CategoryNode *> CategoryNode::getChildren() const{
    return children;
}

vector<pair<int, int>> CategoryNode::topN(int N) const{
    if (N <= counts.size()){
        return counts.topN(N);
    } else {
        return counts.topN(counts.size());
    }
}

string CategoryNode::topNString(TermDictionary& dictionary, int N) const{
    vector<pair<int, int>> topN = this->topN(N);
    string result = to_string();
    for (pair<int, int> item : topN){
        if (!Word::isPunctuation(dictionary.getWord(item.first)->getName())){
            result += "\t" + dictionary.getWord(item.first)->getName() + " (" + std::to_string(item.second) + ")";
        }
    }
    return result;
}

string CategoryNode::to_string() const{
    if (parent != nullptr){
        if (parent->parent != nullptr){
            return parent->to_string() + "%" + name;
        } else {
            return name;
        }
    }
    return "";
}
