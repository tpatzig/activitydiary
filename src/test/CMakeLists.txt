FIND_PACKAGE(KDE4 REQUIRED)


include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${QT4_INCLUDES}
   ${KDE4_INCLUDES}
   )


#add_subdirectory(inputwizard)

set(test_cal_SRCS 
        test_calendar.cpp
)


#kde4_add_ui_files(kbluetray_SRCS
#                        deviceselector/deviceselector.ui)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
KDE4_ADD_UNIT_TEST(calendar_test ${test_cal_SRCS})

target_link_libraries(calendar_test
                      ${KDE4_KDEUI_LIBS} activitydiarylib)

#install(TARGETS kbluetooth4
#        DESTINATION ${KDE4_BIN_INSTALL_DIR})

