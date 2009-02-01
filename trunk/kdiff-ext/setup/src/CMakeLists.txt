include_directories(${CMAKE_CURRENT_BINARY_DIR}/settings ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${KDE3_INCLUDE_DIR} ${QT_INCLUDE_DIR} )


########### main target ###############

SET(kdiffextsetup_SRCS
   main.cpp
   diff.cpp
)

set(kdiffextsetup_UI
    base.ui
    general.ui
)

kde4_add_ui_files(kdiffextsetup_SRCS ${kdiffextsetup_UI})

kde4_add_executable(kdiffextsetup ${kdiffextsetup_SRCS})

target_link_libraries(kdiffextsetup ${QT_AND_KDECORE_LIBS} settings kio kdeui)

install(TARGETS kdiffextsetup DESTINATION bin)


########### install files ###############

install(FILES kdiffextsetupui.rc DESTINATION ${DATA_INSTALL_DIR}/kdiff-ext)
install(FILES kdiffextsetup.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})

kde4_install_icons(${ICON_INSTALL_DIR})

