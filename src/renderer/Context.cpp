//! @file Context.cpp
#include "Context.h"

namespace tone {
    namespace renderer {
        //! \brief Constructor
        Context::Context(const glm::ivec2& size)
        {
            m_size = size;
            m_title = "Monarch Renderer";
            init();
        }

        //! \brief Destructor
        Context::~Context()
        {
            glfwTerminate();
        }

        //! \brief Initializes the window
        /*!
            *
            */
        void Context::init()
        {
            if(!glfwInit())
            {
                //! Error handling
                std::cout << "ERROR (GLFW): Initialization failed!\n";
            }

            m_window = glfwCreateWindow(m_size.x, m_size.y, m_title.c_str(), 0, 0);
            if(!m_window)
            {
                //! Error handling
                std::cout << "ERROR (GLFW): Window could not be created!\n";
            }
            glfwMakeContextCurrent(m_window);

            glewExperimental = GL_TRUE;
            if(glewInit() != GLEW_OK)
            {
                //! Error handling
                std::cout << "ERROR (GLEW): Initialization failed!\n";
            }

            const GLubyte* glinfo_renderer_ptr = glGetString(GL_RENDERER);
            const GLubyte* glinfo_vendor_ptr = glGetString(GL_VENDOR);
            const GLubyte* glinfo_openglVersion_ptr = glGetString(GL_VERSION);
            const GLubyte* glinfo_glslVersion_ptr = glGetString(GL_SHADING_LANGUAGE_VERSION);

            GLint glinfo_major, glinfo_minor;
            glGetIntegerv(GL_MAJOR_VERSION, &glinfo_major);
            glGetIntegerv(GL_MINOR_VERSION, &glinfo_minor);

            //! Write renderer infos to log
            std::cout << "--- HARDWARE -------------------------------------------------------------\n";
            std::cout << "OpenGL Vendor:     ";
            std::cout << glinfo_vendor_ptr;
            std::cout << "\nOpenGL Renderer:   ";
            std::cout << glinfo_renderer_ptr;
            std::cout << "\nOpenGL Version:    ";
            std::cout << glinfo_openglVersion_ptr;
            std::cout << "\nGLSL Version:      ";
            std::cout << glinfo_glslVersion_ptr;
            std::cout << "\n\n";
        }


        //! \brief Checks if the context still exists
        bool Context::isLive(void)
        {
            return !glfwWindowShouldClose(m_window);
        }

        //! \brief Renders the window to screen
        /*!
            *
            */
        void Context::swapBuffers()
        {
                glfwSwapBuffers(m_window);
                glfwPollEvents();
        }

        //!
        glm::ivec2 Context::getSize(void)
        {
            return m_size;
        }

        //! \brief Returns the GLFW-window
        GLFWwindow* Context::getWindow(void)
        {
            return m_window;
        }
    }
}
