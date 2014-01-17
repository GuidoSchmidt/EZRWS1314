//! @file FrameBufferObject.cpp
/*!
 * @date 	02.01.2013
 * @author	Guido Schmidt
 */

#include "FrameBufferObject.h"

namespace renderer {

	//! \todo overwork framebuffer object class
	/*!
		texture settings?
		better attachment handling?
		draw buffers?
	*/
    FrameBufferObject::FrameBufferObject(int width, int height)
    {
        m_width = width;
        m_height = height;
        m_attachmentCounter = 0;
        glGenFramebuffers(1, &m_FBO_ID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO_ID);
        //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    FrameBufferObject::~FrameBufferObject()
    {
    }

    void FrameBufferObject::createGBuffer(void)
    {
        //! World Space Positions
        //AddColorAttachment(0);
        //! View space positions
        addColorAttachment(0);
        //! View space normals
        addColorAttachment(1);
        //! World space normals
        //AddColorAttachment(2);
        //! Color
        addColorAttachment(2);
        //! Reflectance
        addColorAttachment(3);
        //! Billboards
        addColorAttachment(4);
        //! Free color attachment
        //AddColorAttachment(5);
        //! Depth
        addDepthAttachment_Texture(5);
        //AddDepthAttachment_MultisampleTexture(9);

        std::cout << "FrameBuffer: Attachment count: " << m_attachmentCounter << std::endl;
        m_isGBuffer = true;
    }

    void FrameBufferObject::createBuffers(int count)
    {
        //! Color attachment 0
        addColorAttachment(0);
    }

    void FrameBufferObject::addColorAttachment(GLint textureUnit)
    {
        GLuint renderTexture;

        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glGenTextures(1, &renderTexture);
        glBindTexture(GL_TEXTURE_2D, renderTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //! Bind texture to color attachment point
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_attachmentCounter, GL_TEXTURE_2D, renderTexture, 0);

        //! Push the color attachment and the texture to FBO's intern storage
        m_drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + m_attachmentCounter);
        m_renderTargets.push_back(renderTexture);

        //! Increase attachment counter
        m_attachmentCounter++;
    }

    void FrameBufferObject::addDepthAttachment_Buffer(void)
    {

        glGenRenderbuffers(1, &m_depthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 2, GL_DEPTH_COMPONENT, m_width, m_height);

        //! Bind depth buffer to FBO
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);
    }

    void FrameBufferObject::addDepthAttachment_Texture(int textureUnit)
    {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glGenTextures(1, &m_depthTexture);
        glBindTexture(GL_TEXTURE_2D, m_depthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

        glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 2, GL_DEPTH_COMPONENT32F, m_width, m_height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);
    }

    void FrameBufferObject::addDepthAttachment_MultisampleTexture(int textureUnit)
    {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glGenTextures(1, &m_depthMSTexture);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_depthMSTexture);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 2, GL_DEPTH_COMPONENT32F, m_width, m_height, false);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP);

        glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 2, GL_DEPTH_COMPONENT, m_width, m_height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);
    }

    void FrameBufferObject::use(void)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO_ID);
        glDrawBuffers(3, &m_drawBuffers.at(0));
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);
    }


    void FrameBufferObject::unuse(void)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    GLuint FrameBufferObject::getTexture(unsigned int index)
    {
        if(index > m_renderTargets.size()-1){
            std::cout << "ERROR | FrameBufferObject: texture index out of bounds!" << std::endl;
        }
        return m_renderTargets[index];
    }

    GLuint FrameBufferObject::getDepthTexture(void)
    {
        return m_depthTexture;
    }

    GLuint FrameBufferObject::getDepthMSTexture(void)
    {
        return m_depthMSTexture;
    }
}
