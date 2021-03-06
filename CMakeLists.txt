CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

# *** CMake Variables ***
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake CACHE PATH "Project specific path. Set manually if it was not found.")
set(DEPENDENCIES_PATH ${CMAKE_SOURCE_DIR}/ext CACHE PATH "Project specific path. Set manually if it was not found.")
set(RESOURCES_PATH ${CMAKE_SOURCE_DIR}/resources CACHE PATH "Project specific path. Set manually if it was not found.")
set(SHADER_SRC_PATH ${CMAKE_SOURCE_DIR}/resources/shader_source CACHE PATH "Project specific path. Set manually if it was not found.")

# *** Project Properties ***
set(PROJECT_NAME "EZR")
PROJECT ( ${PROJECT_NAME} )
SET (APPLICATION_NAME "${PROJECT_NAME}")
SET (APPLICATION_CODENAME "${PROJECT_NAME}")
SET (APPLICATION_COPYRIGHT_YEARS "2013")
SET (APPLICATION_VERSION_MAJOR 0)
SET (APPLICATION_VERSION_MINOR 1)
SET (APPLICATION_VERSION_STRING "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.{APPLICATION_VERSION_TYPE}")
SET (APPLICATION_VENDOR_ID "")
SET (APPLICATION_VENDOR_NAME "Andreas Meyer, Matthias Bohleber, Markus Solbach, Guido Schmidt")
SET (APPLICATION_VENDOR_URL "")
SET (APPLICATION_ID "${APPLICATION_VENDOR_ID}.${PROJECT_NAME}")

SET (MAINFOLDER ${PROJECT_SOURCE_DIR})
SET (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

include(${CMAKE_MODULE_PATH}/doxygen.cmake)
include(${CMAKE_MODULE_PATH}/macros.cmake)

set(CMAKE_CONFIGURATION_TYPES Debug; Release)

if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Debug
       CACHE STRING "Choose the type of build : None Debug Release RelWithDebInfo MinSizeRel Coverage."
       FORCE)
endif(NOT CMAKE_BUILD_TYPE)
message("* Current build type is : ${CMAKE_BUILD_TYPE}")

find_package(OpenGL3 REQUIRED)
find_package(FreeImage REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(GLM REQUIRED)
find_package(ASSIMP REQUIRED)
find_package(SOIL REQUIRED)
find_package(Rocket REQUIRED)

ADD_SUBDIRECTORY(src)
