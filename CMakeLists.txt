PROJECT(libSpringLobby)

#set minimum cmake version
cmake_minimum_required(VERSION 2.6)
SET(CMAKE_COLOR_MAKEFILE ON)

# Add Definitions, Compiler-Switches, etc.: -Wall -O2 -g3 ...
# MSVC compiler (cl.exe) does not accept the same switches as gcc, although preprocessor definitions in the -D form will work for both
IF(NOT MSVC)
	IF( NOT MINGW )
		#wxMSW generates > 9000 warnings with -wextra...
		ADD_DEFINITIONS( -Wextra )
	ENDIF( NOT MINGW )
	ADD_DEFINITIONS( -Wall -Wno-strict-aliasing -pedantic )
ELSE(NOT MSVC)
	ADD_DEFINITIONS(-D_RC_MSVC)
ENDIF(NOT MSVC)

#----------------------------------------------------------------------------------------------------
# General Settings
#----------------------------------------------------------------------------------------------------
SET( CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true )
SET( CMAKE_FIND_LIBRARY_SUFFIXES ".so" ".lib" ".a" )

#----------------------------------------------------------------------------------------------------
# Load needed Modules
#----------------------------------------------------------------------------------------------------
if(NOT DEFINED LIBSPRINGLOBBY_REV)
  EXECUTE_PROCESS(COMMAND ${libSpringLobby_SOURCE_DIR}/tools/get-revision.sh
    OUTPUT_VARIABLE LIBSPRINGLOBBY_REV
    ERROR_VARIABLE GIT_ERROR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif(NOT DEFINED LIBSPRINGLOBBY_REV)

INCLUDE(cmake/package_config.cmake)
INCLUDE(cmake/sources_list.cmake)

# PKG-Config
FIND_PACKAGE( PkgConfig )

#if nothing went wrong we have the file and can define HAVE_CONFIG_H
IF ( EXISTS ${libSpringLobby_BINARY_DIR}/libSpringLobby_config.h  OR  EXISTS ${libSpringLobby_SOURCE_DIR}/libSpringLobby_config.h  )
    ADD_DEFINITIONS( -DHAVE_CONFIG_H )
    INCLUDE_DIRECTORIES(${libSpringLobby_BINARY_DIR})
ENDIF ( EXISTS ${libSpringLobby_BINARY_DIR}/libSpringLobby_config.h  OR  EXISTS ${libSpringLobby_SOURCE_DIR}/libSpringLobby_config.h  )
INCLUDE_DIRECTORIES(${libSpringLobby_SOURCE_DIR}/lib)

#----------------------------------------------------------------------------------------------------
# Options, that can be changed be the User in order to customise libSpringLobby
#----------------------------------------------------------------------------------------------------
SET( BUILD_SHARED_LIBS
	1 CACHE BOOL
	"Chooses whether to link dynamic or static libraries. Recommend keeping this activated unless you know what you're doing." )

SET( LIBSPRINGLOBBY_REV
	"${LIBSPRINGLOBBY_REV}" CACHE STRING
	"The revision string used in packages etc." )

IF(NOT CMAKE_CROSSCOMPILING)
ENDIF(NOT CMAKE_CROSSCOMPILING)


# If we build for windows Systems, we also include the Resource-File containing the Manifest, Icon and other Resources.
IF(WIN32)
	IF(MINGW OR CMAKE_CROSSCOMPILING)
		FIND_PROGRAM(WINDRES NAMES i586-pc-mingw32-windres windres i686-mingw32-windres DOC "path to mingw's windres executable")
		ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/sl_icon.o COMMAND ${WINDRES} -I${libSpringLobby_SOURCE_DIR}/src -I${wxWidgets_RC_DIR} -i${libSpringLobby_SOURCE_DIR}/src/libSpringLobby.rc -o ${CMAKE_CURRENT_BINARY_DIR}/sl_icon.o)
		SET(libSpringLobby_RC_FILE ${CMAKE_CURRENT_BINARY_DIR}/sl_icon.o )
	ENDIF(MINGW OR CMAKE_CROSSCOMPILING)

	SET(libSpringLobbySrc ${libSpringLobbySrc} src/libSpringLobby.rc)
ENDIF(WIN32)


#----------------------------------------------------------------------------------------------------
# Build target defintions
#----------------------------------------------------------------------------------------------------

# Here we define the executable libSpringLobby ( or on Windows libSpringLobby.exe )

#auto-remove whitespaces before/after lib paths
cmake_policy(SET CMP0004 OLD)
#ignore warnings about macosx app bundle output dir
cmake_policy(SET CMP0006 OLD)

ADD_LIBRARY(libSpringLobby STATIC ${header} ${libSpringLobbySrc} ${libSpringLobby_RC_FILE})
# ADD_LIBRARY(libUnitsync++ STATIC ${usyncheader} ${libUnitsync++Src} )
ADD_EXECUTABLE(libSpringLobby_test WIN32 MACOSX_BUNDLE ${basic_testSrc} )
ADD_EXECUTABLE(swig_test WIN32 MACOSX_BUNDLE ${CMAKE_CURRENT_SOURCE_DIR}/test/swig.cpp )

# Here the wxWidgets Libraries are added. These are set for us by the FIND Script. If you need other Libraries, you can add them here as well.
TARGET_LINK_LIBRARIES(libSpringLobby ${SL_BOOST_LIBS} )
# TARGET_LINK_LIBRARIES(libUnitsync++ dl )
TARGET_LINK_LIBRARIES(libSpringLobby_test dl)#(libUnitsync++)

IF(MINGW)
	TARGET_LINK_LIBRARIES( libSpringLobby iphlpapi )
ENDIF(MINGW)

IF (WIN32)
    install(TARGETS libSpringLobby LIBRARY DESTINATION .)
    install(FILES AUTHORS COPYING NEWS README THANKS  DESTINATION .)
	install(DIRECTORY ${CMAKE_INSTALL_PREFIX}/locale DESTINATION .)
ELSE (WIN32)
    install(TARGETS libSpringLobby 
		LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib 
		ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )
    install(FILES AUTHORS COPYING NEWS README THANKS DESTINATION ${CMAKE_INSTALL_PREFIX}/share/doc/libSpringLobby )
ENDIF (WIN32)

INCLUDE(cmake/compat_flags.cmake)

add_custom_target(pack
					${CMAKE_MAKE_PROGRAM} package
					&& ${libSpringLobby_SOURCE_DIR}/tools/transform_release_zip.sh
						${CMAKE_BINARY_DIR}/${PACKAGE_NAME}-${PACKAGE_VERSION}-win32.zip
						${CMAKE_STRIP}
				)

# setup make dist
add_custom_target(dist ${CMAKE_SOURCE_DIR}/tools/make_dist.sh "${libSpringLobby_BINARY_DIR}" "${PACKAGE_NAME}-${PACKAGE_VERSION}"
					 )

# setup make distcheck
add_custom_target(distcheck + cd ${CMAKE_BINARY_DIR} &&
                        rm -rf ${PACKAGE_NAME}-${PACKAGE_VERSION} &&
                        tar -xzf ${PACKAGE_NAME}-${PACKAGE_VERSION}.tar.gz &&
                        cd ${PACKAGE_NAME}-${PACKAGE_VERSION}/ &&
                        cmake "-DCMAKE_CXX_FLAGS:STRING=-O0". && ${CMAKE_MAKE_PROGRAM}
					)
add_custom_target( version
				${libSpringLobby_SOURCE_DIR}/tools/regen_config_header.sh ${libSpringLobby_SOURCE_DIR}/cmake/config.h ${libSpringLobby_BINARY_DIR}/libSpringLobby_config.h
				)
add_dependencies( distcheck dist )
add_dependencies( libSpringLobby version )

