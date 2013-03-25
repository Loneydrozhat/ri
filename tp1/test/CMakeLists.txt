# CmakeLists in test

include_directories(${TP1_SOURCE_DIR}/lib/gtest/include)
include_directories(${TP1_SOURCE_DIR}/src)

add_executable(TermExtractorTest term_extractor_test)
target_link_libraries(TermExtractorTest indexer gtest gtest_main pthread htmlcxx)
add_test(TermExtractorTest TermExtractorTest)
