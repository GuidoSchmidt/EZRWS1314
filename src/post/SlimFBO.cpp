#include <GL/glew.h>
#include <iostream>
#include "SlimFBO.h"
#include <vector>

using namespace std;


SlimFBO::SlimFBO()
{

}

SlimFBO::~SlimFBO()
{
	cout << "Delete FBO" << endl;
	glDeleteFramebuffers(1, &fboPointer);
	for (int n=0;n<nAttachments;n++)
	{
		glDeleteTextures(1,&texPointer[n]);
	}
	if (depthPointer!=0)
		glDeleteRenderbuffers(1,&depthPointer);
}

SlimFBO::SlimFBO(int x, int y, int n, bool depth)
{
	cout << "create FBO and " << n << " attachments"<< endl;
	glGenFramebuffers(1, &fboPointer);
	glBindFramebuffer(GL_FRAMEBUFFER, fboPointer);

//	GLenum check = glCheckFramebufferStatus(GL_FRAMEBUFFER);
//	cout << "Check1: " << gluErrorString(check) << endl;
	xRes=x;
	yRes=y;
	nAttachments=n;


	//initiate buffers

	//Attach the textures
	for (int a=0;a<nAttachments;a++)
	{

		buffers[a]=GL_COLOR_ATTACHMENT0+a;

		cout << "attachment no "<< a << endl;
		// Create texture for attachmentA
		glGenTextures(1, &texPointer[a]);
		glBindTexture(GL_TEXTURE_2D, texPointer[a]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, xRes, yRes, 0, GL_RGBA, GL_FLOAT, NULL);
		// Attach texture to FBO
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0+a,GL_TEXTURE_2D,texPointer[a],0);
	}

	if (depth)
	{
		// Attach depth buffer
		glGenRenderbuffers(1, &depthPointer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthPointer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
												 xRes, yRes);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER,
			GL_DEPTH_ATTACHMENT,
			GL_RENDERBUFFER,
			depthPointer);
	}
	glBindFramebuffer(GL_FRAMEBUFFER,0);

//	check = glCheckFramebufferStatus(GL_FRAMEBUFFER);
//	cout << "check2: " << gluErrorString(check) << endl;
}


void SlimFBO::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fboPointer);
}

void SlimFBO::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SlimFBO::write()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fboPointer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawBuffers(nAttachments, buffers);
}

void SlimFBO::update()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fboPointer);
	glDrawBuffers(nAttachments, buffers);
}

//void SlimFBO::showAttachment(int n)
//{
//	if (n>=nAttachments) return;
//
//	glBindTexture(GL_TEXTURE_2D,texPointer[n]);
//}
