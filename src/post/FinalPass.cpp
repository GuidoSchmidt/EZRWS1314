#include "FinalPass.h"
//#include <GL/glew.h>


FinalPass::FinalPass(SlimQuad* pQuad, int pWidth, int pHeight)
: ISlimRenderPass(pQuad)
{
	quad = pQuad;
	width = pWidth;
	height = pHeight;
	renderPassShader = new SlimShader(RESOURCES_PATH "/shader_source/", "post.vert.glsl", "final.frag.glsl");

	gBuffer1Uniform   = glGetUniformLocation(renderPassShader->shaderProgram, "gBuffer1");
	sunlight1Uniform = glGetUniformLocation(renderPassShader->shaderProgram, "sunlight1");
	sunlight2Uniform = glGetUniformLocation(renderPassShader->shaderProgram, "sunlight2");
	sunlight3Uniform = glGetUniformLocation(renderPassShader->shaderProgram, "sunlight3");

	screenSizeUniform = glGetUniformLocation(renderPassShader->shaderProgram, "screenSize");

	//params
}


FinalPass::~FinalPass() {
	delete renderPassShader;
}

void FinalPass::doExecute() {

	glBindFramebuffer(GL_FRAMEBUFFER, GL_BACK); 
		renderPassShader->enable();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, inputFBOs[0]->texPointer[0]);
			glUniform1i(gBuffer1Uniform, 0);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, inputFBOs[1]->texPointer[0]);
			glUniform1i(sunlight1Uniform, 1);

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, inputFBOs[2]->texPointer[0]);
			glUniform1i(sunlight2Uniform, 2);

			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, inputFBOs[3]->texPointer[0]);
			glUniform1i(sunlight3Uniform, 3);

			glUniform2i(screenSizeUniform, (GLint)width, (GLint)height);

			quad->draw();
		
		renderPassShader->disable();
}

