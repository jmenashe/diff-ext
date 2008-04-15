include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../include ${PROJECT_SOURCE_DIR}/util/include ${PROJECT_SOURCE_DIR}/dialog-layout/include )

add_definitions(-D_WIN32_WINNT=0x0500)

set(diff-ext_sources
  class_factory.cpp
  diff_ext.cpp
  icon_factory.cpp
  server.cpp
  resource.rc)

link_directories(${PROJECT_BINARY_DIR}/lib)

link_libraries(util uuid uuid ole32 gdi32 shlwapi)

add_library(diff_ext SHARED ${diff-ext_sources})
