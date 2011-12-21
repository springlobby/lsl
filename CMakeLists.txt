PROJECT(liblobby)

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
if(NOT DEFINED SPRINGLOBBY_REV)
  EXECUTE_PROCESS(COMMAND ${springlobby_SOURCE_DIR}/tools/get-revision.sh
    OUTPUT_VARIABLE SPRINGLOBBY_REV
    ERROR_VARIABLE GIT_ERROR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif(NOT DEFINED SPRINGLOBBY_REV)

INCLUDE(cmake/package_config.cmake)
INCLUDE(cmake/sources.list)

# PKG-Config
FIND_PACKAGE( PkgConfig )

#if nothing went wrong we have the file and can define HAVE_CONFIG_H
IF ( EXISTS ${springlobby_BINARY_DIR}/springlobby_config.h  OR  EXISTS ${springlobby_SOURCE_DIR}/springlobby_config.h  )
    ADD_DEFINITIONS( -DHAVE_CONFIG_H )
    INCLUDE_DIRECTORIES(${springlobby_BINARY_DIR})
ENDIF ( EXISTS ${springlobby_BINARY_DIR}/springlobby_config.h  OR  EXISTS ${springlobby_SOURCE_DIR}/springlobby_config.h  )


#----------------------------------------------------------------------------------------------------
# Options, that can be changed be the User in order to customise springlobby
#----------------------------------------------------------------------------------------------------
SET( BUILD_SHARED_LIBS
	1 CACHE BOOL
	"Chooses whether to link dynamic or static libraries. Recommend keeping this activated unless you know what you're doing." )

SET( SPRINGLOBBY_REV
	"${SPRINGLOBBY_REV}" CACHE STRING
	"The revision string used in packages etc." )

IF(NOT CMAKE_CROSSCOMPILING)
ENDIF(NOT CMAKE_CROSSCOMPILING)


# If we build for windows Systems, we also include the Resource-File containing the Manifest, Icon and other Resources.
IF(WIN32)
	IF(MINGW OR CMAKE_CROSSCOMPILING)
		FIND_PROGRAM(WINDRES NAMES i586-pc-mingw32-windres windres i686-mingw32-windres DOC "path to mingw's windres executable")
		ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/sl_icon.o COMMAND ${WINDRES} -I${springlobby_SOURCE_DIR}/src -I${wxWidgets_RC_DIR} -i${springlobby_SOURCE_DIR}/src/springlobby.rc -o ${CMAKE_CURRENT_BINARY_DIR}/sl_icon.o)
		SET(springlobby_RC_FILE ${CMAKE_CURRENT_BINARY_DIR}/sl_icon.o )
	ENDIF(MINGW OR CMAKE_CROSSCOMPILING)

	SET(springlobbySrc ${springlobbySrc} src/springlobby.rc)
ELSE(WIN32)
	# If we don't build for windows, include some source code checks as pre-build step.
    ADD_CUSTOM_TARGET(test_susynclib ALL COMMAND tools/test-susynclib.awk src/springunitsynclib.cpp WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
ENDIF(WIN32)


#----------------------------------------------------------------------------------------------------
# Build target defintions
#----------------------------------------------------------------------------------------------------

# Here we define the executable springlobby ( or on Windows springlobby.exe )

#auto-remove whitespaces before/after lib paths
cmake_policy(SET CMP0004 OLD)
#ignore warnings about macosx app bundle output dir
cmake_policy(SET CMP0006 OLD)

ADD_LIBRARY(springlobby WIN32 MACOSX_BUNDLE ${header} ${springlobbySrc} ${SettingsSrc} ${SLSharedWithSettings} ${springlobby_RC_FILE})
ADD_EXECUTABLE(springlobby_test WIN32 MACOSX_BUNDLE  )
#the updater needs to be build for windows only
IF(WIN32)
       ADD_EXECUTABLE(springlobby_updater WIN32 MACOSX_BUNDLE ${updaterSrc} ${SLSharedWithSettings} ${updater_RC_FILE} )
       TARGET_LINK_LIBRARIES(springlobby_updater ${wxWidgets_LIBRARIES} )
ENDIF(WIN32)

#only enable if debugging is necessary
#ADD_EXECUTABLE(versiontest WIN32 MACOSX_BUNDLE src/versiontest.cpp )

# Here the wxWidgets Libraries are added. These are set for us by the FIND Script. If you need other Libraries, you can add them here as well.
TARGET_LINK_LIBRARIES(springlobby ${SL_BOOST_LIBS} )
TARGET_LINK_LIBRARIES(springlobby_test springlobby )

IF(MINGW)
	TARGET_LINK_LIBRARIES( springlobby iphlpapi )
ENDIF(MINGW)

IF (WIN32)
    install(TARGETS springlobby springlobby_updater springsettings RUNTIME DESTINATION .)
    install(FILES AUTHORS COPYING NEWS README THANKS  DESTINATION .)
	install(DIRECTORY ${CMAKE_INSTALL_PREFIX}/locale DESTINATION .)
ELSE (WIN32)
    install(TARGETS springlobby springsettings RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
    install(FILES AUTHORS COPYING NEWS README THANKS DESTINATION ${CMAKE_INSTALL_PREFIX}/share/doc/springlobby )
    install(FILES src/images/springlobby.svg DESTINATION
	    ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/scalable/apps)
    install(FILES src/springlobby.desktop DESTINATION
	    ${CMAKE_INSTALL_PREFIX}/share/applications)
ENDIF (WIN32)

INCLUDE(cmake/compat_flags.cmake)

add_custom_target(pack
					${CMAKE_MAKE_PROGRAM} package
					&& ${springlobby_SOURCE_DIR}/tools/transform_release_zip.sh
						${CMAKE_BINARY_DIR}/${PACKAGE_NAME}-${PACKAGE_VERSION}-win32.zip
						${CMAKE_STRIP}
				)

# setup make dist
add_custom_target(dist ${CMAKE_SOURCE_DIR}/tools/make_dist.sh "${springlobby_BINARY_DIR}" "${PACKAGE_NAME}-${PACKAGE_VERSION}"
					 )

# setup make distcheck
add_custom_target(distcheck + cd ${CMAKE_BINARY_DIR} &&
                        rm -rf ${PACKAGE_NAME}-${PACKAGE_VERSION} &&
                        tar -xzf ${PACKAGE_NAME}-${PACKAGE_VERSION}.tar.gz &&
                        cd ${PACKAGE_NAME}-${PACKAGE_VERSION}/ &&
                        cmake "-DCMAKE_CXX_FLAGS:STRING=-O0". && ${CMAKE_MAKE_PROGRAM}
					)
add_custom_target( version
				${springlobby_SOURCE_DIR}/tools/regen_config_header.sh ${springlobby_SOURCE_DIR}/cmake/config.h ${springlobby_BINARY_DIR}/springlobby_config.h
				)
add_dependencies( distcheck dist )
add_dependencies( springlobby alure )
add_dependencies( springlobby version )
#add_dependencies( versiontest version )

