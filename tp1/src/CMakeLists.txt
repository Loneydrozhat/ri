# CmakeLists.txt in src/ricplib

include_directories(${TP1_SOURCE_DIR}/lib/ricp/src)

add_library(indexer collection_processer.h term_extractor.h term_extractor.cc)

add_executable(tp1 main)
target_link_libraries(tp1 indexer RICPlib z)
