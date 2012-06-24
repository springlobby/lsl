#don't write when git errored out resulting in unset version (ie when compiling from tarball)
IF ( NOT GIT_ERROR )
    CONFIGURE_FILE( ${libSpringLobby_SOURCE_DIR}/cmake/config.h ${libSpringLobby_BINARY_DIR}/libSpringLobby_config.h )
ENDIF ( NOT GIT_ERROR )
#if nothing went wrong we have the file and can define HAVE_CONFIG_H
IF ( EXISTS ${libSpringLobby_BINARY_DIR}/libSpringLobby_config.h  OR  EXISTS ${libSpringLobby_SOURCE_DIR}/libSpringLobby_config.h  )
    ADD_DEFINITIONS( -DHAVE_CONFIG_H )
    INCLUDE_DIRECTORIES(${libSpringLobby_BINARY_DIR})
ENDIF ( EXISTS ${libSpringLobby_BINARY_DIR}/libSpringLobby_config.h  OR  EXISTS ${libSpringLobby_SOURCE_DIR}/libSpringLobby_config.h  )

INCLUDE_DIRECTORIES(${libSpringLobby_SOURCE_DIR}/lib)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory( lslutils )
add_subdirectory( lsl )
add_subdirectory( unitsync++ )