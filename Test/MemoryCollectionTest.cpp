//
// Created by Olcay Taner YILDIZ on 27.07.2022.
//

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "../src/Document/Parameter.h"
#include "../src/Document/MemoryCollection.h"

TEST_CASE("testIncidenceMatrixSmall") {
    Parameter parameter = Parameter();
    parameter.setIndexType(IndexType::INCIDENCE_MATRIX);
    MemoryCollection collection = MemoryCollection("../../testCollection2", parameter);
    REQUIRE(2 == collection.size());
    REQUIRE(26 == collection.vocabularySize());
}

TEST_CASE("testIncidenceMatrixQuery") {
    Parameter parameter = Parameter();
    parameter.setIndexType(IndexType::INCIDENCE_MATRIX);
    MemoryCollection collection = MemoryCollection("../../testCollection2", parameter);
    Query query = Query("Brutus");
    SearchParameter searchParameter = SearchParameter();
    searchParameter.setRetrievalType(RetrievalType::BOOLEAN);
    QueryResult result = collection.searchCollection(query, searchParameter);
    REQUIRE(2 == result.getItems().size());
    query = Query("Brutus Caesar");
    result = collection.searchCollection(query, searchParameter);
    REQUIRE(2 == result.getItems().size());
    query = Query("enact");
    result = collection.searchCollection(query, searchParameter);
    REQUIRE(1 == result.getItems().size());
    query = Query("noble");
    result = collection.searchCollection(query, searchParameter);
    REQUIRE(1 == result.getItems().size());
    query = Query("a");
    result = collection.searchCollection(query, searchParameter);
    REQUIRE(0 == result.getItems().size());
}

TEST_CASE("testInvertedIndexBooleanQuery") {
    Parameter parameter = Parameter();
    parameter.setNGramIndex(true);
    MemoryCollection collection = MemoryCollection("../../testCollection2", parameter);
    Query query = Query("Brutus");
    SearchParameter searchParameter = SearchParameter();
    searchParameter.setRetrievalType(RetrievalType::BOOLEAN);
    QueryResult result = collection.searchCollection(query, searchParameter);
    REQUIRE(2 == result.getItems().size());
    query = Query("Brutus Caesar");
    result = collection.searchCollection(query, searchParameter);
    REQUIRE(2 == result.getItems().size());
    query = Query("enact");
    result = collection.searchCollection(query, searchParameter);
    REQUIRE(1 == result.getItems().size());
    query = Query("noble");
    result = collection.searchCollection(query, searchParameter);
    REQUIRE(1 == result.getItems().size());
    query = Query("a");
    result = collection.searchCollection(query, searchParameter);
    REQUIRE(0 == result.getItems().size());
}

TEST_CASE("testPositionalIndexBooleanQuery") {
    Parameter parameter = Parameter();
    parameter.setNGramIndex(true);
    MemoryCollection collection = MemoryCollection("../../testCollection2", parameter);
    Query query = Query("Julius Caesar");
    SearchParameter searchParameter = SearchParameter();
    searchParameter.setRetrievalType(RetrievalType::POSITIONAL);
    QueryResult result = collection.searchCollection(query, searchParameter);
    REQUIRE(2 == result.getItems().size());
    query = Query("I was killed");
    result = collection.searchCollection(query, searchParameter);
    REQUIRE(1 == result.getItems().size());
    query = Query("The noble Brutus");
    result = collection.searchCollection(query, searchParameter);
    REQUIRE(1 == result.getItems().size());
    query = Query("a");
    result = collection.searchCollection(query, searchParameter);
    REQUIRE(0 == result.getItems().size());
}

TEST_CASE("testPositionalIndexRankedQuery") {
    Parameter parameter = Parameter();
    parameter.setLoadIndexesFromFile(true);
    MemoryCollection collection = MemoryCollection("../../testCollection2", parameter);
    Query query = Query("Caesar");
    SearchParameter searchParameter = SearchParameter();
    searchParameter.setRetrievalType(RetrievalType::RANKED);
    searchParameter.setDocumentsRetrieved(2);
    QueryResult result = collection.searchCollection(query, searchParameter);
    REQUIRE(2 == result.getItems().size());
    REQUIRE(1 == result.getItems()[0].getDocId());
    query = Query("Caesar was killed");
    result = collection.searchCollection(query, searchParameter);
    REQUIRE(2 == result.getItems().size());
    REQUIRE(0 == result.getItems()[0].getDocId());
    query = Query("in the Capitol");
    result = collection.searchCollection(query, searchParameter);
    REQUIRE(1 == result.getItems().size());
}

TEST_CASE("testLoadIndexesFromFileSmall") {
    Parameter parameter = Parameter();
    parameter.setNGramIndex(true);
    parameter.setLoadIndexesFromFile(true);
    MemoryCollection collection = MemoryCollection("../../testCollection2", parameter);
    REQUIRE(2 == collection.size());
    REQUIRE(26 == collection.vocabularySize());
}

TEST_CASE("testLimitNumberOfDocumentsSmall") {
    Parameter parameter = Parameter();
    parameter.setNGramIndex(false);
    parameter.setLimitNumberOfDocumentsLoaded(true);
    parameter.setDocumentLimit(1);
    MemoryCollection collection = MemoryCollection("../../testCollection2", parameter);
    REQUIRE(1 == collection.size());
    REQUIRE(15 == collection.vocabularySize());
}

TEST_CASE("testCategoricalCollection") {
    Parameter parameter = Parameter();
    parameter.setDocumentType(DocumentType::CATEGORICAL);
    parameter.setLoadIndexesFromFile(true);
    parameter.setPhraseIndex(false);
    parameter.setNGramIndex(false);
    MemoryCollection collection = MemoryCollection("../../testCollection3", parameter);
    REQUIRE(1000 == collection.size());
    REQUIRE(2283 == collection.vocabularySize());
}

TEST_CASE("testAttributeQuery") {
    Parameter parameter = Parameter();
    parameter.setDocumentType(DocumentType::CATEGORICAL);
    parameter.setLoadIndexesFromFile(true);
    MemoryCollection collection = MemoryCollection("../../testCollection3", parameter);
    SearchParameter searchParameter = SearchParameter();
    searchParameter.setRetrievalType(RetrievalType::ATTRIBUTE);
    Query query = Query("Çift Yönlü");
    QueryResult result = collection.searchCollection(query, searchParameter);
    REQUIRE(10 == result.getItems().size());
    query = Query("Müzikli");
    result = collection.searchCollection(query, searchParameter);
    REQUIRE(4 == result.getItems().size());
    query = Query("Çift Yönlü Alüminyum Bebek Arabası");
    result = collection.searchCollection(query, searchParameter);
    REQUIRE(2 == result.getItems().size());
}

TEST_CASE("testCategoricalQuery") {
    Parameter parameter = Parameter();
    parameter.setDocumentType(DocumentType::CATEGORICAL);
    parameter.setLoadIndexesFromFile(true);
    MemoryCollection collection = MemoryCollection("../../testCollection3", parameter);
    SearchParameter searchParameter = SearchParameter();
    searchParameter.setFocusType(FocusType::CATEGORY);
    searchParameter.setRetrievalType(RetrievalType::BOOLEAN);
    Query query = Query("Çift Yönlü Bebek Arabası");
    QueryResult result = collection.searchCollection(query, searchParameter);
    REQUIRE(10 == result.getItems().size());
    searchParameter.setRetrievalType(RetrievalType::BOOLEAN);
    query = Query("Terlik");
    result = collection.searchCollection(query, searchParameter);
    REQUIRE(5 == result.getItems().size());
}

TEST_CASE("testAutoCompleteWord") {
    Parameter parameter = Parameter();
    parameter.setNGramIndex(true);
    parameter.setLoadIndexesFromFile(true);
    MemoryCollection collection = MemoryCollection("../../testCollection2", parameter);
    vector<string> autoCompleteList = collection.autoCompleteWord("kill");
    REQUIRE(1 == autoCompleteList.size());
}
