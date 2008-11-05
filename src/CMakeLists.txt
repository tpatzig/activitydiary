include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   ${QWT_INCLUDE_DIR}
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
)


kde4_add_ui_files(map_SRCS
			activitydiary.ui)

kde4_add_executable(activitydiary ${map_SRCS})

target_link_libraries(activitydiary
                      ${KDE4_KDEUI_LIBS} ${QWT_LIBRARY})
 
#install(TARGETS mapview
#        DESTINATION ${KDE4_BIN_INSTALL_DIR})

