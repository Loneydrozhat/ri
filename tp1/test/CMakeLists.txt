# CmakeLists in test

include_directories(${TP1_SOURCE_DIR}/lib/gmock-1.6.0/gtest/include)
include_directories(${TP1_SOURCE_DIR}/lib/gmock-1.6.0/include)
include_directories(${TP1_SOURCE_DIR}/lib/ricp/src)
include_directories(${TP1_SOURCE_DIR}/src)

add_executable(CollectionProcessorTest collection_processor_test)
target_link_libraries(CollectionProcessorTest indexer gmock gmock_main pthread htmlcxx RICPlib)
add_test(CollectionProcessor CollectionProcessorTest)

add_executable(UtilsTest utils_test)
target_link_libraries(UtilsTest indexer gmock gmock_main pthread htmlcxx RICPlib)
add_test(UtilsTest UtilsTest)
