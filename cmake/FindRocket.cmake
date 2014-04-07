#
# Try to find GLEW library and include path.
# Once done this will define
#
# ROCKET_FOUND
# ROCKET_INCLUDE_PATH
# ROCKET_LIBRARY
# 

IF (MSVC)
    FIND_PATH( ROCKET_INCLUDE_PATH Rocket/
        ${DEPENDENCIES_PATH}/include/
    )

    FIND_LIBRARY( ROCKET_LIBRARY
        NAMES RocketCore RocketControls RocketDebugger
        PATHS
        ${DEPENDENCIES_PATH}/lib/win32/
    )

    foreach (CONFIGURATION_TYPE ${CMAKE_CONFIGURATION_TYPES})
        execute_process(COMMAND ${CMAKE_COMMAND}  -E  copy_if_different
            ${DEPENDENCIES_PATH}/bin/win/glew32.dll
            ${PROJECT_BINARY_DIR}/bin/${CONFIGURATION_TYPE}/glew32.dll
        )
    endforeach()
    

#ELSEIF(APPLE)
#   
#   SET(GLEW_INCLUDE_PATH 
#    ${DEPENDENCIES_PATH}/glew-1.10.0_OSX/include)
#    
#   SET(GLEW_LIBRARY
#   ${DEPENDENCIES_PATH}/glew-1.10.0_OSX/lib/libGLEW.a)

ELSE()
    FIND_PATH(ROCKET_INCLUDE_PATH Rocket/)
    
    FIND_LIBRARY(ROCKET_CORE_LIBRARY
        NAMES RocketCore
    PATH_SUFFIXES dynamic)

    FIND_LIBRARY(ROCKET_CONTROLS_LIBRARY
        NAMES RocketControls
    PATH_SUFFIXES dynamic)

    FIND_LIBRARY(ROCKET_DEBUGGER_LIBRARY
        NAMES RocketDebugger
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
