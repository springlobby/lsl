#if nothing went wrong we have the file and can define HAVE_CONFIG_H
IF ( EXISTS ${libSpringLobby_BINARY_DIR}/libSpringLobby_config.h  OR  EXISTS ${libSpringLobby_SOURCE_DIR}/libSpringLobby_config.h  )
    ADD_DEFINITIONS( -DHAVE_CONFIG_H )
    INCLUDE_DIRECTORIES(${libSpringLobby_BINARY_DIR})
ENDIF ( EXISTS ${libSpringLobby_BINARY_DIR}/libSpringLobby_config.h  OR  EXISTS ${libSpringLobby_SOURCE_DIR}/libSpringLobby_config.h  )
INCLUDE_DIRECTORIES(${libSpringLobby_SOURCE_DIR}/lib)

add_subdirectory( utils )
add_subdirectory( lsl )
add_subdirectory( unitsync++ )