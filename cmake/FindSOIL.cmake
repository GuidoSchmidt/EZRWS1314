#
# Try to find ASSIMP library and include path.
# Once done this will define
#
# SOIL_FOUND
# SOIL_INCLUDE_PATH
# SOIL_LIBRARY
# 

IF (MINGW)
	FIND_PATH( SOIL_INCLUDE_PATH assimp/
		${DEPENDENCIES_PATH}/include/
	)

    FIND_LIBRARY( SOIL_LIBRARY
        NAMES assimp
        PATHS
        ${DEPENDENCIES_PATH}/lib/mingw/
    )

    execute_process(COMMAND ${CMAKE_COMMAND}  -E  copy_if_different
        ${DEPENDENCIES_PATH}/bin/mingw/libassimp.dll
        ${PROJECT_BINARY_DIR}/bin/
    )

ELSEIF (MSVC)
    FIND_PATH( SOIL_INCLUDE_PATH assimp/
        ${DEPENDENCIES_PATH}/include/
    )

    FIND_LIBRARY( SOIL_LIBRARY
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
#	FIND_PATH(SOIL_INCLUDE_PATH GLFW/glfw3.h
#	${DEPENDENCIES_PATH}/glfw-3.0.3_OSX/include)
#	
#	FIND_LIBRARY( GLFW3_LIBRARY
#        NAMES libglfw3.a
#  		PATHS ${DEPENDENCIES_PATH}/glfw-3.0.3_OSX/build/src)
#

ELSE()
	FIND_PATH(SOIL_INCLUDE_PATH SOIL/SOIL.h)
	FIND_LIBRARY(SOIL_LIBRARY
        NAMES SOIL
	PATH_SUFFIXES dynamic) 

ENDIF ()


SET(SOIL_FOUND "NO")
IF (SOIL_INCLUDE_PATH AND SOIL_LIBRARY)
	SET(SOIL_LIBRARY ${SOIL_LIBRARY})
	SET(SOIL_FOUND "YES")
    message("EXTERNAL LIBRARY 'SOIL' FOUND")
    message("SOIL_LIBRARY: " ${SOIL_LIBRARY})
    message("SOIL_INCLUDE_PATH: " ${SOIL_INCLUDE_PATH})
ELSE()
    message("ERROR: EXTERNAL LIBRARY 'SOIL' NOT FOUND")
ENDIF (SOIL_INCLUDE_PATH AND SOIL_LIBRARY)