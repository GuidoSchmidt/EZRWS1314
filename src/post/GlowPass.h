#ifndef GLOWPASS_H_
#define GLOWPASS_H_

//#include "../../Globals.h"
#include "ISlimRenderPass.h"

class GlowPass : public ISlimRenderPass
{
public:
	GlowPass(int scale, SlimQuad* pQuad, int pWidth, int pHeight);
	virtual ~GlowPass();

	virtual void doExecute();


	GLint glowColorUniform;
	GLint glowFactorUniform;
	GLint glowBrightnessUniform;


	GLint glowScale;
	GLfloat params_glow_brightness;
};

#endif /* GLOWPASS_H_ */
