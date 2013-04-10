# CmakeLists.txt in src/ricplib

include_directories(${TP1_SOURCE_DIR}/lib/ricp/src)

add_library(indexer utils file_handler inverted_list indexer document_source collection_processor)

add_executable(tp1 main)
target_link_libraries(tp1 indexer htmlcxx RICPlib z)
