SET(libUnitsyncSrc
	"${CMAKE_CURRENT_SOURCE_DIR}/c_api.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/image.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/loader.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/thread.cpp" 
	"${CMAKE_CURRENT_SOURCE_DIR}/mmoptionmodel.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/optionswrapper.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/unitsync.cpp"
	)
FILE( GLOB RECURSE libUnitsyncHeader "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )

#grep -lR cc\"$ "${CMAKE_CURRENT_SOURCE_DIR}/* | sed -e "s/\.h/\.cc/g"
SET( templatesources
	""
)
LIST( APPEND libUnitsyncHeader ${templatesources} )
set_source_files_properties(  ${libUnitsyncHeader} PROPERTIES HEADER_FILE_ONLY 1 )
	
ADD_LIBRARY(libUnitsync++ STATIC ${libUnitsyncHeader} ${libUnitsyncSrc} )
TARGET_LINK_LIBRARIES(libUnitsync++ dl boost_filesystem boost_thread )

#cimage deps
FIND_PACKAGE( PNG REQUIRED)
ADD_DEFINITIONS( -Dcimg_use_png )
TARGET_LINK_LIBRARIES(libUnitsync++ ${PNG_LIBRARY} X11 )