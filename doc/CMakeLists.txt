set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(DOXYFILE_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/doxygen" )
set(DOXYFILE_HTML_DIR "${CMAKE_CURRENT_BINARY_DIR}/doxygen/html" )
include(UseDoxygen)
find_program( XSLTPROC xsltproc )
message( STATUS "${XSLTPROC}" )
#if( EXISTS ${XSLTPROC} )
	add_custom_target( protocol 
		${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/protocol/ProtocolDescription_xml2html.xsl 
		${CMAKE_CURRENT_SOURCE_DIR}/protocol/ProtocolDescription.xml  > ${CMAKE_CURRENT_BINARY_DIR}/ProtocolDescription.html &&
		${XSLTPROC} ${CMAKE_CURRENT_SOURCE_DIR}/protocol/index.xsl 
		${CMAKE_CURRENT_SOURCE_DIR}/protocol/ProtocolDescription.xml  > ${CMAKE_CURRENT_BINARY_DIR}/index.html 
		)
	add_dependencies( doc protocol )
	#endif( EXISTS ${XSLTPROC} )
