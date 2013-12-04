#
# Try to find ROCKET library and include path.
# Once done this will define
#
# ROCKET_FOUND
# ROCKET_INCLUDE_PATH
# ROCKET_LIBRARY
# 

IF (MINGW)
	FIND_PATH( ROCKET_INCLUDE_PATH Rocket/
		${DEPENDENCIES_PATH}/include/
	)

    FIND_LIBRARY( ROCKET_LIBRARY
        NAMES RocketCore.dll RocketControls.dll RocketDebugger.dll
        PATHS
        ${DEPENDENCIES_PATH}/lib/mingw/
    )

    execute_process(COMMAND ${CMAKE_COMMAND}  -E  copy_if_different
        ${DEPENDENCIES_PATH}/bin/mingw/libassimp.dll
        ${PROJECT_BINARY_DIR}/bin/
    )

ELSEIF (MSVC)
    FIND_PATH( ROKCET_INCLUDE_PATH Rocket/
        ${DEPENDENCIES_PATH}/include/
    )

    FIND_LIBRARY( ROCKET_LIBRARY
        NAMES RocketCore RocketControls RocketDebugger
        PATHS
        ${DEPENDENCIES_PATH}/lib/win32/
    )

    foreach (CONFIGURATION_TYPE ${CMAKE_CONFIGURATION_TYPES})
        execute_process(COMMAND ${CMAKE_COMMAND}  -E  copy_if_different
            ${DEPENDENCIES_PATH}/bin/win/Assimp32.dll
            ${PROJECT_BINARY_DIR}/bin/${CONFIGURATION_TYPE}/
        )
    endforeach()

#ELSEIF(APPLE)
#
#	FIND_PATH(ASSIMP_INCLUDE_PATH GLFW/glfw3.h
#	${DEPENDENCIES_PATH}/glfw-3.0.3_OSX/include)
#	
#	FIND_LIBRARY( GLFW3_LIBRARY
#        NAMES libglfw3.a
#  		PATHS ${DEPENDENCIES_PATH}/glfw-3.0.3_OSX/build/src)
#

ELSE()
	FIND_PATH(ROCKET_INCLUDE_PATH Rocket/)
	FIND_LIBRARY(ROCKET_LIBRARY
        NAMES RocketCore RocketControls RocketDebugger
	PATH_SUFFIXES dynamic) 

ENDIF ()



SET(ROCKET_FOUND "NO")
IF (ROCKET_INCLUDE_PATH AND ROCKET_LIBRARY)
	SET(ROCKET_LIBRARIES ${ROCKET_LIBRARY})
	SET(ROCKET_FOUND "YES")
    message("EXTERNAL LIBRARY 'ROCKET' FOUND")
    message("ROCKET_LIBRARY: " ${ROCKET_LIBRARY})
    message("ROCKET_INCLUDE_PATH: " ${ROCKET_INCLUDE_PATH})
ELSE()
    message("ERROR: EXTERNAL LIBRARY 'ROCKET' NOT FOUND")
ENDIF (ROCKET_INCLUDE_PATH AND ROCKET_LIBRARY)