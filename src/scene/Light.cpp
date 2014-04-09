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

		m_view = glm::lookAt(glm::vec3(m_transform.getPosition()),  //* glm::vec3(0.01),
                             glm::vec3( glm::vec3(0.0f, 0.3f, 0.0f) ),
                             glm::vec3(0.001f, 1.0f, 0.0f));
        return m_view;
    }

    glm::mat4 Light::getProjectionMatrix(void)
    {
		float length = glm::length(m_transform.getPosition());
		m_projection = glm::perspective(4.0f, 1.0f, length*0.95f, length*1.15f);
		//m_projection = glm::ortho(-30,30,-30,30);
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
		m_uniform_loc_diffuse_tex = m_shaderProgram->getUniform("diffuse_tex");
        //! Framebuffer Object(
        m_fbo = new renderer::FrameBufferObject(m_shadowMap_size.x, m_shadowMap_size.y);
        //m_fbo->addColorAttachment(0);

		m_fbo->addDepthAttachment_Texture(0);
    }

    void Light::generateShadowMap(std::vector<Geometry*>* renderQueue)
    {

        m_shaderProgram->use();
        m_fbo->use();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glEnable(GL_CULL_FACE);
        //glCullFace(GL_FRONT);

        glViewport(0, 0, m_shadowMap_size.x, m_shadowMap_size.y);

        m_shaderProgram->setUniform(m_uniform_loc_view, getViewMatrix());
        m_shaderProgram->setUniform(m_uniform_loc_projection, getProjectionMatrix());

        for(unsigned int i = 0; i < renderQueue->size(); i++)
        {
            m_shaderProgram->setUniform(m_uniform_loc_model, renderQueue->at(i)->getTransform()->getModelMatrix() );
			m_shaderProgram->setUniformSampler(m_uniform_loc_diffuse_tex, renderQueue->at(i)->getMaterial()->getDiffuseTexture(), 0);
            renderQueue->at(i)->drawTriangles();
        }

		//glDisable(GL_CULL_FACE);
        m_shaderProgram->unuse();
        m_fbo->unuse();

    }

    GLuint Light::getShadowMap(void)
    {
        return m_fbo->getDepthTexture();
    }
}
