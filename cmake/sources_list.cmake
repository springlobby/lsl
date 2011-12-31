#----------------------------------------------------------------------------------------------------
# Source listing
#----------------------------------------------------------------------------------------------------

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/src )

SET(libSpringLobbySrc 
	src/libspringlobby.cpp
	src/container/userlist.cpp
	src/container/channellist.cpp
	src/container/battlelist.cpp
	src/user/user.cpp
	src/utils/crc.cpp
	src/networking/iserver.cpp
	src/networking/tasserver.cpp
	src/networking/commands.cpp
	src/networking/tasserverdataformats.cpp
	src/channel.cpp)
SET(libUnitsync++Src
	src/unitsync++/c_api.cpp
	src/unitsync++/image.cpp
	src/unitsync++/loader.cpp
	src/unitsync++/unitsync.cpp
	src/unitsync++/thread.cpp 
	)
SET(commonSrc
	src/utils/misc.cpp
	src/lslconfig.cpp 
	)
	
SET(basic_testSrc
	test/basic.cpp test/usync.cpp)
	
FILE( GLOB RECURSE header "src/*.h" )
FILE( GLOB RECURSE usyncheader "src/unitsync++/*.h" )
FILE( GLOB header2 "src/*/*.h" )

#grep -lR cc\"$ src/* | sed -e "s/\.h/\.cc/g"
SET( templatesources
	""
)
LIST( APPEND header ${header2} ${templatesources} )
set_source_files_properties(  ${header} PROPERTIES HEADER_FILE_ONLY 1 )

#cppcheck stuff
find_program( CPPCHECK_BINARY NAMES cppcheck )
IF( EXISTS ${CPPCHECK_BINARY} )
	ADD_CUSTOM_TARGET( cppcheck )
	FOREACH( list ${header} ${libSpringLobbySrc} )
		FOREACH( SOURCEFILE ${list} )
			LIST( APPEND CHECKPATHS ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCEFILE} )
		ENDFOREACH( SOURCEFILE )
	ENDFOREACH( list )

	ADD_CUSTOM_TARGET( cppcheck-full ${CPPCHECK_BINARY} "--xml" "--enable=all" "-f" "--quiet" "-j 3" 
		${CHECKPATHS} "2>${CMAKE_CURRENT_BINARY_DIR}/cppcheck-full.xml" "&&" 
			"${CMAKE_CURRENT_SOURCE_DIR}/tools/cppcheck-htmlreport" "--source-dir=${CMAKE_CURRENT_SOURCE_DIR}/src/" 
				"--file=${CMAKE_CURRENT_BINARY_DIR}/cppcheck-full.xml" "--report-dir=report-full" "--title=SpringLobby - Full")
	ADD_CUSTOM_TARGET( cppcheck-lite ${CPPCHECK_BINARY} "--xml" "-f" "--quiet" "-j 3" 
		${CHECKPATHS} "2>${CMAKE_CURRENT_BINARY_DIR}/cppcheck-lite.xml" "&&" 
			"${CMAKE_CURRENT_SOURCE_DIR}/tools/cppcheck-htmlreport" "--source-dir=${CMAKE_CURRENT_SOURCE_DIR}/src/" 
				"--file=${CMAKE_CURRENT_BINARY_DIR}/cppcheck-lite.xml" "--report-dir=report-lite" "--title=SpringLobby - Lite")
	add_dependencies( cppcheck cppcheck-full )
	add_dependencies( cppcheck cppcheck-lite )
ENDIF( EXISTS ${CPPCHECK_BINARY} )

#platform dependent compiles go here
IF( WIN32 )
	LIST( APPEND SLSharedWithSettings src/stacktrace.c )
ELSE( WIN32 )
	LIST( APPEND SLSharedWithSettings src/stacktrace.cpp )
ENDIF( WIN32 )
