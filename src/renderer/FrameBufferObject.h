//! @file FrameBufferObjet.h
/*!
 * @date 	02.01.2013
 * @author	Guido Schmidt
 */

#pragma once
#ifndef FRAMEBUFFEROBJECT_H
#define FRAMEBUFFEROBJECT_H

#include "../utils/Common.h"
#include <vector>
#include <iostream>

namespace renderer {
    //! @class FrameBuffeObject
    class FrameBufferObject
    {
        private:
            GLuint	m_FBO_ID;
            bool	m_isGBuffer;
            int		m_width, m_height;
            GLsizei m_attachmentCounter;

            GLuint m_depthBuffer;
            GLuint m_depthTexture;
            GLuint m_depthMSTexture;

            std::vector<GLuint> m_renderTargets;
            std::vector<GLenum> m_drawBuffers;

        public:
			//! \brief Constructor
            FrameBufferObject(int width, int height);
            
			//! \brief Destructor
			~FrameBufferObject();

			//! \brief Adds a color attachment
            void addColorAttachment(int textureUnit);

			//! \brief Adds a depth buffer attachment
            void addDepthAttachment_Buffer();

			//! \brief Adds a depth texture attachment
            void addDepthAttachment_Texture(int textureUnit);

			//! \brief Adds a multi-sample-texture attachment
            void addDepthAttachment_MultisampleTexture(int textureUnit);

			//! \brief Creates a G-Buffer
            void createGBuffer(void);

			//! \brief Creates the needed buffers
            void createBuffers(int count);

			//! \brief Activates the framebuffer object for use
            void use(void);

			//! \brief Deactivates the framebuffer object from use
            void unuse(void);

			//! \brief Returns a texture handle for a given attachment number
            GLuint getTexture(unsigned int index);

			//! \brief Returns a texture handle for the attached depth texture attachment
            GLuint getDepthTexture(void);

			//! \brief Returns a texture handle for the given depth multisample texture attachment
            GLuint getDepthMSTexture(void);

			//! \brief Returns the count of render targets
            int getRenderTargetCount(void);
    };
}
#endif //FRAMEBUFFEROBJECT_H
