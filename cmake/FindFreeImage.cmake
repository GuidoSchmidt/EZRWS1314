#
# Try to find GLEW library and include path.
# Once done this will define
#
# GLEW_FOUND
# GLEW_INCLUDE_PATH
# GLEW_LIBRARY
# 

IF (MINGW)
	FIND_PATH( GLEW_INCLUDE_PATH GL/glew.h
		${DEPENDENCIES_PATH}/include/
	)

    FIND_LIBRARY( GLEW_LIBRARY
        NAMES glew32
        PATHS
        ${DEPENDENCIES_PATH}/lib/mingw/
    )

    execute_process(COMMAND ${CMAKE_COMMAND}  -E  copy_if_different
        ${DEPENDENCIES_PATH}/bin/mingw/glew32.dll
        ${PROJECT_BINARY_DIR}/bin/glew32.dll
    )

ELSEIF (MSVC)
    FIND_PATH( FREEIMAGE_INCLUDE_PATH FreeImage/FreeImage.h
        ${DEPENDENCIES_PATH}/include
    )

    FIND_LIBRARY( FREEIMAGE_LIBRARY
        NAMES FreeImage
        PATHS
        ${DEPENDENCIES_PATH}/lib/win32/
    )

    foreach (CONFIGURATION_TYPE ${CMAKE_CONFIGURATION_TYPES})
        execute_process(COMMAND ${CMAKE_COMMAND}  -E  copy_if_different
            ${DEPENDENCIES_PATH}/bin/win/FreeImage.dll
            ${PROJECT_BINARY_DIR}/bin/${CONFIGURATION_TYPE}
        )
    endforeach()
    

#ELSEIF(APPLE)
#	
#	SET(GLEW_INCLUDE_PATH 
#	 ${DEPENDENCIES_PATH}/glew-1.10.0_OSX/include)
#	 
#	SET(GLEW_LIBRARY
#	${DEPENDENCIES_PATH}/glew-1.10.0_OSX/lib/libGLEW.a)

ELSE()
    FIND_PATH(FREEIMAGE_INCLUDE_PATH FreeImage.h)
	FIND_LIBRARY(FREEIMAGE_LIBRARY
	NAMES freeimage PATH_SUFFIXES lib64)

ENDIF()
    

SET(FREEIMAGE_FOUND "NO")
IF (FREEIMAGE_INCLUDE_PATH AND FREEIMAGE_LIBRARY)
	SET(FREEIMAGE_LIBRARIES ${FREEIMAGE_LIBRARY})
	SET(FREEIMAGE_FOUND "YES")
    message("EXTERNAL LIBRARY 'FreeImage' FOUND")
    message("FREEIMAGE_LIBRARY: " ${FREEIMAGE_LIBRARY})
    message("FREEIMAGE_INCLUDE_PATH: " ${FREEIMAGE_INCLUDE_PATH})
ELSE()
    message("ERROR: EXTERNAL LIBRARY 'FREEIMAGE' NOT FOUND")
ENDIF (FREEIMAGE_INCLUDE_PATH AND FREEIMAGE_LIBRARY)
