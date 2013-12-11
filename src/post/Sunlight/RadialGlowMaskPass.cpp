#include "RadialGlowMaskPass.h"


RadialGlowMaskPass::RadialGlowMaskPass(SlimQuad* pQuad, int pWidth, int pHeight)
: ISlimRenderPass(pQuad)
{
	quad = pQuad;
	width = pWidth;
	height = pHeight;
	renderPassShader = new SlimShader(RESOURCES_PATH "/shader_source/", "radialGlow.vert.glsl", "radialGlow.frag.glsl");

	blurUniform = glGetUniformLocation(renderPassShader->shaderProgram, "blur");
	maskUniform = glGetUniformLocation(renderPassShader->shaderProgram, "mask");
	screenSizeUniform = glGetUniformLocation(renderPassShader->shaderProgram, "screenSize");

}


RadialGlowMaskPass::~RadialGlowMaskPass() {
	delete renderPassShader;
}

void RadialGlowMaskPass::doExecute() {

	outputFBO->write();
		renderPassShader->enable();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, inputFBOs[0]->texPointer[0]);
			glUniform1i(blurUniform, 0);

			glActiveTexture(GL_TEXTURE1);
			//glBindTexture(GL_TEXTURE_2D, inputFBOs[1]->texPointer[0]); glowMask
			glUniform1i(maskUniform, 1);

			glUniform2i(screenSizeUniform, (GLint)width, (GLint)height);

			quad->draw();

		renderPassShader->disable();
	outputFBO->unbind();
}

