include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include)

set(diff-ext_sources diff_ext.rc)

add_library(diff_ext1041 SHARED ${diff-ext_sources})
