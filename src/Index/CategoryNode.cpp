//
// Created by Olcay Taner YILDIZ on 2.11.2022.
//

#include "CategoryNode.h"
#include "Term.h"

CategoryNode::CategoryNode(const string& _name, CategoryNode *_parent) {
    categoryWords = Word::split(_name);
    parent = _parent;
    if (parent != nullptr){
        parent->addChild(this);
    }
}

void CategoryNode::addChild(CategoryNode *child) {
    children.push_back(child);
}

string CategoryNode::getName() const{
    string result = categoryWords[0];
    for (int i = 1; i < categoryWords.size(); i++){
        result += " " + categoryWords[i];
    }
    return result;
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

string CategoryNode::to_string() const{
    if (parent != nullptr){
        if (parent->parent != nullptr){
            return parent->to_string() + "%" + getName();
        } else {
            return getName();
        }
    }
    return "";
}

bool CategoryNode::isDescendant(CategoryNode *ancestor) const {
    if (this == ancestor){
        return true;
    }
    if (parent == nullptr){
        return false;
    }
    return parent->isDescendant(ancestor);
}

void CategoryNode::setRepresentativeCount(int representativeCount) {
    vector<pair<int, int>> topList;
    if (representativeCount <= counts.size()){
        topList = counts.topN(representativeCount);
        for (pair<int, int> entry : topList) {
            counts.putNTimes(entry.first, entry.second);
        }
    }
}

void CategoryNode::getCategoriesWithKeyword(const Query &query, vector<CategoryNode *>& result){
    double categoryScore = 0;
    for (int i = 0; i < query.size(); i++){
        if (std::find(categoryWords.begin(), categoryWords.end(), query.getTerm(i).getName()) != categoryWords.end()){
            categoryScore++;
        }
    }
    if (categoryScore > 0){
        result.push_back(this);
    }
    for (CategoryNode* child : children){
        child->getCategoriesWithKeyword(query, result);
    }
}

void CategoryNode::getCategoriesWithCosine(const Query &query, TermDictionary* dictionary, vector<CategoryNode*>& result) {
    double categoryScore = 0;
    for (int i = 0; i < query.size(); i++){
        Term* term = (Term*) dictionary->getWord(query.getTerm(i).getName());
        if (term != nullptr){
            categoryScore += counts.count(term->getTermId());
        }
    }
    if (categoryScore > 0){
        result.push_back(this);
    }
    for (CategoryNode* child : children){
        child->getCategoriesWithCosine(query, dictionary, result);
    }
}
