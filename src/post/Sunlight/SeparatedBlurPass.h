#ifndef SEPARATED_BLUR_H_
#define SEPARATED_BLUR_H_

#include "../ISlimRenderPass.h"

class SeparatedBlurPass : public ISlimRenderPass
{
public:
	SeparatedBlurPass(SlimQuad* pQuad, int pWidth, int pHeight);
	virtual ~SeparatedBlurPass();

	virtual void doExecute();

	GLint colorUniform;
	GLint maskUniform;
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

#endif /* GLOWPASS_H_ */
