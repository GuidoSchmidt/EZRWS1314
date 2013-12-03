#
# Try to find ASSIMP library and include path.
# Once done this will define
#
# ASSIMP_FOUND
# ASSIMP_INCLUDE_PATH
# ASSIMP_LIBRARY
# 

IF (MINGW)
	FIND_PATH( ASSIMP_INCLUDE_PATH assimp/
		${DEPENDENCIES_PATH}/include/
	)

    FIND_LIBRARY( ASSIMP_LIBRARY
        NAMES assimp
        PATHS
        ${DEPENDENCIES_PATH}/lib/mingw/
    )

    execute_process(COMMAND ${CMAKE_COMMAND}  -E  copy_if_different
        ${DEPENDENCIES_PATH}/bin/mingw/libassimp.dll
        ${PROJECT_BINARY_DIR}/bin/
    )

ELSEIF (MSVC)
    FIND_PATH( ASSIMP_INCLUDE_PATH assimp/
        ${DEPENDENCIES_PATH}/include/
    )

    FIND_LIBRARY( ASSIMP_LIBRARY
        NAMES assimp
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
	FIND_PATH(ASSIMP_INCLUDE_PATH GLFW/glfw3.h)
	FIND_LIBRARY(ASSIMP_LIBRARY
        NAMES assimp
	PATH_SUFFIXES dynamic) 

ENDIF ()



SET(ASSIMP_FOUND "NO")
IF (ASSIMP_INCLUDE_PATH AND ASSIMP_LIBRARY)
	SET(ASSIMP_LIBRARIES ${ASSIMP_LIBRARY})
	SET(ASSIMP_FOUND "YES")
    message("EXTERNAL LIBRARY 'ASSIMP' FOUND")
    message("ASSIMP_LIBRARY: " ${ASSIMP_LIBRARY})
    message("ASSIMP_INCLUDE_PATH: " ${ASSIMP_INCLUDE_PATH})
ELSE()
    message("ERROR: EXTERNAL LIBRARY 'ASSIMP' NOT FOUND")
ENDIF (ASSIMP_INCLUDE_PATH AND ASSIMP_LIBRARY)