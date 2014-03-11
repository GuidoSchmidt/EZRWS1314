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
        m_shaderProgram_forward = 0;

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

        m_shaderProgram_forward = new ShaderProgram(GLSL::VERTEX, RESOURCES_PATH "/shader/forward/forward.vs.glsl",
                                                    GLSL::FRAGMENT, RESOURCES_PATH "/shader/forward/forward.fs.glsl");
    }

    void Renderer::setupGL(void)
    {
        //! OpenGL settings
        glClearColor(0.8, 0.1, 0.2, 1.0);
        glEnable(GL_DEPTH_TEST);
    }

    void Renderer::renderloop()
    {
        glClearColor(0.0f, 0.1f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_framecount++;
    }

    std::string Renderer::getShaderSourceOf(GLSL::GLSLShaderType shaderType)
    {
        std::string code = m_shaderProgram_forward->getShaderCodeOf(shaderType);
        return code;
    }
}
