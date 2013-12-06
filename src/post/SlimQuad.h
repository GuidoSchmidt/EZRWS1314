/*
 * SlimQuad.h
 *
 *  Created on: 15.10.2013
 *      Author: Andi
 */

#ifndef SLIMQUAD_H_
#define SLIMQUAD_H_

#include <GL/glew.h>

class SlimQuad {
public:
	SlimQuad();
	virtual ~SlimQuad();
	void draw();
private:
	GLuint quadVertexPointer;
	GLuint quadTexCosPointer;
};

#endif /* SLIMQUAD_H_ */
