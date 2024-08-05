//
// Created by Olcay Taner YILDIZ on 3.12.2022.
//

#include "AbstractCollection.h"
using std::filesystem::directory_iterator;

/**
 * Constructor for the AbstractCollection class. All collections, disk, memory, large, medium are extended from this
 * basic class. Loads the attribute list from attribute file if required. Loads the names of the documents from
 * the document collection. If the collection is a categorical collection, also loads the category tree.
 * @param directory Directory where the document collection resides.
 * @param parameter Search parameter
 */
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

/**
 * Returns size of the document collection.
 * @return Size of the document collection.
 */
int AbstractCollection::size() const{
    return documents.size();
}

/**
 * Returns size of the term dictionary.
 * @return Size of the term dictionary.
 */
int AbstractCollection::vocabularySize() const{
    return dictionary.size();
}

/**
 * Constructs bi-gram and tri-gram indexes in memory.
 */
void AbstractCollection::constructNGramIndex() {
    vector<TermOccurrence> terms = dictionary.constructTermsFromDictionary(2);
    biGramDictionary = TermDictionary(terms);
    biGramIndex = NGramIndex(biGramDictionary, terms);
    terms = dictionary.constructTermsFromDictionary(3);
    triGramDictionary = TermDictionary(terms);
    triGramIndex = NGramIndex(triGramDictionary, terms);
}

/**
 * Loads the category tree for the categorical collections from category index file. Each line of the category index
 * file stores the index of the category and the category name with its hierarchy. Hierarchy string is obtained by
 * concatenating the names of all nodes in the path from root node to a leaf node separated with '%'.
 */
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
        if (items.size() > 1){
            documents[docId].setCategory(categoryTree, items[1]);
        }
    }
    inputFile.close();
}

/**
 * Loads the attribute list from attribute index file. Attributes are single or bi-word phrases representing the
 * important features of products in the collection. Each line of the attribute file contains either single or a two
 * word expression.
 */
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
