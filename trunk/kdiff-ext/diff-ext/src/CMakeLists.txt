include_directories(${CMAKE_CURRENT_BINARY_DIR}/settings ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${KDE4_INCLUDE_DIR} ${QT_INCLUDE_DIR})


########### diff-ext target ###############

set(kdiff_ext_PART_SRCS
   kdiffext.cpp
)

kde4_add_plugin(kdiff_ext ${kdiff_ext_PART_SRCS})

target_link_libraries(kdiff_ext ${QT_AND_KDECORE_LIBS} konq settings kparts)

install(TARGETS kdiff_ext DESTINATION ${PLUGIN_INSTALL_DIR})

########### install files ###############

install(FILES kdiff-ext.desktop DESTINATION ${SERVICES_INSTALL_DIR})

kde4_install_icons(hicolor)
