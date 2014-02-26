//! Common Header
/*!
 * Often used includes
 */

#pragma once
#ifndef COMMON_H
#define COMMON_H

//! C++ includes
    #include <string>
    #include <iostream>
    #include <stdio.h>

//! Third party includes
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
    #include <glm/glm.hpp>
    #include <Rocket/Core.h>

extern GLFWwindow *glfwindow;

//! GUI elements
extern Rocket::Core::ElementDocument *textBox;
extern Rocket::Core::ElementDocument *navBar;


#endif //COMMON_H
