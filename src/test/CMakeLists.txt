FIND_PACKAGE(KDE4 REQUIRED)


include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${QT4_INCLUDES}
   ${KDE4_INCLUDES}
   )


#add_subdirectory(inputwizard)

set(gpx_SRCS 
        main.cpp
        ../waypoint.cpp
        ../imageloader.cpp
        ../gpxparser.cpp
)


#kde4_add_ui_files(kbluetray_SRCS
#                        deviceselector/deviceselector.ui)

add_executable(parser_test ${gpx_SRCS})

target_link_libraries(parser_test
                      ${KDE4_KDEUI_LIBS})

#install(TARGETS kbluetooth4
#        DESTINATION ${KDE4_BIN_INSTALL_DIR})

