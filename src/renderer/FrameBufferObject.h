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

namespace ezr {
    namespace renderer {
        //! @class FrameBuffeObject
        class FrameBufferObject
        {
            private:
                GLuint m_FBO_ID;
                bool m_isGBuffer;
                int m_width, m_height;
                GLsizei m_attachmentCounter;

                GLuint m_depthBuffer;
                GLuint m_depthTexture;
                GLuint m_depthMSTexture;

                std::vector<GLuint> m_renderTargets;
                std::vector<GLenum> m_drawBuffers;

            public:
                FrameBufferObject(int width, int height, bool gBuffer);
                ~FrameBufferObject();

                void AddColorAttachment(int textureUnit);
                void AddDepthAttachment_Buffer();
                void AddDepthAttachment_Texture(int textureUnit);
                void AddDepthAttachment_MultisampleTexture(int textureUnit);

                void CreateGBuffer(void);
                void CreateBuffers(int count);

                void Use(void);
                void Unuse(void);

                GLuint GetTexture(unsigned int index);
                GLuint GetDepthTexture(void);
                GLuint GetDepthMSTexture(void);
                int GetRenderTargetCound(void);
        };
    }
}
#endif //FRAMEBUFFEROBJECT_H
