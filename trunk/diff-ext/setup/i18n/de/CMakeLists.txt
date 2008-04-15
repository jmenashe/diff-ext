include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include)

set(diff-ext_sources diff_ext_setup.rc)

add_library(diff_ext_setup1031 SHARED ${diff-ext_sources})
