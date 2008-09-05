include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
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
)


kde4_add_ui_files(map_SRCS
			sportsdiary.ui)

kde4_add_executable(activitydiary ${map_SRCS})

target_link_libraries(activitydiary
                      ${KDE4_KDEUI_LIBS})
 
#install(TARGETS mapview
#        DESTINATION ${KDE4_BIN_INSTALL_DIR})

