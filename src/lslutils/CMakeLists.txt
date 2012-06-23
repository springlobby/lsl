
SET(libSpringLobbyUtilsSrc
	"${CMAKE_CURRENT_SOURCE_DIR}/misc.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/lslconfig.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/crc.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/thread.cpp" 
	"${CMAKE_CURRENT_SOURCE_DIR}/net.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/globalsmanager.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/md5.c"
	)
	
FILE( GLOB RECURSE libSpringLobbyUtilsHeader "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )

#grep -lR cc\"$ src/* | sed -e "s/\.h/\.cc/g"
SET( templatesources
	""
)
LIST( APPEND libSpringLobbyUtilsHeader ${templatesources} )
set_source_files_properties(  ${libSpringLobbyUtilsHeader} PROPERTIES HEADER_FILE_ONLY 1 )
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ADD_LIBRARY(libSpringLobbyUtils STATIC ${libSpringLobbyHeader} ${libSpringLobbyUtilsSrc} )
TARGET_LINK_LIBRARIES(libSpringLobbyUtils boost_thread-mt)
