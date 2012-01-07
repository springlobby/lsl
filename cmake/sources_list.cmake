#----------------------------------------------------------------------------------------------------
# Source listing
#----------------------------------------------------------------------------------------------------

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
