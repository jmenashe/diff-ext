include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/util/include ${PROJECT_SOURCE_DIR}/dialog-layout/include )

add_definitions(-D_WIN32_WINNT=0x0500)

set(sources
  about.c
  main.c
  diff_ext_setup.rc)

link_directories(${PROJECT_BINARY_DIR}/lib)

link_libraries(layout comdlg32 comctl32 advapi32 version ole32 shlwapi shlwapi util)

add_executable(diff_ext_setup WIN32 ${sources})
