#include "RadialGlowMaskPass.h"


RadialGlowMaskPass::RadialGlowMaskPass(SlimQuad* pQuad, int pWidth, int pHeight)
: ISlimRenderPass(pQuad)
{
	quad = pQuad;
	width = pWidth;
	height = pHeight;
	renderPassShader = new SlimShader(RESOURCES_PATH "/shader_source/", "radialGlow.vert.glsl", "radialGlow.frag.glsl");

	maskTexture = scene::SceneManager::instance()->loadTexture(RESOURCES_PATH "/textures/radialGlowMask.jpg",false);

	blurUniform = glGetUniformLocation(renderPassShader->shaderProgram, "blurSampler");
	maskUniform = glGetUniformLocation(renderPassShader->shaderProgram, "maskSampler");
	screenSizeUniform = glGetUniformLocation(renderPassShader->shaderProgram, "screenSize");
	ssSunPosUniform = glGetUniformLocation(renderPassShader->shaderProgram, "sunPos");

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
			glBindTexture(GL_TEXTURE_2D, maskTexture);
			glUniform1i(maskUniform, 1);

			glUniform2i(screenSizeUniform, (GLint)width, (GLint)height);
			glUniform3f(ssSunPosUniform, param_ssSunPos.x,param_ssSunPos.y,param_ssSunPos.z);

			glViewport(0, 0, width, height);
			quad->draw();
			glViewport(0, 0, width*4, height*4);

		renderPassShader->disable();
	outputFBO->unbind();
}

