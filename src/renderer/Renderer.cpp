//! @file Renderer.cpp
#include "Renderer.h"

namespace tone {
    namespace renderer {
        //! \brief Constructor
        Renderer::Renderer(void)
        {
            //! Initialize fps variables
            m_framecount = 0;
            m_fps = 0;
            m_current_time = 0.0;
            m_previous_time = 0.0;

            init();
        }

        //! \brief Destructor
        Renderer::~Renderer(void)
        {
        }

        //! \brief Returns singleton instance
        Renderer* Renderer::instance(void)
        {
            static Renderer m_instance;
            return &m_instance;
        }

        //! \brief Sets the context to render to
        void Renderer::setRenderContext(Context& context)
        {
            m_context = &context;
        }

        //! \brief Initializes the renderer
        void Renderer::init()
        {
            setupGL();
        }

        //! \brief Sets all the initial OpenGL flags
        void Renderer::setupGL(void)
        {
            //! OpenGL settings
            glClearColor(0.0, 0.0, 0.0, 1.0);
            glEnable(GL_DEPTH_TEST);
        }

        //! \brief Render loop
        void Renderer::renderloop()
        {
            while (m_context && m_context->isLive())
            {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                m_context->swapBuffers();
                m_framecount++;
            }
        }
    }
}
