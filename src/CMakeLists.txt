FILE (GLOB_RECURSE project_SRCS *.cpp *.cxx *.cc *.C *.c *.h *.hpp)
SET (project_MOC_HEADERS app.h)
SET (project_BIN ${PROJECT_NAME})

# Executables
ADD_EXECUTABLE(${project_BIN} ${project_SRCS} ${project_MOC_SRCS_GENERATED})

# Libraries
LINK_DIRECTORIES(${MAINFOLDER}/ext/lib/)
IF(WIN32)
    TARGET_LINK_LIBRARIES(${project_BIN} opengl32)
    TARGET_LINK_LIBRARIES(${project_BIN} ${MAINFOLDER}/ext/lib/win32/glew32.lib)
    TARGET_LINK_LIBRARIES(${project_BIN} ${MAINFOLDER}/ext/lib/win32/glfw3.lib)
    TARGET_LINK_LIBRARIES(${project_BIN} ${MAINFOLDER}/ext/lib/win32/assimp.lib)
ENDIF(WIN32)

IF(UNIX)
    TARGET_LINK_LIBRARIES(${project_BIN} X11)
    TARGET_LINK_LIBRARIES(${project_BIN} Xxf86vm)
    TARGET_LINK_LIBRARIES(${project_BIN} Xext)
    TARGET_LINK_LIBRARIES(${project_BIN} Xmu)
    TARGET_LINK_LIBRARIES(${project_BIN} Xrandr)
    TARGET_LINK_LIBRARIES(${project_BIN} Xi)
    TARGET_LINK_LIBRARIES(${project_BIN} GL)
    TARGET_LINK_LIBRARIES(${project_BIN} GLU)
    TARGET_LINK_LIBRARIES(${project_BIN} m)
    TARGET_LINK_LIBRARIES(${project_BIN} ${MAINFOLDER}/ext/lib/unix/libGLEW.a)
    TARGET_LINK_LIBRARIES(${project_BIN} ${MAINFOLDER}/ext/lib/unix/libglfw3.a)
ENDIF(UNIX)

SET_TARGET_PROPERTIES(${project_BIN} PROPERTIES VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}" OUTPUT_NAME ${project_BIN} CLEAN_DIRECT_OUTPUT 1)

INSTALL(TARGETS ${project_BIN} DESTINATION bin)
