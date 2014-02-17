//! @file Light.h
#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include "../utils/Common.h"
#include "../renderer/ShaderProgram.h"
#include "../renderer/FrameBufferObject.h"
#include "SceneNode.h"
#include "Geometry.h"
#include <vector>

namespace scene
{
    class Light : public SceneNode
    {
        private:
            //! Light properties
            float     m_intensity; // should be [candela]
            //! Light matrices for shadow mapping
            glm::mat4 m_view;
            glm::mat4 m_projection;
            //! Objects for renddering shadow map
            bool                         m_hasShadowMap;
            glm::vec2                    m_shadowMap_size;
            renderer::ShaderProgram*     m_shaderProgram;
            renderer::FrameBufferObject* m_fbo;
            GLuint m_uniform_loc_view;
            GLuint m_uniform_loc_projection;
            GLuint m_uniform_loc_model;
		protected :
			glm::vec3 m_color;
        public:
            //! \brief Constructor
            Light(int id,
                  std::string name,
                  Transform transform,
                  glm::vec3 color,
                  float intensity);

            //! \brief Returns the lights color
            glm::vec3 getColor(void);

            //! \brief Returns the lights intensity
            float getIntensity(void);

            //! \brief Returns the lights view matrix
            glm::mat4 getViewMatrix(void);

            //! \brief Returns the lights projection matrix
            glm::mat4 getProjectionMatrix(void);

            //! \brief Draws the light as geometry
            void draw(void);

            //! \brief Setup shadow mapping
            void setupShadowMapping(glm::vec2 size);

            //! \brief Renders the shadow map
            void generateShadowMap(std::vector<Geometry*>* renderQueue);

            //! \brief Returns the shadow map
            GLuint getShadowMap(void);
    };
}

#endif // LIGHT_H
