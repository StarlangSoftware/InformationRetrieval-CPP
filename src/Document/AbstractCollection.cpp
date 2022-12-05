//
// Created by Olcay Taner YILDIZ on 3.12.2022.
//

#include "AbstractCollection.h"
using std::filesystem::directory_iterator;

AbstractCollection::AbstractCollection(const string &directory, const Parameter &parameter) {
    name = directory;
    this->parameter = parameter;
    loadAttributeList();
    int j = 0;
    for (const auto & file : directory_iterator(directory)){
        if (!file.is_directory() && file.path().string().ends_with(".txt")){
            if (!parameter.limitNumberOfDocumentsLoaded() || j < parameter.getDocumentLimit()){
                Document document = Document(parameter.getDocumentType(), file.path(), file.path(), j);
                documents.emplace_back(document);
                j++;
            }
        }
    }
    if (parameter.getDocumentType() == DocumentType::CATEGORICAL){
        loadCategories();
    }
}

int AbstractCollection::size() const{
    return documents.size();
}

int AbstractCollection::vocabularySize() const{
    return dictionary.size();
}

void AbstractCollection::constructNGramIndex() {
    vector<TermOccurrence> terms = dictionary.constructTermsFromDictionary(2);
    biGramDictionary = TermDictionary(terms);
    biGramIndex = NGramIndex(biGramDictionary, terms);
    terms = dictionary.constructTermsFromDictionary(3);
    triGramDictionary = TermDictionary(terms);
    triGramIndex = NGramIndex(triGramDictionary, terms);
}

void AbstractCollection::loadCategories() {
    ifstream inputFile;
    string line;
    categoryTree = new CategoryTree(name);
    inputFile.open(name + "-categories.txt", ifstream :: in);
    while (inputFile.good()) {
        getline(inputFile, line);
        if (line.empty()){
            continue;
        }
        vector<string> items = Word::split(line, "\t");
        int docId = stoi(items.at(0));
        documents[docId].setCategory(categoryTree, items[1]);
    }
    inputFile.close();
}

void AbstractCollection::loadAttributeList() {
    ifstream inputFile;
    string line;
    categoryTree = new CategoryTree(name);
    inputFile.open(name + "-attributelist.txt", ifstream :: in);
    while (inputFile.good()) {
        getline(inputFile, line);
        if (line.empty()){
            continue;
        }
        attributeList.insert(line);
    }
    inputFile.close();
}
