include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include)

if(MSYS OR MINGW)
  set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} --language=407")
endif(MSYS OR MINGW)

set(diff-ext_sources diff_ext_setup.rc)

add_library(diff_ext_setup1031 SHARED ${diff-ext_sources})
