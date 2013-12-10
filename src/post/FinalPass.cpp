#include "FinalPass.h"
//#include <GL/glew.h>


FinalPass::FinalPass(SlimQuad* pQuad, int pWidth, int pHeight)
: ISlimRenderPass(pQuad)
{
	quad = pQuad;
	width = pWidth;
	height = pHeight;
	renderPassShader = new SlimShader(RESOURCES_PATH "/shader_source/", "post.vert.glsl", "final.frag.glsl");

	colorUniform = glGetUniformLocation(renderPassShader->shaderProgram, "color");

	screenSizeUniform = glGetUniformLocation(renderPassShader->shaderProgram, "screenSize");

	//params
}


FinalPass::~FinalPass() {
	delete renderPassShader;
}

void FinalPass::doExecute() {

	glBindFramebuffer(GL_FRAMEBUFFER, GL_BACK); 
	//outputFBO->write();
		renderPassShader->enable();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, inputFBOs[0]->texPointer[0]);
			glUniform1i(colorUniform, 0);

			glUniform2i(screenSizeUniform, (GLint)width, (GLint)height);

			quad->draw();
		
		renderPassShader->disable();
	//outputFBO->unbind();
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

