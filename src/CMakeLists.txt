include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
#   ${KDE4_INCLUDES}
   ${QT4_INCLUDES}
   ${QWT_INCLUDE_DIR}
   ${CMAKE_CURRENT_BINARY_DIR}
   )


#add_subdirectory(test)
 
set(map_SRCS 
	main.cpp
	mainwindow.cpp
	mapview.cpp	
        imageloader.cpp
        waypoint.cpp
        gpxparser.cpp
	Calc.cpp
	loader.cpp
	Track.cpp
	sportsdiary.cpp
        diagramcurve.cpp
        adxparser.cpp
        calendar.cpp
	Physical.cpp
	hrmparser.cpp
)

# for code coverage
#SET( CMAKE_CXX_FLAGS_COVERAGE "-fprofile-arcs -ftest-coverage" CACHE STRING "Profiling Compiler Flags" )

#kde4_add_ui_files(map_SRCS
#			activitydiary.ui)

set(activity_UIS
	activitydiary.ui)
QT4_WRAP_UI(activity_UIS_H ${activity_UIS})


# for the unit tests
#kde4_add_library(activitydiarylib SHARED ${map_SRCS})

SET(App_RCCS ../icons.qrc)
QT4_ADD_RESOURCES(App_RCCS_SRCS ${App_RCCS})

add_executable(activitydiary ${map_SRCS} ${activity_UIS_H} ${App_RCCS_SRCS})

target_link_libraries(activitydiary
                      ${QT4_LIBS} ${QWT_LIBRARY})
#target_link_libraries(activitydiarylib
#                      ${KDE4_KDEUI_LIBS} ${QWT_LIBRARY})

IF (UNIX AND NOT APPLE)
install(TARGETS activitydiary
        DESTINATION ${BIN_INSTALL_DIR})
     install(FILES activitydiary.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
ENDIF (UNIX AND NOT APPLE)

IF (APPLE)
     install (TARGETS activitydiary RUNTIME DESTINATION bin)
ENDIF (APPLE)
