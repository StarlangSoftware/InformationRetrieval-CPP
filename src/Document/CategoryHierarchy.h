//
// Created by Olcay Taner YILDIZ on 19.10.2022.
//

#ifndef INFORMATIONRETRIEVAL_CATEGORYHIERARCHY_H
#define INFORMATIONRETRIEVAL_CATEGORYHIERARCHY_H

#include <string>
#include <vector>

using namespace std;

class CategoryHierarchy {
private:
    vector<string> categoryList;
public:
    CategoryHierarchy() = default;
    explicit CategoryHierarchy(const string& list);
    string to_string();
};


#endif //INFORMATIONRETRIEVAL_CATEGORYHIERARCHY_H
