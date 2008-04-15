if(MSYS OR MINGW)
  set(OUTPUT diff_ext-${VERSION}-i386)
else(MSYS OR MINGW)
  if(MSVC_VERSION GREATER 0)
    set(OUTPUT diff_ext-${VERSION}-$ENV{CPU})
  endif(MSVC_VERSION GREATER 0)
endif(MSYS OR MINGW)


configure_file(${CMAKE_CURRENT_SOURCE_DIR}/diff_ext.iss.cmake ${CMAKE_CURRENT_BINARY_DIR}/diff_ext.iss)

add_custom_command(
  OUTPUT 
    ${CMAKE_CURRENT_BINARY_DIR}/japanese.isl 
    ${CMAKE_CURRENT_BINARY_DIR}/ukrainian.isl 
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/japanese.isl ${CMAKE_CURRENT_BINARY_DIR}/japanese.isl 
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/ukrainian.isl ${CMAKE_CURRENT_BINARY_DIR}/ukrainian.isl
  DEPENDS 
    ${CMAKE_CURRENT_SOURCE_DIR}/japanese.isl
    ${CMAKE_CURRENT_SOURCE_DIR}/ukrainian.isl)
  
if(MSYS OR MINGW)
  STRING(REPLACE "/" "\\\\" bd ${CMAKE_BINARY_DIR})
  add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/${OUTPUT} 
    COMMAND ${ISCC} ARGS //O${bd} //F${OUTPUT} diff_ext.iss 
    DEPENDS 
      ${CMAKE_CURRENT_BINARY_DIR}/diff_ext.iss
      ${CMAKE_CURRENT_BINARY_DIR}/japanese.isl
      ${CMAKE_CURRENT_BINARY_DIR}/ukrainian.isl)
else(MSYS OR MINGW)
  if(MSVC_VERSION GREATER 0)
    add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/${OUTPUT} 
      COMMAND ${ISCC} ARGS /O${CMAKE_BINARY_DIR} /F${OUTPUT} diff_ext.iss 
      DEPENDS 
        ${CMAKE_CURRENT_BINARY_DIR}/diff_ext.iss
        ${CMAKE_CURRENT_BINARY_DIR}/japanese.isl
        ${CMAKE_CURRENT_BINARY_DIR}/ukrainian.isl)
  endif(MSVC_VERSION GREATER 0)
endif(MSYS OR MINGW)

add_custom_target(setup ALL echo DEPENDS ${CMAKE_BINARY_DIR}/${OUTPUT})
