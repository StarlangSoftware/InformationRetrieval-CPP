//
// Created by Olcay Taner YILDIZ on 2.11.2022.
//

#include "CategoryNode.h"
#include <StringUtils.h>
#include "Term.h"

/**
 * Constructor for the category node. Each category is represented as a tree node in the category tree. Category
 * words are constructed by splitting the name of the category w.r.t. space. Sets the parent node and adds this
 * node as a child to parent node.
 * @param _name Name of the category.
 * @param _parent Parent node of this node.
 */
CategoryNode::CategoryNode(const string& _name, CategoryNode *_parent) {
    categoryWords = StringUtils::split(_name);
    parent = _parent;
    if (parent != nullptr){
        parent->addChild(this);
    }
}

/**
 * Adds the given child node to this node.
 * @param child New child node
 */
void CategoryNode::addChild(CategoryNode *child) {
    children.push_back(child);
}

/**
 * Constructs the category name from the category words. Basically combines all category words separated with space.
 * @return Category name.
 */
string CategoryNode::getName() const{
    string result = categoryWords[0];
    for (int i = 1; i < categoryWords.size(); i++){
        result += " " + categoryWords[i];
    }
    return result;
}

/**
 * Searches the children of this node for a specific category name.
 * @param childName Category name of the child.
 * @return The child with the given category name.
 */
CategoryNode *CategoryNode::getChild(const string& childName) const{
    for (CategoryNode* child : children){
        if (child->getName() == childName){
            return child;
        }
    }
    return nullptr;
}

/**
 * Adds frequency count of the term to the counts hash map of all ascendants of this node.
 * @param termId ID of the occurring term.
 * @param count Frequency of the term.
 */
void CategoryNode::addCounts(int termId, int count) {
    CategoryNode* current = this;
    while (current->parent != nullptr){
        current->counts.putNTimes(termId, count);
        current = current->parent;
    }
}

/**
 * Accessor of the children attribute
 * @return Children of the node
 */
vector<CategoryNode *> CategoryNode::getChildren() const{
    return children;
}

/**
 * Recursive method that returns the hierarchy string of the node. Hierarchy string is obtained by concatenating the
 * names of all ancestor nodes separated with '%'.
 * @return Hierarchy string of this node
 */
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

/**
 * Checks if the given node is an ancestor of the current node.
 * @param ancestor Node for which ancestor check will be done
 * @return True, if the given node is an ancestor of the current node.
 */
bool CategoryNode::isDescendant(CategoryNode *ancestor) const {
    if (this == ancestor){
        return true;
    }
    if (parent == nullptr){
        return false;
    }
    return parent->isDescendant(ancestor);
}

/**
 * Recursive method that sets the representative count. The representative count filters the most N frequent words.
 * @param representativeCount Number of representatives.
 */
void CategoryNode::setRepresentativeCount(int representativeCount) {
    vector<pair<int, int>> topList;
    if (representativeCount <= counts.size()){
        topList = counts.topN(representativeCount);
        for (pair<int, int> entry : topList) {
            counts.putNTimes(entry.first, entry.second);
        }
    }
}

/**
 * Recursive method that checks the query words in the category words of all descendants of this node and
 * accumulates the nodes that satisfies the condition. If any word  in the query appears in any category word, the
 * node will be accumulated.
 * @param query Query string
 * @param result Accumulator array
 */
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

/**
 * Recursive method that checks the query words in the category words of all descendants of this node and
 * accumulates the nodes that satisfies the condition. If any word  in the query appears in any category word, the
 * node will be accumulated.
 * @param query Query string
 * @param dictionary Term dictionary
 * @param result Accumulator array
 */
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
