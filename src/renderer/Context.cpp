//! @file Context.cpp
#include "Context.h"

namespace ezr {
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
			FILE *stream ;
			if((stream = freopen("log.txt", "w", stdout)) == NULL)
				std::cerr << "ERROR (Log): Could not write to file";
			std::cout << "*** LOG ***" << std::endl;

            if(!glfwInit())
            {
                //! Error handling
                std::cerr << "ERROR (GLFW): Initialization failed!\n";
            }

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

        GLFWwindow* Context::getWindow(void)
        {
            return m_window;
        }
    }
}
