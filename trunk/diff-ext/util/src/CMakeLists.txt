include_directories(${util_SOURCE_DIR}/include)

add_definitions(-D_WIN32_WINNT=0x0500)

set(util_sources
  file_sink.cpp
  icon.cpp
  log.cpp
  menu.cpp
  string.cpp
  trace.cpp
  resource_string.c
  hyperlink.c)

add_library(util STATIC ${util_sources})
