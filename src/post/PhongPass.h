#ifndef PHONGPASS_H_
#define PHONGPASS_H_

//#include "../../Globals.h"
#include "ISlimRenderPass.h"


class PhongPass : public ISlimRenderPass
{
public:
	PhongPass(SlimQuad* pQuad, glm::vec2 pNearFar, int pWidth, int pHeight);//, int* pMouseX, int* pMouseY);
	virtual ~PhongPass();

	virtual void doExecute();

	GLuint colorUniform;
	GLuint normalUniform;
	//GLuint glowUniform;
	GLuint nearFarUniform;
	GLuint screenSizeUniform;
//	GLuint mousePosUniform;
	GLuint ambientUniform;
	GLuint diffuseUniform;
	GLuint specularUniform;
	GLuint specFactorUniform;
	//hdr
	GLuint hdrExposureUniform;
	GLuint hdrLightPowerUniform;


	GLfloat params_ambient;
	GLfloat params_diffuse;
	GLfloat params_specular;
	GLfloat params_spec_fac;
	//hdr
	GLfloat params_hdr_exposure;
	GLfloat params_hdr_lightPower;
//
//	int mouseX;
//	int mouseY;
	glm::vec2 nearFar;

};

#endif /* PHONGPASS_H_ */
