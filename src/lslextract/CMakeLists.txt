message(STATUS ${libUnitsyncSrc})

add_executable(lslextract
	lslextract.cpp
)
FIND_PACKAGE(PNG REQUIRED)
FIND_PACKAGE(X11 REQUIRED)

remove_definitions(-DHAVE_WX -D__WXDEBUG__ -D__WXGTK__ -DHAVE_SPRINGLOBBY=1 -DHAVE_CONFIG_H  -DHAVE_LIBNOTIFY)
TARGET_LINK_LIBRARIES(lslextract
	${Boost_LIBRARIES}
	${PNG_LIBRARY}
	${X11_LIBRARIES}
	${CMAKE_DL_LIBS}
	lsl-unitsync
	lsl-utils
)

target_include_directories(lslextract
	PRIVATE ${libSpringLobby_SOURCE_DIR}/src
)
