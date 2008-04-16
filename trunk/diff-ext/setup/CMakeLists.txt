set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/build)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/common ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(i18n)
add_subdirectory(src)

