#include "CompositingPass.h"
//#include <GL/glew.h>


CompositingPass::CompositingPass(SlimQuad* pQuad, int pWidth, int pHeight)
: ISlimRenderPass(pQuad)
{
	quad = pQuad;
	width = pWidth;
	height = pHeight;
	renderPassShader = new SlimShader(RESOURCES_PATH "/shader_source/", "post.vert.glsl", "compositing.frag.glsl");

	sceneTextureUniform = glGetUniformLocation(renderPassShader->shaderProgram, "scene");
	skyTextureUniform = glGetUniformLocation(renderPassShader->shaderProgram, "sky");
	bloomTextureUniform = glGetUniformLocation(renderPassShader->shaderProgram, "bloom");
	bloomAmountUniform = glGetUniformLocation(renderPassShader->shaderProgram, "bloomAmount");
	

	//params
	param_bloomAmount = 1.0f;
}


CompositingPass::~CompositingPass() {
	delete renderPassShader;
}

void CompositingPass::doExecute() {

	outputFBO->write();
		renderPassShader->enable();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, inputFBOs[0]->texPointer[0]);
			glUniform1i(sceneTextureUniform, 0);
			
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, inputFBOs[0]->texPointer[1]);
			glUniform1i(sceneTextureUniform, 1);

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, inputFBOs[1]->texPointer[0]);
			glUniform1i(bloomTextureUniform, 2);



			glUniform1f(bloomAmountUniform, param_bloomAmount);
			quad->draw();
		
		renderPassShader->disable();
}

