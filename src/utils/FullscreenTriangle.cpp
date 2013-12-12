#include "FullscreenTriangle.h"

namespace utils
{
	FullscreenTriangle::FullscreenTriangle()
	{
		createGeometry();
	}


	FullscreenTriangle::~FullscreenTriangle()
	{
	}

	FullscreenTriangle* FullscreenTriangle::instance(void)
	{
		static FullscreenTriangle m_fullscreen_triangle;
		return &m_fullscreen_triangle;
	}

	void FullscreenTriangle::createGeometry(void)
	{
		glGenVertexArrays(1, &VAO_handle);
		glBindVertexArray(VAO_handle);

		glGenBuffers(1, &VBO_handle);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_handle);

		//        3 :.
		//          :   .
		//          :      .
		//        1 :_________.
		//          |         |  .
		//          |    +    |     .
		//          |_________|........
		//       -1/-1        1        3

		GLfloat vertices[] = { -1, -1, 3, -1, -1, 3 };
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	void FullscreenTriangle::draw(void)
	{
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}
