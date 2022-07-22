cmake_minimum_required(VERSION 3.22)
project(InformationRetrieval)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)
link_directories(library)

add_library(InformationRetrieval Document/DocumentWeighting.h Document/IndexType.h Index/TermType.h Index/TermWeighting.h Query/RetrievalType.h Document/Parameter.cpp Document/Parameter.h Index/Term.cpp Index/Term.h Index/Posting.cpp Index/Posting.h Index/PostingSkip.cpp Index/PostingSkip.h Index/TermOccurrence.cpp Index/TermOccurrence.h Index/PositionalPosting.cpp Index/PositionalPosting.h Query/QueryResultItem.cpp Query/QueryResultItem.h Query/QuertResultItemComparator.h Query/QueryResult.cpp Query/QueryResult.h Query/Query.cpp Query/Query.h Document/DocumentText.cpp Document/DocumentText.h Document/Document.cpp Document/Document.h)
target_link_libraries(InformationRetrieval Dictionary MorphologicalDisambiguation MorphologicalAnalysis Corpus)
