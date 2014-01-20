//! @file Light.cpp
#include "Light.h"

namespace scene
{
    Light::Light(int id,
                 std::string name,
                 Transform transform,
                 glm::vec3 color,
                 float intensity)
        : SceneNode(LIGHT, id, name)
    {
        m_color      = color;
        m_intensity  = intensity;
        m_view       = glm::mat4(0.0f);
        m_projection = glm::mat4(0.0f);

        setTransform(transform);
        m_transform.setPosition(transform.getPosition());

        m_hasShadowMap = false;
    }

    glm::vec3 Light::getColor(void)
    {
        return m_color;
    }

    float Light::getIntensity(void)
    {
        return m_intensity;
    }

    glm::mat4 Light::getViewMatrix(void)
    {
        m_view = glm::lookAt(glm::vec3(m_transform.getPosition()),
                             glm::vec3( glm::vec3(0.0f, 0.3f, 0.0f) ),
                             glm::vec3(0.0f, 1.0f, 0.0f));
        return m_view;
    }

    glm::mat4 Light::getProjectionMatrix(void)
    {
        m_projection = glm::perspective(60.0f, 1.0f, 0.1f, 100.0f);
        return m_projection;
    }

    void Light::setupShadowMapping(glm::vec2 size)
    {
        m_shadowMap_size = size;
        m_hasShadowMap = true;
        //! Shader program
        m_shaderProgram = new renderer::ShaderProgram(renderer::GLSL::VERTEX, RESOURCES_PATH "/shader/shadow/shadowMap.vs.glsl",
                                                      renderer::GLSL::FRAGMENT, RESOURCES_PATH "/shader/shadow/shadowMap.fs.glsl");
        m_shaderProgram->link();
        //! Uniform setup
        m_uniform_loc_model      = m_shaderProgram->getUniform("model");
        m_uniform_loc_view       = m_shaderProgram->getUniform("view");
        m_uniform_loc_projection = m_shaderProgram->getUniform("projection");
        //! Framebuffer Object
        m_fbo = new renderer::FrameBufferObject(m_shadowMap_size.x, m_shadowMap_size.y);
        m_fbo->addColorAttachment(0);
        m_fbo->addDepthAttachment_Texture(1);
    }

    void Light::generateShadowMap(std::vector<Geometry*>* renderQueue)
    {
        //if(!m_hasShadowMap)
        //{
            m_shaderProgram->use();
            m_fbo->use();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glCullFace(GL_FRONT_FACE);
            glViewport(0, 0, m_shadowMap_size.x, m_shadowMap_size.y);

            m_shaderProgram->setUniform(m_uniform_loc_view, getViewMatrix());
            m_shaderProgram->setUniform(m_uniform_loc_projection, getProjectionMatrix());

            for(unsigned int i = 0; i < renderQueue->size(); i++)
            {
                m_shaderProgram->setUniform(m_uniform_loc_model, renderQueue->at(i)->getTransform()->getModelMatrix() );
                renderQueue->at(i)->drawTriangles();
            }

            m_shaderProgram->unuse();
            m_fbo->unuse();
        //}
    }

    GLuint Light::getShadowMap(void)
    {
        return m_fbo->getDepthTexture();
    }
}
