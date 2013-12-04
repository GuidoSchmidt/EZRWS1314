#include "GlowPass.h"
//#include <GL/glew.h>


GlowPass::GlowPass(int scale, SlimQuad* pQuad, int pWidth, int pHeight)
	: ISlimRenderPass(pQuad)
{
	width = pWidth;
	height = pHeight;
	renderPassShader = new SlimShader("src/Passes/Glow/","post.vert.glsl", "glow.frag.glsl");

	glowColorUniform = glGetUniformLocation(renderPassShader->shaderProgram,"color");
	glowFactorUniform = glGetUniformLocation(renderPassShader->shaderProgram,"factor");
	glowBrightnessUniform = glGetUniformLocation(renderPassShader->shaderProgram,"brightness");

	glowScale = scale;
	quad = pQuad;

//	TwAddSeparator(theBar,"glow","label=Glow");
//	TwAddVarRW(theBar,"GlowBrightness",TW_TYPE_FLOAT,&params_glow_brightness,"min=1 max=1000 step=1");


}

GlowPass::~GlowPass() {
	delete renderPassShader;
}

void GlowPass::doExecute() {

	outputFBO->write();
		renderPassShader->enable();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D,inputFBOs[0]->texPointer[0]);
			glUniform1i(glowColorUniform,0);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D,inputFBOs[0]->texPointer[1]);
			glUniform1i(glowFactorUniform,1);

			glUniform1f(glowBrightnessUniform,params_glow_brightness);

			glViewport(0,0,width/glowScale,width/glowScale);
			quad->draw();

	outputFBO->unbind();
	glViewport(0,0,width,width);
}

