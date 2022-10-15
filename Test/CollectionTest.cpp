//
// Created by Olcay Taner YILDIZ on 27.07.2022.
//

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "../src/Document/Parameter.h"
#include "../src/Document/Collection.h"

TEST_CASE("testIncidenceMatrixSmall") {
    Parameter parameter = Parameter();
    parameter.setIndexType(IndexType::INCIDENCE_MATRIX);
    Collection collection = Collection("../../testCollection2", parameter);
    REQUIRE(2 == collection.size());
    REQUIRE(26 == collection.vocabularySize());
}

TEST_CASE("testIncidenceMatrixQuery") {
    Parameter parameter = Parameter();
    parameter.setIndexType(IndexType::INCIDENCE_MATRIX);
    Collection collection = Collection("../../testCollection2", parameter);
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
    Collection collection = Collection("../../testCollection2", parameter);
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
    Collection collection = Collection("../../testCollection2", parameter);
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
    Collection collection = Collection("../../testCollection2", parameter);
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
    Collection collection = Collection("../../testCollection2", parameter);
    REQUIRE(2 == collection.size());
    REQUIRE(26 == collection.vocabularySize());
}

TEST_CASE("testLimitNumberOfDocumentsSmall") {
    Parameter parameter = Parameter();
    parameter.setNGramIndex(false);
    parameter.setLimitNumberOfDocumentsLoaded(true);
    parameter.setDocumentLimit(1);
    Collection collection = Collection("../../testCollection2", parameter);
    REQUIRE(1 == collection.size());
    REQUIRE(15 == collection.vocabularySize());
}
