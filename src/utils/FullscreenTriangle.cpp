#include "FullscreenTriangle.h"

namespace utils
{
	FullscreenTriangle::FullscreenTriangle()
	{
        m_VAO_handle = 0;
        m_VBO_handle = 0;
        m_IBO_handle = 0;
        m_UVBO_handle = 0;
        createGeometry();
	}


	FullscreenTriangle::~FullscreenTriangle()
	{
	}

    /*
    FullscreenTriangle* FullscreenTriangle::instance(void)
	{
		static FullscreenTriangle m_fullscreen_triangle;
		return &m_fullscreen_triangle;
	}
    */

	void FullscreenTriangle::createGeometry(void)
	{
        //! Create vertex buffer
        //        3 :.
        //          :   .
        //          :      .
        //        1 :_________.
        //          |         |  .
        //          |    +    |     .
        //          |_________|........
        //       -1/-1        1        3

        GLfloat fsqVertices[] = {
                -1.0f,  1.0f,  0.0f,
                -1.0f, -1.0f,  0.0f,
                 1.0f, -1.0f,  0.0f,
                 1.0f,  1.0f,  0.0f
        };
        GLint fsqIndices[] = {
                0, 1, 3,
                2, 3, 1
        };
        GLfloat fsqUVs[] = {
                0.0f, 1.0f,
                0.0f, 0.0f,
                1.0f, 0.0f,
                1.0f, 1.0f
        };

        glGenVertexArrays(1, &m_VAO_handle);
        glBindVertexArray(m_VAO_handle);

        glGenBuffers(1, &m_VBO_handle);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO_handle);
        glBufferData(GL_ARRAY_BUFFER, sizeof(fsqVertices), fsqVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        glGenBuffers(1, &m_IBO_handle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO_handle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(fsqIndices), fsqIndices, GL_STATIC_DRAW);

        glGenBuffers(1, &m_UVBO_handle);
        glBindBuffer(GL_ARRAY_BUFFER, m_UVBO_handle);
        glBufferData(GL_ARRAY_BUFFER, sizeof(fsqUVs), fsqUVs, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(1);
	}

	void FullscreenTriangle::draw(void)
	{
        glBindVertexArray(m_VAO_handle);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}
