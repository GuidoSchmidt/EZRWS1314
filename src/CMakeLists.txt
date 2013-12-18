FILE (GLOB_RECURSE project_SRCS *.cpp *.cxx *.cc *.C *.c *.h *.hpp)
SET (project_MOC_HEADERS app.h)
SET (project_BIN ${PROJECT_NAME})

include_directories(
    ${OpenGL3_INCLUDE_PATH}
    ${GLEW_INCLUDE_PATH}
    ${GLFW3_INCLUDE_PATH}
    ${GLM_INCLUDE_PATH}
)

# Executables
ADD_EXECUTABLE(${project_BIN} ${project_SRCS} ${project_MOC_SRCS_GENERATED})

ADD_DEFINITIONS( -DRESOURCES_PATH="${RESOURCES_PATH}" )

SET_TARGET_PROPERTIES(${project_BIN} PROPERTIES VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}" OUTPUT_NAME ${project_BIN} CLEAN_DIRECT_OUTPUT 1)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

TARGET_LINK_LIBRARIES(
    ${project_BIN}

    ${GLFW3_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${OpenGL3_LIBRARIES}
    ${ASSIMP_LIBRARIES}
    ${SOIL_LIBRARIES}
    ${ROCKET_CORE_LIBRARY}
    ${ROCKET_CONTROLS_LIBRARY}
    ${ROCKET_DEBUGGER_LIBRARY}
)

IF("${CMAKE_SYSTEM}" MATCHES "Linux")
        TARGET_LINK_LIBRARIES(
            ${project_BIN} 
            X11
            Xxf86vm
            Xext
            Xmu
            Xrandr
            Xi
            pthread
        )
ENDIF()
