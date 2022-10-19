//
// Created by Olcay Taner YILDIZ on 19.10.2022.
//

#include "CategoryHierarchy.h"
#include "Dictionary/Word.h"

CategoryHierarchy::CategoryHierarchy(const string& list) {
    this->categoryList = Word::split(list, "%");
}

string CategoryHierarchy::to_string() {
    string result = categoryList[0];
    for (int i = 1; i < categoryList.size(); i++){
        result += categoryList[i];
    }
    return result;
}
