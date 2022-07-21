cmake_minimum_required(VERSION 3.22)
project(InformationRetrieval)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)
link_directories(library)

add_library(InformationRetrieval Document/DocumentWeighting.h Document/IndexType.h Index/TermType.h Index/TermWeighting.h Query/RetrievalType.h Document/Parameter.cpp Document/Parameter.h)
target_link_libraries(InformationRetrieval Dictionary MorphologicalDisambiguation MorphologicalAnalysis Corpus)
