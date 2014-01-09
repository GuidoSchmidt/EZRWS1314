#include "FinalPass.h"
//#include <GL/glew.h>


FinalPass::FinalPass(SlimQuad* pQuad, int pWidth, int pHeight)
: ISlimRenderPass(pQuad)
{
	quad = pQuad;
	width = pWidth;
	height = pHeight;
	renderPassShader = new SlimShader(RESOURCES_PATH "/shader_source/", "post.vert.glsl", "final.frag.glsl");

	topLeftUniform    = glGetUniformLocation(renderPassShader->shaderProgram, "topLeft");
	topRightUniform   = glGetUniformLocation(renderPassShader->shaderProgram, "topRight");
	lowerLeftUniform  = glGetUniformLocation(renderPassShader->shaderProgram, "lowerLeft");
	lowerRightUniform = glGetUniformLocation(renderPassShader->shaderProgram, "lowerRight");
	minAveMaxTextureUniform = glGetUniformLocation(renderPassShader->shaderProgram, "minAveMaxTexture");
	
	fastExtractionUniform = glGetUniformLocation(renderPassShader->shaderProgram, "fastExtraction");
	screenSizeUniform = glGetUniformLocation(renderPassShader->shaderProgram, "screenSize");

	minAveMaxUniform = glGetUniformLocation(renderPassShader->shaderProgram, "minAveMaxUni");
	//params
}


FinalPass::~FinalPass() {
	delete renderPassShader;
}

void FinalPass::doExecute() {

	glBindFramebuffer(GL_FRAMEBUFFER, 0); 
		renderPassShader->enable();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, inputFBOs[0]->texPointer[0]);
			glUniform1i(topLeftUniform, 0);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, inputFBOs[1]->texPointer[0]);
			glUniform1i(topRightUniform, 1);

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, inputFBOs[2]->texPointer[0]);
			glUniform1i(lowerLeftUniform, 2);

			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, inputFBOs[3]->texPointer[0]);
			glUniform1i(lowerRightUniform, 3);

			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, minAveMaxTexture);
			glUniform1i(minAveMaxTextureUniform, 4);

			glUniform2i(screenSizeUniform, (GLint)width, (GLint)height);
			glUniform3f(minAveMaxUniform, param_minAveMax.x, param_minAveMax.y, param_minAveMax.z);
			glUniform1f(fastExtractionUniform, param_fastExtraction);
			quad->draw();
		
		renderPassShader->disable();
}

