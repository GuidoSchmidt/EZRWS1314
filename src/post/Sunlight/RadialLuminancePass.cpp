#include "RadialLuminancePass.h"
//#include <GL/glew.h>


RadialLuminancePass::RadialLuminancePass(SlimQuad* pQuad, int pWidth, int pHeight)
: ISlimRenderPass(pQuad)
{
	quad = pQuad;
	width = pWidth;
	height = pHeight;
	renderPassShader = new SlimShader(RESOURCES_PATH "/shader_source/", "radialLuminance.vert.glsl", "radialLuminance.frag.glsl");

	maskUniform = glGetUniformLocation(renderPassShader->shaderProgram, "maskSampler");

	//nSamplesUniform = glGetUniformLocation(renderPassShader->shaderProgram, "nSamples");
	scaleUniform    = glGetUniformLocation(renderPassShader->shaderProgram, "scale");
	sunPosUniform = glGetUniformLocation(renderPassShader->shaderProgram, "sunPos");
	screenSizeUniform = glGetUniformLocation(renderPassShader->shaderProgram, "screenSize");


	//params
	param_nSamples = 30.0f;
	param_scale  = -2.0f;
}


RadialLuminancePass::~RadialLuminancePass() {
	delete renderPassShader;
}

void RadialLuminancePass::doExecute() {

	outputFBO->write();
		renderPassShader->enable();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, inputFBOs[0]->texPointer[0]);
			glUniform1i(maskUniform, 0);

			//glUniform1f(nSamplesUniform, param_nSamples);
			glUniform1f(scaleUniform, param_scale);
			glUniform2i(screenSizeUniform, (GLint)width, (GLint)height);
			glUniform3f(sunPosUniform, param_ssSunPos.x, param_ssSunPos.y, param_ssSunPos.z);

			glViewport(0, 0, width, height);
			quad->draw();

		renderPassShader->disable();
	outputFBO->unbind();
	glViewport(0, 0, width*4, height*4);
}