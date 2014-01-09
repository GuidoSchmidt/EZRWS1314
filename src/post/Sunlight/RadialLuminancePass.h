#ifndef RADIAL_LUMINANCE_H_
#define RADIAL_LUMINANCE_H_

#include "../ISlimRenderPass.h"

class RadialLuminancePass : public ISlimRenderPass
{
public:
	RadialLuminancePass(SlimQuad* pQuad, int pWidth, int pHeight);
	virtual ~RadialLuminancePass();

	virtual void doExecute();

	GLint maskUniform;
	GLint nSamplesUniform;
	GLint scaleUniform;
	GLuint sunPosUniform;
	GLint screenSizeUniform;

	GLfloat param_nSamples;
	GLfloat param_scale;
	glm::vec4 param_ssSunPos;
};

#endif /* RADIAL_LUMINANCE_H_ */
