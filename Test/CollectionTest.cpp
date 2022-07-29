//
// Created by Olcay Taner YILDIZ on 27.07.2022.
//

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "../Document/Parameter.h"
#include "../Document/Collection.h"

TEST_CASE("testIncidenceMatrixSmall") {
    Parameter parameter = Parameter();
    parameter.setIndexType(IndexType::INCIDENCE_MATRIX);
    Collection collection = Collection("../testCollection2", parameter);
    REQUIRE(2 == collection.size());
    REQUIRE(26 == collection.vocabularySize());
}

TEST_CASE("testSaveIndexesToFileSmall") {
    Parameter parameter = Parameter();
    parameter.setNGramIndex(true);
    Collection collection = Collection("../testCollection2", parameter);
    collection.save();
}

TEST_CASE("testLoadIndexesFromFileSmall") {
    Parameter parameter = Parameter();
    parameter.setNGramIndex(true);
    parameter.setLoadIndexesFromFile(true);
    Collection collection = Collection("../testCollection2", parameter);
    REQUIRE(2 == collection.size());
    REQUIRE(26 == collection.vocabularySize());
}

TEST_CASE("testConstructIndexesInDiskSmall") {
    Parameter parameter = Parameter();
    parameter.setConstructIndexInDisk(true);
    parameter.setNGramIndex(false);
    parameter.setDocumentLimit(1);
    Collection collection = Collection("../testCollection2", parameter);
    REQUIRE(2 == collection.size());
    REQUIRE(26 == collection.vocabularySize());
}

TEST_CASE("testLimitNumberOfDocumentsSmall") {
    Parameter parameter = Parameter();
    parameter.setNGramIndex(false);
    parameter.setLimitNumberOfDocumentsLoaded(true);
    parameter.setDocumentLimit(1);
    Collection collection = Collection("../testCollection2", parameter);
    REQUIRE(1 == collection.size());
    REQUIRE(15 == collection.vocabularySize());
}

TEST_CASE("testConstructDictionaryAndIndexesInDiskSmall") {
    Parameter parameter = Parameter();
    parameter.setConstructDictionaryInDisk(true);
    parameter.setDocumentLimit(1);
    parameter.setWordLimit(10);
    Collection collection = Collection("../testCollection2", parameter);
}