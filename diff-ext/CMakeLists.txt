project(diff_ext)

set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "9")
set(CPACK_PACKAGE_VERSION_PATCH "2")
set(VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${VERSION}")

cmake_minimum_required(VERSION 2.4)

if(UNICODE)
  add_definitions(-DUNICODE -D_UNICODE)
  set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} -DUNICODE -D_UNICODE")
  message("-- Building for Unicode")
else(UNICODE)
  add_definitions(-D_MBCS)
  message("-- Building for MBCS")
endif(UNICODE)

if(MSYS OR MINGW)
  find_program(CMAKE_RC_COMPILER NAMES windres PATHS ${COMPILER_LOCATION} DOC "Resource compiler")
  enable_language(RC)
  set(CMAKE_SHARED_LIBRARY_PREFIX "")
  
  set(CMAKE_RC_COMPILE_OBJECT 
    "<CMAKE_RC_COMPILER> -O coff <FLAGS>  -o <OBJECT> <SOURCE>")
  set(CMAKE_RC_CREATE_SHARED_LIBRARY 
    "<CMAKE_LINKER> <OBJECTS>  -o <TARGET>")
  set(CMAKE_C_CREATE_SHARED_LIBRARY
    "<CMAKE_C_COMPILER> <CMAKE_SHARED_LIBRARY_C_FLAGS> <LINK_FLAGS> <CMAKE_SHARED_LIBRARY_CREATE_C_FLAGS> -o <TARGET> ${CMAKE_GNULD_IMAGE_VERSION} <OBJECTS> <LINK_LIBRARIES>")
  set(CMAKE_CXX_CREATE_SHARED_LIBRARY
    "<CMAKE_CXX_COMPILER> <CMAKE_SHARED_LIBRARY_CXX_FLAGS> <LINK_FLAGS> <CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS> -o <TARGET> ${CMAKE_GNULD_IMAGE_VERSION} <OBJECTS> <LINK_LIBRARIES>")
else(MSYS OR MINGW)
  if(MSVC_VERSION GREATER 0)
    set(CMAKE_RC_CREATE_SHARED_LIBRARY 
      "<CMAKE_LINKER> /dll /noentry /out:<TARGET>  <OBJECTS>")
    #somehow its not getting set in Windows-cl.cmake
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
  endif(MSVC_VERSION GREATER 0)
endif(MSYS OR MINGW)

message("-- Check for InnoSetup v5 compiler")
find_program(ISCC iscc PATHS "c:/Program Files/Inno Setup 5")
if(ISCC)
  message(${ISCC})
else(ISCC)
  message(FATAL_ERROR "InnoSetup v5 not found")
endif(ISCC)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(dialog-layout)
add_subdirectory(util)
add_subdirectory(diff-ext)
add_subdirectory(setup)
add_subdirectory(install)

include(CPack)
