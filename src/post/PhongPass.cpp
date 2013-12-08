#include "PhongPass.h"
//#include <GL/glew.h>


PhongPass::PhongPass(SlimQuad* pQuad, glm::vec2 pNearFar,int pWidth, int pHeight)//, int* pMouseX, int* pMouseY)
	: ISlimRenderPass(pQuad)
{
	width = pWidth;
	height = pHeight;
	nearFar = pNearFar;
	renderPassShader = new SlimShader("../../resources/shader_source/","post.vert.glsl", "phong.frag.glsl");

	normalUniform = glGetUniformLocation(renderPassShader->shaderProgram,"normals");
	colorUniform = glGetUniformLocation(renderPassShader->shaderProgram,"diffuse");
	nearFarUniform = glGetUniformLocation(renderPassShader->shaderProgram,"nearFar");
	screenSizeUniform = glGetUniformLocation(renderPassShader->shaderProgram,"screenSize");
//	mousePosUniform = glGetUniformLocation(renderPassShader->shaderProgram,"mousePos");
	ambientUniform = glGetUniformLocation(renderPassShader->shaderProgram,"ambientAmount");
	diffuseUniform = glGetUniformLocation(renderPassShader->shaderProgram,"diffuseAmount");
	specularUniform = glGetUniformLocation(renderPassShader->shaderProgram,"specularAmount");
	specFactorUniform = glGetUniformLocation(renderPassShader->shaderProgram,"specularFactor");
	//hdr
	hdrLightPowerUniform = glGetUniformLocation(renderPassShader->shaderProgram,"lightPower");
	hdrExposureUniform = glGetUniformLocation(renderPassShader->shaderProgram,"exposure");

	quad = pQuad;


	//params
	params_ambient=0.8f;
	params_diffuse=0.3f;
	params_specular=0.2f;
	params_spec_fac=3.0f;
	
	params_hdr_exposure=1;
	params_hdr_lightPower=0.3;


}

PhongPass::~PhongPass()
{
	delete renderPassShader;
}

void PhongPass::doExecute()
{

	outputFBO->write();
		renderPassShader->enable();

			//Pass uniforms to shader
			////textures
			//////color buffer + depth buffer
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D,inputFBOs[0]->texPointer[0]);
			glUniform1i(colorUniform,0);

			//////normals buffer (+ glo factor)
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D,inputFBOs[0]->texPointer[1]);
			glUniform1i(normalUniform,1);



			////nearfar distances
			glUniform2fv(nearFarUniform,1,glm::value_ptr(nearFar));

			////screensize
			glUniform2i(screenSizeUniform,(GLint)width,(GLint)height);

			////lighting
			glUniform1f(ambientUniform,params_ambient);
			glUniform1f(diffuseUniform,params_diffuse);
			glUniform1f(specularUniform,params_specular);
			glUniform1f(specFactorUniform,params_spec_fac);

			//hdr
			glUniform1f(hdrLightPowerUniform,params_hdr_lightPower);
			glUniform1f(hdrExposureUniform,params_hdr_exposure);


			////mousepos
//			glUniform2i(mousePosUniform,mouseX,mouseY);

			// renderQuad
			quad->draw();
		renderPassShader->disable();
	outputFBO->unbind();
}

