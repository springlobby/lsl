SET(basic_testSrc
	${CMAKE_CURRENT_SOURCE_DIR}/basic.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/usync.cpp
	)

ADD_EXECUTABLE(libSpringLobby_test WIN32 MACOSX_BUNDLE ${basic_testSrc} )
ADD_EXECUTABLE(swig_test WIN32 MACOSX_BUNDLE ${CMAKE_CURRENT_SOURCE_DIR}/swig.cpp )

SET( SL_BOOST_LIBS boost_system boost_filesystem boost_thread )

INCLUDE_DIRECTORIES(${libSpringLobby_SOURCE_DIR}/src)
TARGET_LINK_LIBRARIES(libSpringLobby_test libSpringLobby libUnitsync++ )
