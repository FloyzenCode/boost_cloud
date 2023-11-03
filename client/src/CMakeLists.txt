file(GLOB SRC_FILES *.cpp)
add_library(src_lib ${SRC_FILES})
target_include_directories(src_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})