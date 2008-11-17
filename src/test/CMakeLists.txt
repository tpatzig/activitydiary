FIND_PACKAGE(KDE4 REQUIRED)


include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${QT4_INCLUDES}
   ${KDE4_INCLUDES}
   )




SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

KDE4_ADD_UNIT_TEST(calendar_test test_calendar.cpp)
KDE4_ADD_UNIT_TEST(adxparser_test test_adxparser.cpp)
KDE4_ADD_UNIT_TEST(diagramcurve_test test_diagramcurve.cpp)

target_link_libraries(calendar_test ${KDE4_KDEUI_LIBS} activitydiarylib)
target_link_libraries(adxparser_test ${KDE4_KDEUI_LIBS} activitydiarylib)
target_link_libraries(diagramcurve_test ${KDE4_KDEUI_LIBS} activitydiarylib)


