#ifndef RADIAL_GLOW_H_
#define RADIAL_GLOW_H_

#include "../ISlimRenderPass.h"

class RadialGlowMaskPass : public ISlimRenderPass
{
public:
	RadialGlowMaskPass(SlimQuad* pQuad, int pWidth, int pHeight);
	virtual ~RadialGlowMaskPass();

	virtual void doExecute();

	GLint colorUniform;
	GLint horizontalUniform;
	GLint glowRangeUniform;
	GLint glowBrightnessUniform;
	GLint glowThreshholdUniform;
	GLint screenSizeUniform;

	GLfloat param_glowRange;
	GLfloat param_glowThreshhold;
	GLfloat param_glowHorizontal;
	GLfloat param_glowBrightness;
};

#endif /* RADIAL_GLOW_H_ */
