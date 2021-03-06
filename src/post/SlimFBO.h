#ifndef SLIMFBO_H_
#define SLIMFBO_H_

#include <GL/glew.h>

#include <iostream>
#include <vector>

using namespace std;


class SlimFBO
{
public:
	SlimFBO();
	SlimFBO(int x,int y,int n, bool depth, int filter);
	~SlimFBO();
	void bind();
	void unbind();
	void update();
	void write();
	static void blit(SlimFBO* read, SlimFBO* draw);
//	void showAttachment(int n);

	GLuint fboPointer;
	GLuint depthPointer;
	GLuint texPointer[4];
	GLenum buffers[4];
	int nAttachments;
	int xRes, yRes;

};

#endif /* SLIMFBO_H_ */
