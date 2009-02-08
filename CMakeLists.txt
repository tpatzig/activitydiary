set(PACKAGE "activitydiary")
project(${PACKAGE})


include( ${CMAKE_SOURCE_DIR}/package/VERSION.cmake )
set ( VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}" )
set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})

set( CPACK_GENERATOR "TBZ2")
set( CPACK_SOURCE_GENERATOR "TBZ2")
set( CPACK_SOURCE_PACKAGE_FILE_NAME "${PACKAGE}-${VERSION}" )


# The following components are regex's to match anywhere (unless anchored)
# in absolute path + filename to find files or directories to be excluded
# from source tarball.
set (CPACK_SOURCE_IGNORE_FILES
"/CVS/;/.svn/;/.git/;/.libs/;/.deps/;.swp$;.#;/#;/build/"
"~$"
"\\\\.cvsignore$"
#"/package"
"BUILD_COVERAGE.sh"
"/src/coverage.sh"
"\\\\.bz2$"
"\\\\.gz$"
"Makefile\\\\.in$"
)

include(CPack)


cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")

 
set (CMAKE_INSTALL_PREFIX /usr)

# Find the required Libaries
#find_package(KDE4 REQUIRED)
find_package(Qt4 REQUIRED)
find_package(Qwt REQUIRED)
#include(KDE4Defaults)
#include(MacroLibrary)

#set(KDE4_BUILD_TESTS ON)
 
add_definitions (${QT_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
#   ${KDE4_INCLUDES}
   ${QT4_INCLUDES}
   ${QWT_INCLUDE_DIR}
    
   )


add_subdirectory ( src )
add_subdirectory ( icons )

message(STATUS "Writing spec file...")
configure_file(${CMAKE_SOURCE_DIR}/package/${PACKAGE}.spec.in ${CMAKE_BINARY_DIR}/package/${PACKAGE}.spec @ONLY)

#set( AUTOBUILD_COMMAND
#COMMAND ${CMAKE_MAKE_PROGRAM} package_source
#COMMAND ${CMAKE_COMMAND} -E copy ${CPACK_SOURCE_PACKAGE_FILE_NAME}.tar.bz2 ${CMAKE_BINARY_DIR}/package
#COMMAND ${CMAKE_COMMAND} -E remove ${CPACK_SOURCE_PACKAGE_FILE_NAME}.tar.bz2
#COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/package/${PACKAGE}.changes" "${CMAKE_BINARY_DIR}/package/${PACKAGE}.changes"
#)
