project(activitydiary)

cmake_minimum_required(VERSION 2.6)
 
FIND_PACKAGE(KDE4 REQUIRED)

set (CMAKE_INSTALL_PREFIX /usr)

# Find the required Libaries
find_package(KDE4 REQUIRED)
find_package(Qt4 REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)
 
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   ${QT4_INCLUDES}
    
   )


add_subdirectory ( src )
