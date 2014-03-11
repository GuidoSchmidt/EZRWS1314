//! @file Context.cpp
/*!
*	@date 	03.12.2013
*	@author	Guido Schmidt
*/
#include "Context.h"

//! \todo replace
#include "../ui/shell/x11/InputX11.h"
#include "../ui/shell/ShellFileInterface.h"
#include <sys/types.h>
//#include <sys/time.h>
#include <time.h>
#include <Rocket/Core/Core.h>

//static timeval start_time;
static ShellFileInterface* file_interface = NULL;

namespace renderer {
    Context::Context(const glm::ivec2& size)
    {
        m_size = size;
    }

    Context::~Context()
    {
        glfwTerminate();
    }

	void Context::createWindow(std::string title)
    {
        //FILE *stream ;
        //if((stream = freopen("log.txt", "w", stdout)) == NULL)
            //std::cerr << "ERROR (Log): Could not write to file";
		std::cout << "*** LOG ***" << std::endl;

        if(!glfwInit())
        {
            //! Error handling
            std::cerr << "ERROR (GLFW): Initialization failed!\n";
        }

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		m_window = glfwCreateWindow(m_size.x, m_size.y, title.c_str(), 0, 0);
        if(!m_window)
        {
            //! Error handling
            std::cerr << "ERROR (GLFW): Window could not be created!\n";
        }
        glfwMakeContextCurrent(m_window);

        if(glewInit() != GLEW_OK)
        {
            //! Error handling
            std::cerr << "ERROR (GLEW): Initialization failed!\n";
        }

        const GLubyte* glinfo_renderer_ptr = glGetString(GL_RENDERER);
        const GLubyte* glinfo_vendor_ptr = glGetString(GL_VENDOR);
        const GLubyte* glinfo_openglVersion_ptr = glGetString(GL_VERSION);
        const GLubyte* glinfo_glslVersion_ptr = glGetString(GL_SHADING_LANGUAGE_VERSION);

        GLint glinfo_major, glinfo_minor;
        glGetIntegerv(GL_MAJOR_VERSION, &glinfo_major);
        glGetIntegerv(GL_MINOR_VERSION, &glinfo_minor);

        //! Write renderer infos to log
        std::cout << "--- HARDWARE -------------------------------------------------------------" << std::endl;
        std::cout << "OpenGL Vendor:     ";
        std::cout << glinfo_vendor_ptr << std::endl;
        std::cout << "OpenGL Renderer:   ";
        std::cout << glinfo_renderer_ptr << std::endl;
        std::cout << "OpenGL Version:    ";
        std::cout << glinfo_openglVersion_ptr << std::endl;;
        std::cout << "GLSL Version:      ";
        std::cout << glinfo_glslVersion_ptr  << std::endl;

        // Set up the GL state.
        glClearColor(0, 0, 0, 1);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, 1024, 768, 0, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }


    bool Context::isLive(void)
    {
        return !glfwWindowShouldClose(m_window);
    }

    void Context::swapBuffers()
    {
            glfwSwapBuffers(m_window);
            glfwPollEvents();
    }

    glm::ivec2 Context::getSize(void)
    {
        return m_size;
    }

	float Context::getAspectRatio(void)
	{
		return m_size.y / static_cast<float>(m_size.x);
	}

    GLFWwindow* Context::getWindow(void)
    {
        return m_window;
    }

    void Context::SetLoop(IdleFunction loop)
    {
        while(!glfwWindowShouldClose(m_window))
        {
            loop();
            glfwPollEvents();
        }
    }

    float Context::GetElapsedTime()
    {
        return static_cast<float>(glfwGetTime());
    }
}
