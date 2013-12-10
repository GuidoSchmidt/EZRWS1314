#include "SeparatedBlurPass.h"
//#include <GL/glew.h>


SeparatedBlurPass::SeparatedBlurPass(int scale, SlimQuad* pQuad, int pWidth, int pHeight)
: ISlimRenderPass(pQuad)
{
	width = pWidth;
	height = pHeight;
	renderPassShader = new SlimShader("../../resources/shader_source/", "post.vert.glsl", "glow.frag.glsl");

	glowColorUniform = glGetUniformLocation(renderPassShader->shaderProgram, "color");
	glowFactorUniform = glGetUniformLocation(renderPassShader->shaderProgram, "factor");
	glowBrightnessUniform = glGetUniformLocation(renderPassShader->shaderProgram, "brightness");

	glowScale = scale;
	quad = pQuad;


	//params
	params_glow_brightness = 100.0f;


}

GlowPass::~GlowPass() {
	delete renderPassShader;
}

void GlowPass::doExecute() {

	outputFBO->write();
	renderPassShader->enable();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, inputFBOs[0]->texPointer[0]);
	glUniform1i(glowColorUniform, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, inputFBOs[0]->texPointer[1]);
	glUniform1i(glowFactorUniform, 1);

	glUniform1f(glowBrightnessUniform, params_glow_brightness);

	glViewport(0, 0, width / glowScale, width / glowScale);
	quad->draw();

	outputFBO->unbind();
	glViewport(0, 0, width, width);
}

