#include "PhongPass.h"
//#include <GL/glew.h>


PhongPass::PhongPass(SlimQuad* pQuad, glm::vec2 pNearFar,int pWidth, int pHeight)//, int* pMouseX, int* pMouseY)
	: ISlimRenderPass(pQuad)
{
	width = pWidth;
	height = pHeight;
	nearFar = pNearFar;
	renderPassShader = new SlimShader("src/Passes/Lighting/","post.vert.glsl", "phong.frag.glsl");

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


//	TwAddSeparator(theBar,"lighting","label=Lighting");
//	TwAddVarRW(theBar,"Ambient Amount",TW_TYPE_FLOAT,&params_ambient,"min=0 max=1 step=0.1");
//	TwAddVarRW(theBar,"Diffuse Amount",TW_TYPE_FLOAT,&params_diffuse,"min=0 max=1 step=0.1");
//	TwAddVarRW(theBar,"Specular Amount",TW_TYPE_FLOAT,&params_specular,"min=0 max=1 step=0.1");
//	TwAddVarRW(theBar,"Specular Factor",TW_TYPE_FLOAT,&params_spec_fac,"min=1 max=100 step=1");
//
//	TwAddVarRW(theBar,"HDR lightPower",TW_TYPE_FLOAT,&params_hdr_lightPower,"min=0 max=10000 step=1");
//	TwAddVarRW(theBar,"HDR exposure",TW_TYPE_FLOAT,&params_hdr_exposure,"min=0.01 max=1000 step=0.1");

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

