/*
 * SlimQuad.cpp
 *
 *  Created on: 15.10.2013
 *      Author: Andi
 */


#include "SlimQuad.h"

SlimQuad::SlimQuad()
{
	GLfloat quadArray[] = {-1,-1,0,
							1,-1,0,
							1, 1,0,
						   -1, 1,0};
	GLfloat quadTexCos[] = {0,0,
							1,0,
							1,1,
							0,1};

	glGenBuffers(1,&quadVertexPointer);
	glBindBuffer(GL_ARRAY_BUFFER, quadVertexPointer);
	glBufferData(GL_ARRAY_BUFFER, 12*sizeof(GLfloat), quadArray, GL_STATIC_DRAW);

	glGenBuffers(1,&quadTexCosPointer);
	glBindBuffer(GL_ARRAY_BUFFER, quadTexCosPointer);
	glBufferData(GL_ARRAY_BUFFER, 8*sizeof(GLfloat), quadTexCos, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER,0);
	//std::cout << "quad created" << std::endl;
}

void SlimQuad::draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, quadVertexPointer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, quadTexCosPointer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0); 
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_QUADS,0,4);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

SlimQuad::~SlimQuad() {

}
