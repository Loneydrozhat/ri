# CmakeLists.txt in src/ricplib

include_directories(${TP1_SOURCE_DIR}/lib/ricp/src)

add_library(indexer utils file_handler inverted_list indexer document_source collection_processor)

add_executable(indexp main)
target_link_libraries(indexp indexer htmlcxx RICPlib z)

add_executable(queryp query)
target_link_libraries(queryp indexer htmlcxx RICPlib z)
