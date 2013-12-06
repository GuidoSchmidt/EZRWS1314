//! @file Renderer.cpp
#include "Renderer.h"

namespace renderer {

    Renderer::Renderer(void)
    {
        //! Initialize fps variables
        m_framecount = 0;
        m_fps = 0;
        m_current_time = 0.0;
        m_previous_time = 0.0;

        init();
    }

    Renderer::~Renderer(void)
    {
    }

    Renderer* Renderer::instance(void)
    {
        static Renderer m_instance;
        return &m_instance;
    }

    void Renderer::setRenderContext(Context& context)
    {
        m_context = &context;
    }

    void Renderer::init()
    {
        setupGL();
        setupShaderStages();
    }

    void Renderer::setupGL(void)
    {
        //! OpenGL settings
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glEnable(GL_DEPTH_TEST);
    }

    void Renderer::setupShaderStages()
    {
        m_shaderProgram_forward = new ShaderProgram(GLSL::VERTEX, RESOURCES_PATH "/shader/forward/forward.vs.glsl",
                                                    GLSL::FRAGMENT, RESOURCES_PATH "/shader/forward/forward.fs.glsl");
        m_shaderProgram_forward->Link();
    }

    void Renderer::renderloop()
    {
        while (m_context && m_context->isLive())
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //! Render calls here



            m_context->swapBuffers();
            m_framecount++;
        }
    }
}
