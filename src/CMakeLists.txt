include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   ${QWT_INCLUDE_DIR}
   )


add_subdirectory(test)
 
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
)

# for code coverage
# SET( CMAKE_CXX_FLAGS_COVERAGE "-fprofile-arcs -ftest-coverage" CACHE STRING "Profiling Compiler Flags" )

kde4_add_ui_files(map_SRCS
			activitydiary.ui)

kde4_add_executable(activitydiary ${map_SRCS})

# for the unit tests
kde4_add_library(activitydiarylib SHARED ${map_SRCS})

target_link_libraries(activitydiary
                      ${KDE4_KDEUI_LIBS} ${QWT_LIBRARY})
target_link_libraries(activitydiarylib
                      ${KDE4_KDEUI_LIBS} ${QWT_LIBRARY})

install(TARGETS activitydiary
        DESTINATION /opt/activitydiary)

install(FILES activitydiary.sh
        DESTINATION /opt/activitydiary)

