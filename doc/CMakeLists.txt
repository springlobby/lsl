if( NOT TARGET doc )
	add_custom_target( doc )
endif()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
set(DOXYFILE_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/doxygen" )
set(DOXYFILE_HTML_DIR "${CMAKE_CURRENT_BINARY_DIR}/doxygen/html" )
include(UseDoxygen)
find_program( XSLTPROC xsltproc )

function( target_exists TARGET_NAME RESULT_VAR )
GET_TARGET_PROPERTY(target_location ${TARGET_NAME} ${RESULT_VAR})
set( RESULT_VAR False PARENT_SCOPE)
if( ${RESULT_VAR} )
	set( RESULT_VAR True PARENT_SCOPE)
endif( ${RESULT_VAR} )
endfunction( target_exists )

GET_TARGET_PROPERTY(target_location target_name LOCATION)

if( EXISTS ${XSLTPROC} )
	add_custom_target( protocol 
		${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/protocol/ProtocolDescription_xml2html.xsl 
		${CMAKE_CURRENT_SOURCE_DIR}/protocol/ProtocolDescription.xml  > ${CMAKE_CURRENT_BINARY_DIR}/ProtocolDescription.html &&
		${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/protocol/index.xsl 
		${CMAKE_CURRENT_SOURCE_DIR}/protocol/ProtocolDescription.xml  > ${CMAKE_CURRENT_BINARY_DIR}/index.html 
		)
	add_dependencies( doc protocol )
endif( EXISTS ${XSLTPROC} )
