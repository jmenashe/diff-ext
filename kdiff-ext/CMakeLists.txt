project(kdiff-ext)

find_package(KDE4 REQUIRED)
find_package(Gettext REQUIRED)
include (KDE4Defaults)

set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "4")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${VERSION}")

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

link_directories(${KDE4_LIB_DIR})

include_directories(${CMAKE_CURRENT_BINARY_DIR}/settings ${CMAKE_CURRENT_BINARY_DIR} ${KDE4_INCLUDE_DIR} ${QT_INCLUDE_DIR})

add_subdirectory(settings)
add_subdirectory(diff-ext)
add_subdirectory(setup)
add_subdirectory(po)
add_subdirectory(doc)

########### install files ###############

install(FILES README COPYING AUTHORS ChangeLog INSTALL NEWS TODO DESTINATION share/doc/kdiff-ext)

include(CPack)
