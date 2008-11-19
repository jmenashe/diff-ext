include_directories(${CMAKE_CURRENT_BINARY_DIR}/settings ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${KDE4_INCLUDE_DIR} ${QT_INCLUDE_DIR})


########### next target ###############

set(kdiff_ext_PART_SRCS
   kdiffext.cpp
)

kde4_add_plugin(kdiff_ext ${kdiff_ext_PART_SRCS})

target_link_libraries(kdiff_ext ${QT_AND_KDECORE_LIBS} konq settings kparts)

message("diff-ext prefix: ${CMAKE_INSTALL_PREFIX}")

# fix global install
#if(user_install)
  install(TARGETS kdiff_ext DESTINATION ${PLUGIN_INSTALL_DIR})
#else(user_install)
#  install(TARGETS kdiff_ext DESTINATION ${PLUGIN_INSTALL_DIR})
#endif(user_install)

########### install files ###############

install(FILES DESTINATION ${CMAKE_INSTALL_PREFIX}/share/services)
install(FILES kdiff-ext.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/services)

kde4_install_icons(hicolor)
